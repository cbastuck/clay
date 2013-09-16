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

//---------------------------------------------registerModule
bool ModuleRegistry::registerModule(const char* moduleURI, tModuleFactory fac)
{
  return addItem(moduleURI, fac);
}

//---------------------------------------------registerModules
bool ModuleRegistry::registerModules(const ModuleRegistry& aOther)
{
  ModuleRegistry::const_iterator it = aOther.begin();
  ModuleRegistry::const_iterator end = aOther.end();
  for(; it!=end; ++it)
  {
    addItem(it->first.c_str(), it->second);
  }
  return true;
}

//---------------------------------------------registerModules
bool ModuleRegistry::registerModules(NUGGET::IClayNugget* pNugget)
{
  unsigned int uNumModules = pNugget->getNumModules();
  if(uNumModules == 0)
  {
    return false;
  }

  for(unsigned int i=0; i<uNumModules; ++i)
  {
    const char* uri = pNugget->getModuleURI(i);
    tModuleFactory fac = pNugget->getModuleFactory(i);
    if(!uri || !fac)
    {
      return false;
    }

    if(!registerModule(uri, fac))
    {
      return false;
    }
  }
  return true;
}

//---------------------------------------------createItem
Module* ModuleRegistry::createItem(const char* sModuleURI, const tString& sRuntimeModuleId)
{
  iterator pos = m_registeredModules.find(sModuleURI);
  if(pos != m_registeredModules.end())
  {
    return pos->second(sRuntimeModuleId);
  }
  CLAY_FAIL();
  return NULL;
}

//---------------------------------------------destroyItem
void ModuleRegistry::destroyItem(Module* pModule)
{
  CLAY_ASSERT(pModule);
  //TODO: before we called descriptors destroy method, check if this is ok
  delete pModule;
}

//---------------------------------------------getNumRegisteredModules
unsigned int ModuleRegistry::getNumRegisteredModules() const
{
  return m_registeredModules.size();
}

//---------------------------------------------
const char* ModuleRegistry::getModuleURI(unsigned int uIdx) const
{
  const_iterator it = getEntry(uIdx);
  if(it == m_registeredModules.end())
  {
    return NULL;  
  }
  return it->first.c_str();
}

//---------------------------------------------
ModuleRegistry::tModuleFactory ModuleRegistry::getModuleFactory(unsigned int uIdx) const
{
  const_iterator it = getEntry(uIdx);
  if(it == m_registeredModules.end())
  {
    return NULL;
  }
  return it->second;
}

//---------------------------------------------addItem
bool ModuleRegistry::addItem(const char* moduleURI, tModuleFactory fac)
{
  tFactoryCollection::const_iterator pos = m_registeredModules.find(moduleURI);
  if(pos != m_registeredModules.end())
  {
      return false;
  }
  
  return m_registeredModules.insert(std::make_pair(moduleURI, fac)).second;
}

//---------------------------------------------
ModuleRegistry::tFactoryCollection::const_iterator ModuleRegistry::getEntry(unsigned int uIdx) const
{
  if(uIdx < getNumRegisteredModules())
  {
    const_iterator it = m_registeredModules.begin();
    std::advance(it, uIdx);
    return it;
  }
  return m_registeredModules.end();
}

}
