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

#include "StandardRegistry.h"

/*
#include <clay-modules/generators/MultiDimensionalRandomOutput.h>
#include <clay-modules/graphic/SceneGraphOutputModule.h>
#include <clay-modules/graphic/SceneGraphInputModule.h>
#include <clay-modules/audio/SampleBufferOutputModule.h>
#include <clay-modules/audio/AudioBufferOutputModule.h>
#include <clay-modules/audio/SampleBufferInputModule.h>
#include <clay-modules/audio/AudioBufferInputModule.h>
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
*/

namespace CLAY{ namespace MODULE{

//---------------------------------------------instance
StandardRegistry* StandardRegistry::instance()
{
  static StandardRegistry* pInstance = NULL;
  if(!pInstance)
  {
    pInstance = new StandardRegistry();
    pInstance->setupRegistry();
  }
  return pInstance;
}

//---------------------------------------------StandardRegistry
StandardRegistry::StandardRegistry()
{

}

//---------------------------------------------setupRegistry
void StandardRegistry::setupRegistry()
{
/*
  registerModule<ConsoleMonitor>();
  registerModule<MultiDimensionalRandomOutput>();
  registerModule<MidiOutputModule>();
  registerModule<MidiInputModule>();
  registerModule<MidiRouterModule>();
  registerModule<TimerModule>();
  registerModule<AudioInputModule>();
  registerModule<GainModule>();
  registerModule<LuaModule>();
  registerModule<HostModule>();
  //registerModule<AudioBufferInputModule>();
  //registerModule<AudioBufferOutputModule>();
  registerModule<WavReaderModule>();
  registerModule<SampleBufferInputModule>();
  registerModule<SampleBufferOutputModule>();
  //registerModule<SceneGraphInputModule>();
  //registerModule<SceneGraphOutputModule>();
  registerModule<XMLResourceProvider>();
*/
}

} }
