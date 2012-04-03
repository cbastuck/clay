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

#include "ASIODriver.h"

#include <clay-core/base/ClayDefines.h>
#include <clay-data-types/audio/SampleBuffer.h>

#include <Windows.h>
#include <iostream>

#include <common/iasiodrv.h>
#include <host/pc/asiolist.h>

#include <math.h>

#define UNKNOWN_DEVICE_NAME  "unknown-device"
#define DEFAULT_BUFFER_SIZE  512

extern IASIO* theAsioDriver;

//-----------------------------------------static members
ASIODriverInfo   ASIODriver::m_aDriverInfo;
ASIOCallbacks    ASIODriver::m_aAsioCallbacks;
ASIOBufferInfo   ASIODriver::m_aAsioBufferInfos[NUM_CHANNELS];
ASIOChannelInfo  ASIODriver::m_aAsioDriverChannelInfo;

ISampleProvider* ASIODriver::m_pSampleProvider = NULL;

unsigned int     ASIODriver::m_uBufferSize  = DEFAULT_BUFFER_SIZE;

//-----------------------------------------ASIODriver
ASIODriver::ASIODriver()
:  m_pDeviceList(NULL),
   m_bAsioDriverIsRunning(false)
{
  m_pDeviceList = new AsioDriverList();
}

//-----------------------------------------~ASIODriver
ASIODriver::~ASIODriver()
{
  delete m_pDeviceList;
  m_pDeviceList = NULL;
}

//-----------------------------------------getNumAsioDevices
unsigned int ASIODriver::getNumAsioDevices()
{
  return m_pDeviceList->asioGetNumDev();
}

//-----------------------------------------getAsioDeviceEntry
bool ASIODriver::getAsioDeviceEntry(unsigned int uDeviceIdx, tString& sDst)
{
  char pDrvName[255] = {'\0'};
  unsigned int uRes = m_pDeviceList->asioGetDriverName(uDeviceIdx, pDrvName, sizeof(pDrvName));	
  if(uRes == 0)
  {
    sDst = pDrvName;
    return true;
  }
  return false;
}

//-----------------------------------------getAsioDeviceList
void ASIODriver::getAsioDeviceList(tDeviceList& aDst)
{
  unsigned int uNumDevs = getNumAsioDevices();
  aDst.reserve(uNumDevs);
  for(unsigned int i=0; i<uNumDevs; ++i)
  {
    char pDrvName[255] = {'\0'};
    unsigned int uRes = m_pDeviceList->asioGetDriverName(i, pDrvName, sizeof(pDrvName));	
    if(uRes == 0)
    {
      aDst.push_back(pDrvName);
    }
    else
    {
      aDst.push_back(UNKNOWN_DEVICE_NAME);
    }
  }
}

//-----------------------------------------openAsioDriver
bool ASIODriver::openAsioDriver(unsigned int uIdx)
{
  CLAY_ASSERT(uIdx < getNumAsioDevices());

  if(m_pDeviceList->asioOpenDriver(uIdx, (void **)&theAsioDriver) != 0)
  {
    CLAY_FAIL();
    return false;
  };

  ASIOError aError = ASIOInit(&m_aDriverInfo);
  if(aError != ASE_OK)
  {
    CLAY_FAIL();
    return false;
  }

  //initialize the callbacks
  m_aAsioCallbacks.bufferSwitch         = &bufferSwitch;
  m_aAsioCallbacks.sampleRateDidChange  = &onSampleRateChanged;
  m_aAsioCallbacks.asioMessage          = &onAsioMessage;
  m_aAsioCallbacks.bufferSwitchTimeInfo = &bufferSwitchTimeInfo;

  //initialize the input buffers
  m_aAsioBufferInfos[INPUT_LEFT].isInput    = ASIOTrue;
  m_aAsioBufferInfos[INPUT_LEFT].channelNum = 0;
  m_aAsioBufferInfos[INPUT_LEFT].buffers[0] = NULL;
  m_aAsioBufferInfos[INPUT_LEFT].buffers[1] = NULL;

  m_aAsioBufferInfos[INPUT_RIGHT].isInput    = ASIOTrue;
  m_aAsioBufferInfos[INPUT_RIGHT].channelNum = 1;
  m_aAsioBufferInfos[INPUT_RIGHT].buffers[0] = NULL;
  m_aAsioBufferInfos[INPUT_RIGHT].buffers[1] = NULL;

  //initialize the output buffers
  m_aAsioBufferInfos[OUTPUT_LEFT].isInput    = ASIOFalse;
  m_aAsioBufferInfos[OUTPUT_LEFT].channelNum = 0;
  m_aAsioBufferInfos[OUTPUT_LEFT].buffers[0] = NULL;
  m_aAsioBufferInfos[OUTPUT_LEFT].buffers[1] = NULL;

  m_aAsioBufferInfos[OUTPUT_RIGHT].isInput    = ASIOFalse;
  m_aAsioBufferInfos[OUTPUT_RIGHT].channelNum = 1;
  m_aAsioBufferInfos[OUTPUT_RIGHT].buffers[0] = NULL;
  m_aAsioBufferInfos[OUTPUT_RIGHT].buffers[1] = NULL;

  aError = ASIOCreateBuffers(m_aAsioBufferInfos, NUM_CHANNELS, getBufferSize(), &m_aAsioCallbacks);
  if(aError != ASE_OK)
  {
    CLAY_FAIL();
    return false;
  }

  m_aAsioDriverChannelInfo.channel = 0;
  m_aAsioDriverChannelInfo.isInput = false;
  aError = ASIOGetChannelInfo(&m_aAsioDriverChannelInfo);
  if(aError != ASE_OK)
  {
    CLAY_FAIL();
    return false;
  }

  return true;
}

//-----------------------------------------startAsioDriver
bool ASIODriver::startAsioDriver()
{
  //std::cout<<"Starting the ASIO driver"<<std::endl;
  ASIOError aError = ASIOStart();
  if(aError == ASE_OK)
  {
    m_bAsioDriverIsRunning = true;
    return true;
  }
  else if(aError == ASE_NotPresent)
  {
    std::cout<<"Error: Hardware input or output is not present or available"<<std::endl;
  }
  else
  {
    std::cout<<"Error: Unknown error code: "<<aError<<std::endl;
  }
  return false;
}

//-----------------------------------------stopAsioDriver
bool ASIODriver::stopAsioDriver()
{
  ASIOError aError = ASIOStop();
  if(aError = ASE_OK)
  {
    m_bAsioDriverIsRunning = false;
    return true;
  }
  return false;
}

//-----------------------------------------isAsioDriverRunning
bool ASIODriver::isAsioDriverRunning() const
{
  return m_bAsioDriverIsRunning;
}

//-----------------------------------------registerAudioCallback
void ASIODriver::registerAudioCallback(ISampleProvider* pSampleProvider)
{
  CLAY_ASSERT(!m_pSampleProvider);
  m_pSampleProvider= pSampleProvider;
}

//-----------------------------------------getBufferSize
unsigned int ASIODriver::getBufferSize() const
{
  return m_uBufferSize;
}

//-----------------------------------------setBufferSize
void ASIODriver::setBufferSize(unsigned int uSize)
{
  CLAY_ASSERT(!m_bAsioDriverIsRunning); //do not call this method while driver is running
  m_uBufferSize = uSize;
}

//-----------------------------------------convertSampleFloat2Int
int ASIODriver::convertSampleFloat2Int(float src)
{
  return static_cast<int>(src * 2147483392);
}

//-----------------------------------------bufferSwitch
void ASIODriver::bufferSwitch(long doubleBufferIndex, ASIOBool directProcess)
{
  ASIOTime timeInfo;
  memset(&timeInfo, 0, sizeof (timeInfo));

  // get the time stamp of the buffer, not necessary if no
  // synchronization to other media is required
  if(ASIOGetSamplePosition(&timeInfo.timeInfo.samplePosition, &timeInfo.timeInfo.systemTime) == ASE_OK)
  {
    timeInfo.timeInfo.flags = kSystemTimeValid | kSamplePositionValid;
  }

  bufferSwitchTimeInfo(&timeInfo, doubleBufferIndex, directProcess);
}

//-----------------------------------------bufferSwitchTimeInfo
ASIOTime* ASIODriver::bufferSwitchTimeInfo(ASIOTime* pTimeInfo, long doubleBufferIndex, ASIOBool directProcess)
{
  if(m_pSampleProvider)
  {
    void** pLeftOutBuffers  = m_aAsioBufferInfos[OUTPUT_LEFT].buffers;
    void** pRightOutBuffers = m_aAsioBufferInfos[OUTPUT_RIGHT].buffers;
  
    if(m_aAsioDriverChannelInfo.type == ASIOSTInt32LSB)
    {
      int* pLeftOutBuffer  = static_cast<int*>(pLeftOutBuffers[doubleBufferIndex]);
      int* pRightOutBuffer = static_cast<int*>(pRightOutBuffers[doubleBufferIndex]);

      CLAY::MODEL::SampleBuffer* pSampleBuffer = m_pSampleProvider->getSampleBuffer();
      if(pSampleBuffer)
      {
        CLAY_ASSERT(pSampleBuffer->getNumSamples() >= m_uBufferSize);
        unsigned int uIncomingChannels = pSampleBuffer->getNumChannels();
        switch(uIncomingChannels)
        {
          case 1:
          for(unsigned int i=0; i<m_uBufferSize; ++i)
          {
            pLeftOutBuffer[i]  = convertSampleFloat2Int(pSampleBuffer->getSample(0, i));
            pRightOutBuffer[i] = pLeftOutBuffer[i];
          }
          break;
          
          case 2:
          for(unsigned int i=0; i<m_uBufferSize; ++i)
          {
            pLeftOutBuffer[i]  = convertSampleFloat2Int(pSampleBuffer->getSample(0, i));
            pRightOutBuffer[i] = convertSampleFloat2Int(pSampleBuffer->getSample(1, i));
          }
          break;
        }
      }
      else //no sample buffer available - produce silence
      {
        for(unsigned int i=0; i<m_uBufferSize; ++i)
        {
          pLeftOutBuffer[i]  = 0;
          pRightOutBuffer[i] = 0;
        }
      }

/*
      for(unsigned int i=0; i<m_uBufferSize; ++i)
      {
        static unsigned int time = 0;
        pLeftOutBuffer[i]  = convertSampleFloat2Int(sin((2.f*3.14159f)*(440.f/44100.f)*time++));
        pRightOutBuffer[i] = pLeftOutBuffer[i];
      }
*/
    }
  
  }

  return pTimeInfo;
}

//-----------------------------------------onSampleRateChanged
void ASIODriver::onSampleRateChanged(ASIOSampleRate sRate)
{

}

//-----------------------------------------onAsioMessage
long ASIODriver::onAsioMessage(long selector, long value, void* message, double* opt)
{
  return 0;
}

