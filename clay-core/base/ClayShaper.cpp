/**************************************************************************\
*
*  This file is part of the C++ Modular Development Framework.
*  Copyright (C) 2009 by Christoph Bastuck.  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU General Public License
*  ("GPL") version 2 as published by the Free Software Foundation.
*  See the file LICENSE.GPL at the root directory of this source
*  distribution for additional information about the GNU GPL.
*
*  For using this framework with software that can not be combined with the GNU
*  GPL please contact mail@cbastuck about acquiring a commercial license.
*
*  See http://www.cbastuck.de/ for more information.
*
\**************************************************************************/

#include "ClayShaper.h"
#include "ModuleDependencies.h"

#include <boost/bind.hpp>

#include <iostream>

namespace CLAY{

//---------------------------------------------ClayShaper
ClayShaper::ClayShaper()
  : m_pPriorityList(NULL)
{
}

//---------------------------------------------ClayShaper
ClayShaper::ClayShaper(const tModuleCollection* pPriorityList)
  : m_pPriorityList(pPriorityList)
{
}

//---------------------------------------------~ClayShaper
ClayShaper::~ClayShaper()
{
}

//---------------------------------------------addExternalModule
bool ClayShaper::addExternalModule(Module* pModule)
{
  if(std::find(m_aExternalModules.begin(), m_aExternalModules.end(), pModule) == m_aExternalModules.end())
  {
    m_aExternalModules.push_back(pModule);
    return true;
  }
  return false;
}

//---------------------------------------------clearExternals
void ClayShaper::clearExternals()
{
  m_aExternalModules.clear();
}
  
//---------------------------------------------shapeProcess
Module::CompileResultCode ClayShaper::shapeProcess(Module* pModule, ClayExecutable& aTarget)
{
  ModuleDependencies aDirectDependencies;
  pModule->visitTraversal(boost::bind(&ClayShaper::collectDependenciesHelper, this, _1, boost::ref(aDirectDependencies)));

  ModuleDependencies aTransitiveDependencies = aDirectDependencies;
  aTransitiveDependencies.computeTransitiveHull();

  return shapeProcess(aDirectDependencies, aTransitiveDependencies, aTarget);
}

//---------------------------------------------shapeProcess
Module::CompileResultCode ClayShaper::shapeProcess(const tModuleCollection& collModules, ClayExecutable& aTarget)
{
  if(collModules.size() == 0) 
  {
    return Module::NothingToCompile_Error;
  }

  tModuleCollection::const_iterator it  = collModules.begin();
  tModuleCollection::const_iterator end = collModules.end();

  ModuleDependencies aDirectDependencies;
  for(; it!=end; ++it)
  {
    Module* pModule = *it;
    if(aDirectDependencies.isUnknown(pModule)) //in this case the module has not already been processed by dependency checker
    {
      pModule->visitTraversal(boost::bind(&ClayShaper::collectDependenciesHelper, this, _1, boost::ref(aDirectDependencies)));
    }
  }
  ModuleDependencies aTransitiveDependencies = aDirectDependencies;
  aTransitiveDependencies.computeTransitiveHull(); //include transitive connections  

  return shapeProcess(aDirectDependencies, aTransitiveDependencies, aTarget); //compile the program by resolving the dependencies
}

//---------------------------------------------shapeProcess
Module::CompileResultCode ClayShaper::shapeProcess(ModuleDependencies& aDirectDependencies, ModuleDependencies& aTransitiveDependencies, ClayExecutable& aTarget)
{
  if(aTransitiveDependencies.hasRecursiveModule())
  {
    return shapeRecursiveExecutable(aDirectDependencies, aTransitiveDependencies, aTarget);
  }
  else
  {
    return shapeExecutable(aTransitiveDependencies, aTarget);
  }

  return Module::Compile_OK;
}

//---------------------------------------------shapeExecutable
Module::CompileResultCode ClayShaper::shapeExecutable(ModuleDependencies& aDeps, ClayExecutable& aTarget)
{
  Module::CompileResultCode aResult = Module::Compile_OK;
  Module* pModule = aDeps.getMostIndependentModule();
  while(pModule && aResult != Module::Compile_ERROR)
  {
    if(aDeps.getNumDependencies(pModule))
    {
      if(!checkExternalDependencies(pModule, aDeps))
      { 
        /*
         * In this case every open dependency must be declared as external - otherwise this is an error.
         * Note external dependencies are resolved 'outside', i.e. before depending modules are processed.
         */
        return Module::UnresolvedDependencies_ERROR;
      }
    }

    aResult = pModule->shapeProcess(this, aTarget);
    if(aResult != Module::Compile_OK)
    {
      return aResult;
    }

    aDeps.removeDependencies(pModule);
    pModule = aDeps.getMostIndependentModule();
  }

  return aResult;
}

//---------------------------------------------compileRecursiveExecutable
Module::CompileResultCode ClayShaper::shapeRecursiveExecutable(ModuleDependencies& aDirectDependencies, 
                                                               const ModuleDependencies& aTransitiveDependencies, 
                                                               ClayExecutable& aTarget)
{
  if(!m_pPriorityList)
  {
    CLAY_FAIL(); //can not compile a recursive setup without defining priority modules
    return Module::Compile_ERROR;
  }

  //find the priority module in the recursive chain
  Module* pModule = aTransitiveDependencies.getRecursiveModule();
  if(!pModule)
  {
    CLAY_FAIL(); //do not call this method without recursive setup
    return Module::Compile_ERROR;
  }

  //remove dependencies to the priority module as this will be the start module
  Module* pPriorityModule = getPriorityModule(pModule, aTransitiveDependencies);
  if(!pPriorityModule)
  {
    CLAY_FAIL();
    return Module::Compile_ERROR;
  }

  //shape the priority module and remove dependencies
  Module::CompileResultCode aError = pPriorityModule->shapeProcess(this, aTarget);
  if(aError != Module::Compile_OK)
  {
    return aError;
  }
  aDirectDependencies.removeDependencies(pPriorityModule/*, true*/);

  //recomupte the transitive hull
  aDirectDependencies.computeTransitiveHull();
  CLAY_ASSERT(!aDirectDependencies.hasRecursiveModule()); //now we should have a non-recursive setup
  
  return shapeExecutable(aDirectDependencies, aTarget);
}

//---------------------------------------------getPriorityModule
Module* ClayShaper::getPriorityModule(Module* pModule, const ModuleDependencies& aDeps)
{
  CLAY_ASSERT(m_pPriorityList);
  if(m_pPriorityList)
  {
    Module* pPriorityModule = NULL;
    if(std::find(m_pPriorityList->begin(), m_pPriorityList->end(), pModule) == m_pPriorityList->end())
    {
      ModuleDependencies::tModuleDep::const_iterator it = aDeps.beginDep(pModule);
      ModuleDependencies::tModuleDep::const_iterator ed = aDeps.endDep(pModule);
      for(; it!=ed; ++it)
      {
        Module* pDependency = *it;
        if(std::find(m_pPriorityList->begin(), m_pPriorityList->end(), pDependency) != m_pPriorityList->end())
        {
          return pDependency;
        }
      }
    }
    else
    {
      return pModule;
    }
  }
  return NULL;
}

//---------------------------------------------checkExternalDependencies
bool ClayShaper::checkExternalDependencies(Module* pModule, const ModuleDependencies& aDeps)
{
  ModuleDependencies::tModuleDep::const_iterator it = aDeps.beginDep(pModule);
  ModuleDependencies::tModuleDep::const_iterator ed = aDeps.endDep(pModule);
  for(; it!=ed; ++it)
  {
    Module* pDependency = *it;
    if(std::find(m_aExternalModules.begin(), m_aExternalModules.end(), pDependency) == m_aExternalModules.end())
    {
      return false; //this dependency is not declared as external - error
    }
  }
  return true; //all dependency are declared as external
}

//---------------------------------------------collectDependenciesHelper
void ClayShaper::collectDependenciesHelper(Module* pModule, ModuleDependencies& aDst)
{
  aDst.addModule(pModule);
  for(unsigned int i=0, n=pModule->getNumInputs(); i<n; ++i)
  {
    ModuleInputBase* pInput = pModule->getInput(i);
    if(pInput->isConnected())
    {
      Module* pModuleAtInput = pInput->getSourceModule();
      aDst.addDependency(pModule, pModuleAtInput);
    }
  }
}

}