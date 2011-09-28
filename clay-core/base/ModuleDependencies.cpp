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

#include "ModuleDependencies.h"
#include "Module.h"

#include <limits>
#include <iostream>

namespace CLAY
{

//---------------------------------------------ModuleDependencies
ModuleDependencies::ModuleDependencies()
{

}

//---------------------------------------------addModule
void ModuleDependencies::addModule(Module* pModule)
{
  m_collDependencies[pModule];
}

//---------------------------------------------addDependency
void ModuleDependencies::addDependency(Module* pModule, Module* pDependency)
{
  tModuleDep& aModuleDep = m_collDependencies[pModule];
  aModuleDep.insert(pDependency);
}

//---------------------------------------------removeDependencies
void ModuleDependencies::removeDependencies(Module* pModule, bool bOnlyOutgoingDependencies)
{
  //remove outgoing dependencies
  tDepCollection::iterator it  = m_collDependencies.begin();
  tDepCollection::iterator end = m_collDependencies.end();
  for(; it!=end; ++it)
  {
    tModuleDep& aCurrent = it->second;
    tModuleDep::iterator pos = aCurrent.find(pModule);
    if(pos != aCurrent.end())
    {
      aCurrent.erase(pos);
    }
  }

  if(!bOnlyOutgoingDependencies)
  {
    //also remove the incoming dependencies
    m_collDependencies.erase(pModule);
  }
}

//---------------------------------------------getDependencies
const ModuleDependencies::tModuleDep* ModuleDependencies::getDependencies(Module* pModule) const
{
  tDepCollection::const_iterator pos = m_collDependencies.find(pModule);
  if(pos!=m_collDependencies.end())
    return &pos->second;
  return NULL;
}

//---------------------------------------------isUnknown
bool ModuleDependencies::isUnknown(Module* pModule) const
{
  return getDependencies(pModule) == NULL;
}

//---------------------------------------------getNumDependencies
ModuleDependencies::tDepCollection::size_type ModuleDependencies::getNumDependencies(Module* pModule) const
{
  const ModuleDependencies::tModuleDep* pDep = getDependencies(pModule);
  if(pDep)
  {
    return pDep->size();
  }
  return 0;
}

//---------------------------------------------beginDep
ModuleDependencies::tModuleDep::const_iterator ModuleDependencies::beginDep(Module* pModule) const
{
  tDepCollection::const_iterator pos = m_collDependencies.find(pModule);
  if(pos!=m_collDependencies.end())
    return pos->second.begin();
    
  ModuleDependencies::tModuleDep::const_iterator dummy;
  return dummy;
}

//---------------------------------------------endDep
ModuleDependencies::tModuleDep::const_iterator ModuleDependencies::endDep(Module* pModule) const
{
  tDepCollection::const_iterator pos = m_collDependencies.find(pModule);
  if(pos!=m_collDependencies.end())
    return pos->second.end();

  ModuleDependencies::tModuleDep::const_iterator dummy;
  return dummy;
}

//---------------------------------------------computeTransitiveHull
void ModuleDependencies::computeTransitiveHull(Module* pModule, tModuleDep* pDst, std::set<Module*>* pVisitedModules)
{
  if(!pModule)
  {
    tDepCollection::iterator it  = m_collDependencies.begin();
    tDepCollection::iterator end = m_collDependencies.end();
    for(; it!=end; ++it)
    {
      std::set<Module*> collVisitedModules;
      computeTransitiveHull(it->first, &it->second, &collVisitedModules); //recursive call
    }
  }
  else
  {
    CLAY_ASSERT(pDst);
    CLAY_ASSERT(pVisitedModules);
    pVisitedModules->insert(pModule);
    const tModuleDep* pDep = getDependencies(pModule);
    if(pDep)
    {
      tModuleDep::const_iterator ti  = pDep->begin();
      tModuleDep::const_iterator dne = pDep->end();
      for(; ti!=dne; ++ti)
      {
        pDst->insert(*ti);
        if(pVisitedModules->find(*ti) == pVisitedModules->end()) //avoid cycles ...
        {
          computeTransitiveHull(*ti, pDst, pVisitedModules); //... on recursive call
        }
      }
    }
  }
}

//---------------------------------------------hasRecursiveModule
bool ModuleDependencies::hasRecursiveModule() const
{
  return getRecursiveModule() != NULL;
}

//---------------------------------------------getRecursiveModule
Module* ModuleDependencies::getRecursiveModule() const
{
  tDepCollection::const_iterator it  = m_collDependencies.begin();
  tDepCollection::const_iterator end = m_collDependencies.end();
  for(; it!=end; ++it)
  {
    const tModuleDep& aCurrent = it->second;
    if(aCurrent.find(it->first) != aCurrent.end())
    {
      return it->first; // in this case a module depends on itself, i.e. an infinite recursion
    }
  }
  return false;
}

//---------------------------------------------getMostIndependentModule
Module* ModuleDependencies::getMostIndependentModule()
{
  Module* pResult = NULL;
  tModuleDep::size_type min = std::numeric_limits<tModuleDep::size_type>::max();
  tDepCollection::const_iterator it  = m_collDependencies.begin();
  tDepCollection::const_iterator end = m_collDependencies.end();
  for(; it!=end; ++it)
  {
    const tModuleDep& aCurrent = it->second;
    tModuleDep::size_type n = aCurrent.size();
    if(n < min)
    {
      pResult = it->first;
      min = n;
    }
  }
  return pResult;
}

//---------------------------------------------debugPrint
void ModuleDependencies::debugPrint()
{
  tDepCollection::iterator it  = m_collDependencies.begin();
  tDepCollection::iterator end = m_collDependencies.end();
  for(; it!=end; ++it)
  {
    tModuleDep& aCurrent = it->second;
    std::cout<<"Dependencies for: "<<it->first->getRuntimeModuleID()<<std::endl;
    tModuleDep::const_iterator ti  = aCurrent.begin();
    tModuleDep::const_iterator dne = aCurrent.end();
    for(; ti!=dne; ++ti)
    {
      std::cout<<"\t"<<(*ti)->getRuntimeModuleID()<<std::endl;
    }
  }
}

}
