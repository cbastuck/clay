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

#ifndef ASIODRIVER_H_
#define ASIODRIVER_H_

#include <clay-core/base/ClayDefines.h>

#include "ISampleProvider.h"

#include <common/asio.h>

#include <vector>

class AsioDriverList;

class ASIODriver
{
private:
  typedef std::string tString;

public:
  typedef std::vector<tString> tDeviceList;

  ASIODriver();  
  ~ASIODriver();
  
  unsigned int getNumAsioDevices();
  bool getAsioDeviceEntry(unsigned int uDeviceIdx, tString& sDst);
  void getAsioDeviceList(tDeviceList& aDst);

  bool openAsioDriver(unsigned int uIdx);
  bool startAsioDriver();
  bool stopAsioDriver();
  bool isAsioDriverRunning() const;

  void registerAudioCallback(ISampleProvider* pSampleProvider);

  unsigned int getBufferSize() const;
  void setBufferSize(unsigned int uSize);
  
  unsigned int getNumChannels() const;
  void setNumChannels(unsigned int uNumChannels);

  //---------- static ASIO callbacks
  static void bufferSwitch(long doubleBufferIndex, ASIOBool directProcess);
  static ASIOTime* bufferSwitchTimeInfo(ASIOTime* params, long doubleBufferIndex, ASIOBool directProcess);
  static void onSampleRateChanged(ASIOSampleRate sRate);
  static long onAsioMessage(long selector, long value, void* message, double* opt);
  static int convertSampleFloat2Int(float src);

private:
  enum
  {
    INPUT_LEFT   = 0,
    INPUT_RIGHT  = 1,
    OUTPUT_LEFT  = 2,
    OUTPUT_RIGHT = 3,
    NUM_CHANNELS,
  };

  AsioDriverList*         m_pDeviceList;
  static ASIODriverInfo   m_aDriverInfo;
  static ASIOCallbacks    m_aAsioCallbacks;
  static ASIOBufferInfo   m_aAsioBufferInfos[NUM_CHANNELS];
  static ASIOChannelInfo  m_aAsioDriverChannelInfo;

  static ISampleProvider* m_pSampleProvider;

  static unsigned int     m_uBufferSize;

  bool                    m_bAsioDriverIsRunning;
};

#endif
