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

#include "QtModuleUIRegistry.h"
#include <clay-core/base/Module.h>
#include <clay-gui-qt/base/ModuleWidget.h>

#include <QWidget>

#include <clay-core/nugget/IClayNugget.h>

namespace CLAY{ namespace UI{

//---------------------------------------------instance
QtModuleUIRegistry* QtModuleUIRegistry::instance()
{
  static QtModuleUIRegistry aInstance;
  return &aInstance;
}

//---------------------------------------------create
ModuleWidget* QtModuleUIRegistry::createUI(Module* pModule, const char* moduleURI, Host* pHost, QWidget* pParent)
{
    ModuleWidget* pModuleWidget = createRegisteredModuleUI(moduleURI,
                                                           pModule,
                                                           pHost, 
                                                           pParent);
    if(!pModuleWidget)
    {
      pModuleWidget = new ModuleWidget(pModule, pHost, pParent); //default empty widget
    }
        
    pModuleWidget->init();
    return pModuleWidget;

  return NULL;
}

//---------------------------------------------destroyUI
void QtModuleUIRegistry::destroyUI(ModuleWidget* pWidget)
{
  ModuleUIDescriptor* pDescriptor = pWidget->getUIDescriptor();
  if(pDescriptor)
  {
    pDescriptor->getDestroyer()(pWidget);
  }
  else
  {
    delete pWidget; //no descriptor available - do it here
  }
}

//---------------------------------------------createRegisteredModuleUI
ModuleWidget* QtModuleUIRegistry::createRegisteredModuleUI(const char* moduleURI, Module* pModule, Host* pHost, QWidget* pParent)
{
  tModuleUIFactoryCollection::const_iterator pos = m_collRegisteredUIDescriptors.find(moduleURI);
  if(pos != m_collRegisteredUIDescriptors.end())
  {
    const ModuleUIDescriptor& aDescriptor = pos->second;
    ModuleUIDescriptorTraits::tFactoryMethod aFactory = aDescriptor.getFactory();
    return aFactory(pModule, pHost, pParent);
  }
  return NULL;
}

} }
