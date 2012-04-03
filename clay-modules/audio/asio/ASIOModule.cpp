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

#include "ASIOModule.h"
#include "ASIODriver.h"

//STL
#include <limits>

//boost
#include <boost/bind.hpp>

namespace CLAY{ namespace MODULE{

//---------------------------------------------ASIOModule
ASIOModule::ASIOModule(const tString& sId)
  : tBase(sId),
    m_pAsioDriver(NULL),
    m_uOpenDeviceIdx(std::numeric_limits<unsigned int>::max())
{

}

//---------------------------------------------~ASIOModule
ASIOModule::~ASIOModule()
{

}

//---------------------------------------------init
bool ASIOModule::init(unsigned int uBufferSize, ISampleProvider& aProvider)
{
  if(tBase::init(NULL))
  {
    CLAY_ASSERT(!m_pAsioDriver);
    m_pAsioDriver = new ASIODriver();
    m_pAsioDriver->registerAudioCallback(&aProvider);
    m_pAsioDriver->setBufferSize(uBufferSize);
    return true;
  }
  return false;
}

//---------------------------------------------init
bool ASIOModule::init(XERCES::DOMNode* pNode)
{
  if(tBase::init(pNode))
  {
    CLAY_ASSERT(!m_pAsioDriver);
    m_pAsioDriver = new ASIODriver();
    m_pAsioDriver->registerAudioCallback(this);
    return true;
  }
  return false;
}

//---------------------------------------------deInit
void ASIOModule::deInit()
{
  CLAY_ASSERT(m_pAsioDriver);
  for(unsigned int i=0; i<10 && m_pAsioDriver->isAsioDriverRunning(); ++i) //try max n times to stop the ASIO driver
  {
    m_pAsioDriver->stopAsioDriver();
  }
  delete m_pAsioDriver;
  m_pAsioDriver = NULL;
  
  tBase::deInit();
}

//---------------------------------------------save
bool ASIOModule::save(XERCES::DOMElement* pParent)
{
  XERCES::DOMElement* pNode = XercesXML::appendNode(pParent, "asio-module");
  tString sOpenDevice;
  getOpenAsioDeviceName(sOpenDevice);
  XercesXML::addAttribute(pNode, "open-device", sOpenDevice);

  XERCES::DOMElement* pBase = XercesXML::appendNode(pNode, "base");
  return tBase::save(pBase);
}

//---------------------------------------------load
bool ASIOModule::load(XERCES::DOMElement* pParent, tConnectionMap* pInputConnections, tConnectionMap* pOutputConnections)
{
  XERCES::DOMElement* pNode = XercesXML::findChildNode(pParent, "asio-module");
  if(!pNode)
  {
    return false;
  }

  tString sOpenDevice;
  if(!XercesXML::getAttributeValue(pNode, "open-device", sOpenDevice))
  {
    return false;
  }

  if(!sOpenDevice.empty())
  {
    openAsioDevice(sOpenDevice);
  }

  XERCES::DOMElement* pBase = XercesXML::findChildNode(pNode, "base");
  if(!pBase)
  {
    return false;
  }

  return tBase::load(pBase, pInputConnections, pOutputConnections);
}

//---------------------------------------------registerModuleOutputs
void ASIOModule::registerModuleOutputs()
{
  registerOutput(new tProcessSizeOutput(0u), "num-samples");
}

//---------------------------------------------registerModuleInputs
void ASIOModule::registerModuleInputs()
{
  registerInput(new tSampleInput, "samples");

  tBufferSizeInput* pBufferSizeIn = new tBufferSizeInput();
  pBufferSizeIn->registerInputChangeCallback(boost::bind(&ASIOModule::onBufferSizeChanged, this));
  registerInput(pBufferSizeIn, "blocklen");
}

//---------------------------------------------process
Module::ProcessResultCode ASIOModule::process()
{
  if(!m_pAsioDriver->isAsioDriverRunning())
  {
    if(!m_pAsioDriver->startAsioDriver())
    {
      return Module::Process_ERROR;
    }
  }
  return Module::Process_OK;
}

//---------------------------------------------getNumAsioDevices
unsigned int ASIOModule::getNumAsioDevices() const
{
  CLAY_ASSERT(m_pAsioDriver);
  return m_pAsioDriver->getNumAsioDevices();
}

//---------------------------------------------getAsioDeviceList
void ASIOModule::getAsioDeviceList(tDeviceList& collDst) const
{
  CLAY_ASSERT(m_pAsioDriver);
  m_pAsioDriver->getAsioDeviceList(collDst);
}

//---------------------------------------------getAsioDeviceName
bool ASIOModule::getAsioDeviceName(unsigned int uDeviceIdx, tString& sDst) const
{
  if(uDeviceIdx < getNumAsioDevices())
  {
    CLAY_ASSERT(m_pAsioDriver);
    return m_pAsioDriver->getAsioDeviceEntry(uDeviceIdx, sDst);
  }
  return false;
}

//---------------------------------------------getOpenAsioDeviceName
bool ASIOModule::getOpenAsioDeviceName(tString& sDst) const
{
  if(m_uOpenDeviceIdx != std::numeric_limits<unsigned int>::max())
  {
    return getAsioDeviceName(m_uOpenDeviceIdx, sDst);
  }
  return false;
}

//---------------------------------------------openAsioDevice
bool ASIOModule::openAsioDevice(unsigned uDeviceIdx)
{
  CLAY_ASSERT(m_pAsioDriver);
  if(m_pAsioDriver->openAsioDriver(uDeviceIdx))
  {
    m_uOpenDeviceIdx = uDeviceIdx;
    return true;
  }
  return false;
}

//---------------------------------------------openAsioDevice
bool ASIOModule::openAsioDevice(const tString& sDeviceName)
{
  for(unsigned int i=0, n=getNumAsioDevices(); i<n; ++i)
  {
    tString sCurrent;
    if(getAsioDeviceName(i, sCurrent))
    {
      if(sCurrent == sDeviceName)
      {
        return openAsioDevice(i);
      }
    }
  }

  return false;
}

//---------------------------------------------closeAsioDevice
bool ASIOModule::closeAsioDevice()
{
  CLAY_ASSERT(m_pAsioDriver);
  if(m_pAsioDriver->stopAsioDriver())
  {
    m_uOpenDeviceIdx = std::numeric_limits<unsigned int>::max();
    return true;
  }
  return false;
}

//---------------------------------------------getSampleBuffer
MODEL::SampleBuffer* ASIOModule::getSampleBuffer()
{
  tSampleInput* pInput = static_cast<tSampleInput*>(getInput("samples"));
  if(pInput->isConnected())
  {
    //the output indicated how many samples are processed the next call
    tProcessSizeOutput* pOutput = static_cast<tProcessSizeOutput*>(getOutput("num-samples"));
    pOutput->setData(m_pAsioDriver->getBufferSize()); 

    Module* pConnectedModule = pInput->getSourceModule();
    pConnectedModule->process();

    //the output is only valid during processing of the preceding module
    pOutput->setData(0);

    MODEL::SampleBuffer* pSamples = pInput->getData();
    if(pSamples->getNumSamples() >= m_pAsioDriver->getBufferSize())
    {
      return pSamples;
    }
  }
  return NULL; //no buffer available
}

//---------------------------------------------onBufferSizeChanged
void ASIOModule::onBufferSizeChanged()
{
  tBufferSizeInput* pIn = static_cast<tBufferSizeInput*>(getInput("blocklen"));
  unsigned int uBufferSize = *pIn->getData();
  m_pAsioDriver->setBufferSize(uBufferSize);
}

} }
