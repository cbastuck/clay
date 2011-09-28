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

#include "ModuleRegistry.h"

#include <clay-core/helper/SharedLibrary.h>
#include <clay-core/base/ModuleDescriptor.h>
#include <clay-core/base/Module.h>

#include <clay-core/nugget/IClayNugget.h> //CLAY::NUGGET support

namespace CLAY{
  
//---------------------------------------------ModuleRegistry
ModuleRegistry::ModuleRegistry()
{

}

//---------------------------------------------~ModuleRegistry
ModuleRegistry::~ModuleRegistry()
{

}

//---------------------------------------------registerModules
bool ModuleRegistry::registerModules(const ModuleRegistry& aOther)
{
  ModuleRegistry::const_namespace_iterator it  = aOther.beginNamespace();
  ModuleRegistry::const_namespace_iterator end = aOther.endNamespace();
  for(; it!=end; ++it)
  {
    ModuleDescriptorTraits::tNamespaceID aNamespaceId = it->first;

    const tDescriptorCollection& aCollection = it->second;
    ModuleRegistry::const_descr_iterator ti  = aCollection.begin();
    ModuleRegistry::const_descr_iterator dne = aCollection.end();
    for(; ti!=dne; ++ti)
    {
      addItem(ti->second);
    }
  }
  return true;
}

//---------------------------------------------registerModule
void ModuleRegistry::registerModule(const ModuleDescriptorBase& aDescriptor)
{
  addItem(aDescriptor);
}

//---------------------------------------------registerModules
bool ModuleRegistry::registerModules(NUGGET::IClayNugget* pNugget)
{
  unsigned int uNumModules = pNugget->getNumModules();
  for(unsigned int i=0; i<uNumModules; ++i)
  {
    const ModuleDescriptorBase* pDescriptor = pNugget->getModuleDescriptor(i);
    CLAY_ASSERT(pDescriptor);
    registerModule(*pDescriptor);
  }
  return (uNumModules) ? true : false;
}

//---------------------------------------------createItem
Module* ModuleRegistry::createItem(ModuleDescriptorTraits::tModuleID    aModuleId, 
                                   ModuleDescriptorTraits::tNamespaceID aNamespaceId, 
                                   const tString&                       sRuntimeModuleId)
{
  namespace_iterator posNS = m_collRegisteredNamespaces.find(aNamespaceId);
  if(posNS != m_collRegisteredNamespaces.end())
  {
    tDescriptorCollection& collDescriptors = posNS->second;
    descr_iterator posDescr = collDescriptors.find(aModuleId);
    if(posDescr != collDescriptors.end())
    {
      return posDescr->second.create(sRuntimeModuleId);
    }
  }
  CLAY_FAIL();
  return NULL;
}

//---------------------------------------------destroyItem
void ModuleRegistry::destroyItem(Module* pModule)
{
  const ModuleDescriptorBase* pDescriptor = pModule->getModuleDescriptor();
  CLAY_ASSERT(pDescriptor);
  pDescriptor->getDestroyer()(pModule);
}

//---------------------------------------------getNumModules
unsigned int ModuleRegistry::getNumModules() const
{
  unsigned int uRes = 0;
  const_namespace_iterator it  = beginNamespace();
  const_namespace_iterator end = endNamespace();
  for(; it!=end; ++it)
  {
    uRes += it->second.size();
  }
  return uRes;
}

//---------------------------------------------getNumNamespaces
unsigned int ModuleRegistry::getNumNamespaces() const
{
  return m_collRegisteredNamespaces.size();
}

//---------------------------------------------getModuleDescriptor
const ModuleDescriptorBase* ModuleRegistry::getModuleDescriptor(unsigned int uIdx) const
{
  CLAY_ASSERT(getNumModules() > uIdx);
  unsigned int uIter = 0;

  const_namespace_iterator it  = beginNamespace();
  const_namespace_iterator end = endNamespace();
  for(; it!=end; ++it)
  {
    const tDescriptorCollection& collDesc = it->second;
    tDescriptorCollection::const_iterator ti = collDesc.begin();
    tDescriptorCollection::const_iterator dne = collDesc.end();
    for(; ti!=dne; ++ti)
    {
      if(uIter == uIdx)
      {
        return &ti->second;
      }
      ++uIter;
    }
  }
  return NULL; //descriptor not found (invalid index?)
}

//---------------------------------------------addItem
void ModuleRegistry::addItem(const ModuleDescriptorBase& aDescriptor)
{
  m_collRegisteredNamespaces[aDescriptor.getNamespaceId()][aDescriptor.getModuleId()] = aDescriptor;
}

}
