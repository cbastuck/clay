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

#ifndef SampleBuffer_H_
#define SampleBuffer_H_

#include <clay-data-types/base/Matrix.h>

#include <clay-core/base/ModuleIO.h>

namespace CLAY{ namespace MODEL{

class SampleBuffer : public Matrix<float>
{
public:
  typedef float           tSample;
  typedef Matrix<tSample> tBase;
  typedef tBase::tVector  tChannel;

  enum BufferLayout
  {
    eMonoLayout   = 1,
    eStereoLayout = 2,
  };

  SampleBuffer(BufferLayout eLayout=eMonoLayout);
  ~SampleBuffer();

  void setLayout(BufferLayout eLayout);
  void resize(unsigned int uNumSamples);
  void reserve(unsigned int uNumSamples);

  tChannel getSamples(unsigned int uChannel); //inline 
  const tChannel getSamples(unsigned int uChannel) const; //inline 
  const tSample& getSample(unsigned int uChannel, unsigned int uIdx) const; //inline

  void setSample(unsigned int uChannel, unsigned int uIdx, tSample aSample);

  unsigned int getNumChannels() const;
  unsigned int getNumSamples() const;

  void setSampleRate(unsigned int uSampleRate);
  unsigned int getSampleRate() const;

  BufferLayout getLayout() const;

  bool writeRaw(const tString& sRawFn);

  SampleBuffer createClip(unsigned int uStartSample, unsigned int uNumSamples, unsigned int uNumChannels);
  Const<SampleBuffer> createClip(unsigned int uStartSample, unsigned int uNumSamples, unsigned int uNumChannels) const;

  void createSilence(unsigned int uNumSamples);
  void normalize();

protected:
  unsigned int getNumChannels(BufferLayout eLayout) const;

private:
  BufferLayout m_eBufferLayout;
  unsigned int m_uSampleRate;
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------getSamples
inline SampleBuffer::tChannel SampleBuffer::getSamples(unsigned int uChannel)
{
  return tBase::row(uChannel);
}

//---------------------------------------------getSamples
inline const SampleBuffer::tChannel SampleBuffer::getSamples(unsigned int uChannel) const
{
  return tBase::row(uChannel);
}

//---------------------------------------------getSample
inline const SampleBuffer::tSample& SampleBuffer::getSample(unsigned int uChannel, unsigned int uIdx) const
{
  return tBase::operator()(uChannel, uIdx);
}


//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

class SampleBufferOutput : public ModuleOutput<SampleBuffer>
{
public:
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

class SampleBufferOutputPtr : public TypedModuleOutput<SampleBuffer>
{
  typedef TypedModuleOutput<SampleBuffer> tBase;
public:
  SampleBufferOutputPtr(SampleBuffer* pSampleBuffer)
    : tBase(pSampleBuffer){}
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

class SampleBufferConstOutput : public TypedModuleOutput<const SampleBuffer>
{
  typedef TypedModuleOutput<const SampleBuffer> tBase;
public:
  SampleBufferConstOutput(const SampleBuffer* pData)
    : tBase(pData){}
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

class SampleBufferInput : public ModuleInput<SampleBuffer>
{
public:
};

} }

#endif
