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

#include "SampleBuffer.h"

#include <clay-data-types/base/Vector.h>

//STL
#include <fstream>

namespace CLAY{ namespace MODEL{

//---------------------------------------------SampleBuffer
SampleBuffer::SampleBuffer(BufferLayout eLayout)
  : tBase(0, 0, tBase::ROW_ALIGNMENT),
    m_eBufferLayout(eLayout),
    m_uSampleRate(0)
{

}
    
//---------------------------------------------~SampleBuffer
SampleBuffer::~SampleBuffer()
{

}

/*
//---------------------------------------------init
void SampleBuffer::init(BufferLayout eBufferLayout)
{
  m_eBufferLayout = eBufferLayout;
  
  tBase::resize(getNumChannels(), 0);
}
*/

/*
//---------------------------------------------init
void SampleBuffer::init(const SampleBuffer& aOther, unsigned int uFromSample, unsigned int uNumSamples)
{
  deInit();

  m_eBufferLayout = aOther.m_eBufferLayout;
  m_uSampleRate   = aOther.m_uSampleRate;

  attach(aOther.getMemoryPtr() +  uFromSample * sizeof(tSampleType), uNumSamples * sizeof(tSampleType));
}
*/

/*
//---------------------------------------------deInit
void SampleBuffer::deInit()
{
  m_eBufferLayout = eInvalidLayout;
  m_uSampleRate = 0;
}
*/

/*
//---------------------------------------------reserve
void SampleBuffer::reserve(unsigned int uNumSamples)
{
  tBase::reserve(getNumChannels(), uNumSamples);
}
*/

//---------------------------------------------setLayout
void SampleBuffer::setLayout(BufferLayout eLayout)
{
  if(eLayout == m_eBufferLayout)
  {
    return; //nothing to do
  }

  if(m_eBufferLayout == eStereoLayout && 
     eLayout == eMonoLayout) //stereo to mono
  {
    setNumRows(1);
  }
  else if(m_eBufferLayout == eMonoLayout && 
          eLayout == eStereoLayout) //from mono to stereo
  {
    tBase::resize(2, getNumColumns());
    
    tBase::row(1) = tBase::row(0); //copy left to right channel
  }
  m_eBufferLayout = eLayout;
}

//---------------------------------------------resize
void SampleBuffer::resize(unsigned int uNumSamples)
{
  tBase::resize(getNumChannels(), uNumSamples);
}

//---------------------------------------------reserve
void SampleBuffer::reserve(unsigned int uNumSamples)
{
  tBase::reserve(getNumChannels(), uNumSamples);
}

//---------------------------------------------operator[]
const SampleBuffer::tChannel SampleBuffer::getSamples(unsigned int uChannel)
{
  return tBase::row(uChannel);
}

//---------------------------------------------getSample
const SampleBuffer::tSample& SampleBuffer::getSample(unsigned int uChannel, unsigned int uIdx) const
{
  return tBase::operator()(uChannel, uIdx);
}

//---------------------------------------------setSample
void SampleBuffer::setSample(unsigned int uChannel, unsigned int uIdx, tSample aSample)
{
  tBase::operator()(uChannel, uIdx) = aSample;
}

//---------------------------------------------getNumChannels
unsigned int SampleBuffer::getNumChannels() const
{
  return getNumChannels(m_eBufferLayout);
}

//---------------------------------------------getNumSamples
unsigned int SampleBuffer::getNumSamples() const
{
  return tBase::getNumColumns();
}

//---------------------------------------------setSampleRate
void SampleBuffer::setSampleRate(unsigned int uSampleRate)
{
  m_uSampleRate = uSampleRate;
}

//---------------------------------------------getSampleRate
unsigned int SampleBuffer::getSampleRate() const
{
  return m_uSampleRate;
}

//---------------------------------------------getLayout
SampleBuffer::BufferLayout SampleBuffer::getLayout() const
{
  return m_eBufferLayout;
}

/*
//---------------------------------------------writeRaw
bool SampleBuffer::writeRaw(const tString& sRawFn)
{
  std::ofstream os(sRawFn.c_str(), std::ios::binary | std::ios::out);
  if(os)
  {
    for(unsigned int i=0, n=getNumChannels(); i<n; ++i)
    {
      tBase::writeRowRaw(i, os);
    }
    return true;
  }
  return false;
}
*/

//---------------------------------------------createClip
SampleBuffer SampleBuffer::createClip(unsigned int uStartSample, unsigned int uNumSamples, unsigned int uNumChannels)
{
  CLAY_ASSERT(uNumChannels > 0 && uNumChannels < 3);
  CLAY_ASSERT(uNumChannels <= getNumChannels());

  SampleBuffer aRes(m_eBufferLayout);
  tBase& aSubmat = aRes;
  aSubmat.attach(tBase::submatrix(0, uNumChannels, uStartSample, uNumSamples));

  aRes.m_uSampleRate = m_uSampleRate;
  aRes.m_eBufferLayout = (uNumChannels == 1) ? eMonoLayout : eStereoLayout;

  return aRes;
}

//---------------------------------------------getNumChannels
unsigned int SampleBuffer::getNumChannels(BufferLayout eLayout) const
{
  return (eLayout == eMonoLayout) ? 1 : 2;
}

} } 