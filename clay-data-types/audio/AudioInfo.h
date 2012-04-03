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

#ifndef AUDIOINFO_H_
#define AUDIOINFO_H_

#include <clay-core/base/ClayDefines.h>

NAMESPACE_CLAY_MODEL_BEGIN

class AudioInfo
{
public:
  AudioInfo() 
    : m_uNumChannels(2),
      m_uSampleRate(44100),
      m_uBlockSize(512)
  {

  }

  void setNumChannels(unsigned int uNumChannels){ m_uNumChannels = uNumChannels; }

  void setBlockSize(unsigned int uBlockSize){ m_uBlockSize = uBlockSize; }

  void setSampleRate(unsigned int uSampleRate){ m_uSampleRate = uSampleRate; }

  unsigned int getNumChannels() const{ return m_uNumChannels; }
  
  unsigned int getBlockSize() const{ return m_uBlockSize; }

  unsigned int getSampleRate() const{ return m_uSampleRate; }

private:
  unsigned int m_uNumChannels;
  unsigned int m_uBlockSize;
  unsigned int m_uSampleRate;
};

class AudioInfoOutput : public ModuleOutput<AudioInfo>
{
public:
};


class AudioInfoInput : public ModuleInput<AudioInfo>
{
public:
};

NAMESPACE_CLAY_MODEL_END

#endif

