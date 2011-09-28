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

#ifndef QT_ModuleUIRegistry_h_
#define QT_ModuleUIRegistry_h_

//clay-core
#include <clay-core/base/ClayDefines.h>
#include <clay-core/registry/ModuleUIRegistry.h>

//STL
#include <map>

class QWidget;

namespace CLAY{ namespace NUGGET{ 
class IClayNugget;
} }

namespace CLAY{
class Host;
class Module;
}
  
namespace CLAY{namespace UI{

class ModuleWidget;

class QtModuleUIRegistry : public ModuleUIRegistry
{
public:
  static QtModuleUIRegistry* instance();

  ModuleWidget* createUI(Module* pModule, ModuleDescriptorTraits::tModuleID aId, Host* pHost, QWidget* pParent);

  void destroyUI(ModuleWidget* pWidget);

protected:
  ModuleWidget* createRegisteredModuleUI(ModuleDescriptorTraits::tModuleID    aModuleID,
                                          ModuleDescriptorTraits::tNamespaceID aModuleNS,
                                          Module* pModule, Host* pHost, QWidget* pParent);

private:
      
};

} }

#endif 
