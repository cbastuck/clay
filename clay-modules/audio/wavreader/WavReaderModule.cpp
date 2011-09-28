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

#include "WavReaderModule.h"

#include <clay-core/helper/FileHelper.h>

#include <limits>

namespace CLAY{ namespace MODULE{

//---------------------------------------------WavReaderModule
WavReaderModule::WavReaderModule(const tString& sRuntimeId)
  : tBase(sRuntimeId)
{
}

//---------------------------------------------~WavReaderModule
WavReaderModule::~WavReaderModule()
{

}

//---------------------------------------------init
bool WavReaderModule::init(XERCES::DOMNode* pNode)
{
  return tBase::init(pNode);
}

//---------------------------------------------save
bool WavReaderModule::save(XERCES::DOMElement* pNode)
{
  return tBase::save(pNode);
}

//---------------------------------------------load
bool WavReaderModule::load(XERCES::DOMElement* pNode, tConnectionMap* pInputConnections, tConnectionMap* pOutputConnections)
{
  return tBase::load(pNode, pInputConnections, pOutputConnections);
}

//---------------------------------------------registerModuleOutputs
void WavReaderModule::registerModuleOutputs()
{
  registerOutput(new tOutputType, "samples");
}
 
//---------------------------------------------registerModuleInputs
void WavReaderModule::registerModuleInputs()
{

}

//---------------------------------------------process
Module::ProcessResultCode WavReaderModule::process()
{
  return Module::Process_OK;
}

//---------------------------------------------loadWAV
bool WavReaderModule::loadWAV(const tString& sFn)
{
  if(getNumOutputs())
  {
    ModuleOutputBase* pOut = tBase::getOutput(0);
    tOutputType* pOutType  = static_cast<tOutputType*>(pOut);
    return loadWAV(sFn, pOutType->getData());
  }
  return false;
}

//---------------------------------------------loadWAV
bool WavReaderModule::loadWAV(const tString& sFn, tSampleBuffer& aSampleBuffer)
{
  char buffer[4];
  std::ifstream is(sFn.c_str(), std::ios::binary);
  if(!is)
  {
    return false;
  }
  is.read(buffer,sizeof(buffer));
  if(strncmp(buffer,"RIFF", sizeof(buffer)))
  {
    return false;
  }
  int iFileLength=HELPER::readLittleEndian<int>(is); //file-length-8
  is.read(buffer,sizeof(buffer));
  if(strncmp(buffer,"WAVE", sizeof(buffer)))
  {
    return false;
  }
  is.read(buffer,sizeof(buffer));
  if(strncmp(buffer,"fmt ", sizeof(buffer)))
  {
    return false; 
  }
  int iSubchunk1Size   = HELPER::readLittleEndian<int>(is);
  short iAudioFormat   = HELPER::readLittleEndian<short>(is);
  short numChannels    = HELPER::readLittleEndian<short>(is);
  int iSampleRate      = HELPER::readLittleEndian<int>(is);
  int iByteRate        = HELPER::readLittleEndian<int>(is);
  short iBlockAlign    = HELPER::readLittleEndian<short>(is);
  short iBitsPerSample = HELPER::readLittleEndian<short>(is);
  is.read(buffer,sizeof(buffer));
  if(strncmp(buffer,"data", sizeof(buffer)))
  {
    return false;
  }
  unsigned int iNumBytes   = HELPER::readLittleEndian<int>(is);
  unsigned int iNumSamples = iNumBytes / sizeof(short);

  MODEL::SampleBuffer::BufferLayout eLayout = (numChannels == 1) ? MODEL::SampleBuffer::eMonoLayout : MODEL::SampleBuffer::eStereoLayout;
  aSampleBuffer.setLayout(eLayout);

  aSampleBuffer.resize(iNumSamples);
  aSampleBuffer.setSampleRate(iSampleRate);
  for(unsigned int i=0; i<iNumSamples; ++i)
  {
    short sample;
    is.read(reinterpret_cast<char*>(&sample), sizeof(short));
    aSampleBuffer.setSample(0, i, (float)sample / (float)std::numeric_limits<short>::max());
  }
  return true;
}

} }
