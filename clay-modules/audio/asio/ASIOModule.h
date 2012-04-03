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

#ifndef ASIOMODULE_H_
#define ASIOMODULE_H_

//Clay
#include <clay-core/base/ClayDefines.h>
#include <clay-core/base/ModuleHelper.h>
#include <clay-data-types/audio/AudioInfo.h>
#include <clay-data-types/audio/SampleBuffer.h>

#include "ISampleProvider.h"

//STL
#include <vector>

class ASIODriver;

namespace CLAY{ namespace MODULE{

class ASIOModule : public ModuleHelper<ASIOModule,
                                       HELPER::IntegerEncoding<'A','S','I','O'>::value,
                                       HELPER::IntegerEncoding<'A','D','R','V'>::value>,
                   public ISampleProvider
{
private:
  typedef ModuleHelper<ASIOModule,
                       HELPER::IntegerEncoding<'A','S','I','O'>::value,
                       HELPER::IntegerEncoding<'A','D','R','V'>::value> tBase;

  typedef MODEL::SampleBufferInput   tSampleInput;
  typedef ModuleInput<unsigned int>  tBufferSizeInput;
  typedef ModuleOutput<unsigned int> tProcessSizeOutput;

public:
  typedef std::vector<tString> tDeviceList;

  ASIOModule(const tString& sId);
  virtual ~ASIOModule();

  virtual bool init(XERCES::DOMNode* pNode);
  CLAY_DLL_EXPORT bool init(unsigned int uBufferSize, ISampleProvider& aProvider);
  virtual void deInit();

  virtual bool save(XERCES::DOMElement* pNode);
  virtual bool load(XERCES::DOMElement* pNode, tConnectionMap* pInputConnections = NULL, tConnectionMap* pOutputConnections = NULL);

  virtual void registerModuleOutputs();
  virtual void registerModuleInputs ();

  virtual ProcessResultCode process();
  
  CLAY_DLL_EXPORT unsigned int getNumAsioDevices() const;
  CLAY_DLL_EXPORT void getAsioDeviceList(tDeviceList& collDst) const;
  CLAY_DLL_EXPORT bool getAsioDeviceName(unsigned int uDeviceIdx, tString& sDst) const;
  CLAY_DLL_EXPORT bool getOpenAsioDeviceName(tString& sDst) const;

  CLAY_DLL_EXPORT bool openAsioDevice(unsigned int uDeviceIdx);
  CLAY_DLL_EXPORT bool openAsioDevice(const tString& sDeviceName);
  CLAY_DLL_EXPORT bool closeAsioDevice();

  CLAY_DLL_EXPORT MODEL::SampleBuffer* getSampleBuffer();
  
protected:
  void onBufferSizeChanged();

private:
  ASIODriver*   m_pAsioDriver;
  unsigned int  m_uOpenDeviceIdx;
};

} }

#endif
