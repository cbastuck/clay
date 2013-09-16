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

#include "ModuleUIRegistry.h"

#include <clay-core/nugget/IClayNugget.h>

namespace CLAY{ namespace UI{ 

//---------------------------------------------ModuleUIRegistry
ModuleUIRegistry::ModuleUIRegistry()
{

}

//---------------------------------------------~ModuleUIRegistry
ModuleUIRegistry::~ModuleUIRegistry()
{

}

//---------------------------------------------init
void ModuleUIRegistry::init()
{

}

//---------------------------------------------deInit
void ModuleUIRegistry::deInit()
{

}

//---------------------------------------------registerModuleUI
void ModuleUIRegistry::registerModuleUI(const ModuleUIDescriptor& aDescriptor)
{
  m_collRegisteredUIDescriptors.insert(std::make_pair(aDescriptor.getModuleURI(), aDescriptor));
}

//---------------------------------------------registerModuleUIs
void ModuleUIRegistry::registerModuleUIs(NUGGET::IClayNugget* pClayNugget)
{
  for(unsigned int i=0, n=pClayNugget->getNumModuleUIs(); i<n; ++i)
  {
    const ModuleUIDescriptor* pDescriptor = pClayNugget->getModuleUIDescriptor(i);
    registerModuleUI(*pDescriptor);
  }
}

//---------------------------------------------getNumModuleUIs
unsigned int ModuleUIRegistry::getNumModuleUIs() const
{
  return m_collRegisteredUIDescriptors.size();
}

//---------------------------------------------getModuleUIDescriptor
UI::ModuleUIDescriptor* ModuleUIRegistry::getModuleUIDescriptor(unsigned int uIdx)
{
  CLAY_ASSERT(getNumModuleUIs() > uIdx);
  tModuleUIFactoryCollection::iterator it  = m_collRegisteredUIDescriptors.begin();
  tModuleUIFactoryCollection::iterator end = m_collRegisteredUIDescriptors.end();
  for(unsigned int i=0; it!=end; ++i, ++it)
  {
    if(i == uIdx)
    {
      return &it->second;
    }
  }
  return NULL;
}

} }
