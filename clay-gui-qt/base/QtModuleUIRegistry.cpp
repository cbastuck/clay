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

/*
#include <clay-modules/generators/MultiDimensionalRandomOutput.h>
#include <clay-modules/graphic/SceneGraphOutputModule.h>
#include <clay-modules/graphic/SceneGraphInputModule.h>
#include <clay-modules/web/XMLResourceProvider.h>
#include <clay-modules/monitors/ConsoleMonitor.h>
#include <clay-modules/audio/AudioInputModule.h>
#include <clay-modules/audio/WavReaderModule.h>
#include <clay-modules/midi/MidiOutputModule.h>
#include <clay-modules/midi/MidiRouterModule.h>
#include <clay-modules/midi/MidiInputModule.h>
#include <clay-modules/audiofx/GainModule.h>
#include <clay-modules/timer/TimerModule.h>
#include <clay-modules/script/LuaModule.h>
#include <clay-modules/host/HostModule.h>

//UI classes
#include "staticUI/MultiDimensionalRandomOutputModule/MultiDimensionalRandomOutputUI.h"
#include "staticUI/OpenInventorExaminerViewer/OpenInventorExaminerViewer.h"
#include "staticUI/ConsoleMonitorUI/ConsoleMonitorUI.h"
#include "staticUI/MidiInputModuleUI/MidiInputModuleUI.h"
#include "staticUI/MidiOutputModuleUI/MidiOutputModuleUI.h"
#include "staticUI/AudioInputModuleUI/AudioInputModuleUI.h"
#include "staticUI/WAVReaderModuleUI/WAVReadModuleUI.h"
#include "staticUI/GainModuleUI/GainModuleUI.h"
#include "staticUI/LuaModuleUI/LuaModuleUI.h"
#include "staticUI/HostModuleUI/HostModuleUI.h"
#include "staticUI/misc/XMLResourceProviderUI.h"
*/

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
ModuleWidget* QtModuleUIRegistry::createUI(Module* pModule, ModuleDescriptorTraits::tModuleID aId, Host* pHost, QWidget* pParent)
{
  /*
  if(aId == MODULE::MultiDimensionalRandomOutput::tDescriptor::getModuleId())
  {
    //GenericRuntimeUI* pModuleWidget = new GenericRuntimeUI(pModule, pParent, "MultiDimensionalRandomOutput.ui", sUserId);
    MultiDimensionalRandomOutputUI* pModuleWidget = new MultiDimensionalRandomOutputUI(pModule, pHost, pParent);
    pModuleWidget->init();
    return pModuleWidget;
  }
  else if(aId == MODULE::ConsoleMonitor::tDescriptor::getModuleId())
  {
    //GenericRuntimeUI* pModuleWidget = new GenericRuntimeUI(pModule, pParent, "ConsoleMonitor.ui", sUserId);
    ConsoleMonitorUI* pModuleWidget = new ConsoleMonitorUI(pModule, pHost, pParent);
    pModuleWidget->init();
    return pModuleWidget;
  }
  else if(aId == MODULE::MidiOutputModule::tDescriptor::getModuleId())
  {
    MidiOutputModuleUI* pModuleWidget = new MidiOutputModuleUI(pModule, pHost, pParent);
    pModuleWidget->init();
    return pModuleWidget;
  }
  else if(aId == MODULE::MidiInputModule::tDescriptor::getModuleId())
  {
    MidiInputModuleUI* pModuleWidget = new MidiInputModuleUI(pModule, pHost, pParent);
    pModuleWidget->init();
    return pModuleWidget;
  }
  else if(aId == MODULE::MidiRouterModule::tDescriptor::getModuleId())
  {
    ModuleWidget* pModuleWidget = new ModuleWidget(pModule, pHost, pParent);
    pModuleWidget->init();
    return pModuleWidget;
  }
  else if(aId == MODULE::TimerModule::tDescriptor::getModuleId())
  {
    ModuleWidget* pModuleWidget = new ModuleWidget(pModule, pHost, pParent);
    pModuleWidget->init();
    return pModuleWidget;
  }
  else if(aId == MODULE::AudioInputModule::tDescriptor::getModuleId())
  {
    AudioInputModuleUI* pModuleWidget = new AudioInputModuleUI(pModule, pHost, pParent);
    pModuleWidget->init();
    return pModuleWidget;
  }
  else if(aId == MODULE::LuaModule::tDescriptor::getModuleId())
  {
    LuaModuleUI* pModuleWidget = new LuaModuleUI(pModule, pHost, pParent);
    pModuleWidget->init();
    return pModuleWidget;
  }
  else if(aId == MODULE::HostModule::tDescriptor::getModuleId())
  {
    HostModuleUI* pModuleWidget = new HostModuleUI(pModule, pHost, pParent);
    pModuleWidget->init();
    return pModuleWidget;
  }
  else if(aId == MODULE::GainModule::tDescriptor::getModuleId())
  {
    GainModuleUI* pModuleWidget = new GainModuleUI(pModule, pHost, pParent);
    pModuleWidget->init();
    return pModuleWidget;
  }
  else if(aId == MODULE::SceneGraphInputModule::tDescriptor::getModuleId())
  {
    OpenInventorExaminerViewer* pExaminer = new OpenInventorExaminerViewer(pModule, pHost, pParent);
    pExaminer->init();
    return pExaminer;
  }
  else if(aId == MODULE::XMLResourceProvider::tDescriptor::getModuleId())
  {
    XMLResourceProviderUI* pModuleWidget = new XMLResourceProviderUI(pModule, pHost, pParent);
    pModuleWidget->init();
    return pModuleWidget;
  }
  else if(aId == MODULE::WavReaderModule::tDescriptor::getModuleId())
  {
    WAVReaderModuleUI* pModuleWidget = new WAVReaderModuleUI(pModule, pHost, pParent);
    pModuleWidget->init();
    return pModuleWidget;
  }
  else
  */
  {
    ModuleWidget* pModuleWidget = createRegisteredModuleUI(pModule->getModuleDescriptor()->getModuleId(), 
                                                           pModule->getModuleDescriptor()->getNamespaceId(),
                                                           pModule,
                                                           pHost, 
                                                           pParent);
    if(!pModuleWidget)
    {
      pModuleWidget = new ModuleWidget(pModule, pHost, pParent); //default empty widget
    }
        
    pModuleWidget->init();
    return pModuleWidget;
  }

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
ModuleWidget* QtModuleUIRegistry::createRegisteredModuleUI(ModuleDescriptorTraits::tModuleID aModuleID,
                                                           ModuleDescriptorTraits::tNamespaceID aModuleNS,
                                                           Module* pModule, Host* pHost, QWidget* pParent)
{
  ModuleDescriptorTraits::tCompoundIdentifier aCompoundID = ModuleDescriptorTraits::encode(aModuleID, aModuleNS);
  tModuleUIFactoryCollection::const_iterator pos = m_collRegisteredUIDescriptors.find(aCompoundID);
  if(pos != m_collRegisteredUIDescriptors.end())
  {
    const ModuleUIDescriptor& aDescriptor = pos->second;
    ModuleUIDescriptorTraits::tFactoryMethod aFactory = aDescriptor.getFactory();
    return aFactory(pModule, pHost, pParent);
  }
  return NULL;
}

} }
