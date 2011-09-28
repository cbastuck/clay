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

#ifndef WavReaderModule_H_
#define WavReaderModule_H_

#include <clay-core/base/ModuleHelper.h>
#include <clay-data-types/audio/SampleBuffer.h>

namespace CLAY{ namespace MODULE{

class WavReaderModule : public ModuleHelper<WavReaderModule,
                                            HELPER::IntegerEncoding<'W','A','V','R'>::value,
                                            HELPER::IntegerEncoding<'A','U','I','O'>::value >
{
public:
  typedef ModuleHelper<WavReaderModule,
                       HELPER::IntegerEncoding<'W','A','V','R'>::value,
                       HELPER::IntegerEncoding<'A','U','I','O'>::value > tBase;

  typedef MODEL::SampleBuffer       tSampleBuffer;
  typedef MODEL::SampleBufferOutput tOutputType;

  WavReaderModule(const tString& sRuntimeId);
  ~WavReaderModule();

  virtual bool init(XERCES::DOMNode* pNode);

  virtual bool save(XERCES::DOMElement* pNode);
  virtual bool load(XERCES::DOMElement* pNode, tConnectionMap* pInputConnections = NULL, tConnectionMap* pOutputConnections = NULL);

  virtual void registerModuleOutputs();
  virtual void registerModuleInputs ();

  virtual ProcessResultCode process();

  bool loadWAV(const tString& sFn);

  bool loadWAV(const tString& sFn, tSampleBuffer& aDst);

private:
  
};

} } 

#endif
