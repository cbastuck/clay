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

#ifndef MODULE_UI_REGISTRY_H_
#define MODULE_UI_REGISTRY_H_

//clay-core
#include <clay-core/base/ClayDefines.h>
#include <clay-core/base/ModuleUIDescriptor.h>

//STL
#include <map>

namespace CLAY{ namespace UI{ class ModuleUIDescriptor; } }

namespace CLAY{ namespace NUGGET{ class IClayNugget; } }

namespace CLAY{ namespace UI{ 

class ModuleUIRegistry
{
public:
  CLAY_DLL_EXPORT ModuleUIRegistry();
  CLAY_DLL_EXPORT ~ModuleUIRegistry();

  CLAY_DLL_EXPORT void init();
  CLAY_DLL_EXPORT void deInit();

  template<class MODULE, class UI>
  CLAY_DLL_EXPORT void registerModuleUI(); //inline
  CLAY_DLL_EXPORT void registerModuleUI(const ModuleUIDescriptor& aDescriptor);
  CLAY_DLL_EXPORT void registerModuleUIs(NUGGET::IClayNugget* pClayNugget);

  CLAY_DLL_EXPORT unsigned int getNumModuleUIs() const;

  CLAY_DLL_EXPORT UI::ModuleUIDescriptor* getModuleUIDescriptor(unsigned int uIdx);

protected:
  typedef std::map<const char*, ModuleUIDescriptor> tModuleUIFactoryCollection;

  tModuleUIFactoryCollection m_collRegisteredUIDescriptors;
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------registerModuleUI
template<class MODULE, class UI>
inline void ModuleUIRegistry::registerModuleUI()
{
  ModuleUIDescriptor aDescriptor(UI::create, 
                                 UI::destroy, 
                                 MODULE::staticModuleURI());
  registerModuleUI(aDescriptor);
}

} }

#endif 