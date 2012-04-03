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

#ifndef MP3ReaderModule_H_
#define MP3ReaderModule_H_

#include <clay-core/base/ModuleHelper.h>
#include <clay-data-types/audio/SampleBuffer.h>

namespace CLAY{ namespace MODULE{

class MP3ReaderModule : public ModuleHelper<MP3ReaderModule,
                                            HELPER::IntegerEncoding<'M','P','3','R'>::value,
                                            HELPER::IntegerEncoding<'A','U','I','O'>::value >
{
private:
  typedef ModuleHelper<MP3ReaderModule,
                       HELPER::IntegerEncoding<'M','P','3','R'>::value,
                       HELPER::IntegerEncoding<'A','U','I','O'>::value > tBase;

public:
  typedef MODEL::SampleBufferOutput tOutputType;

  MP3ReaderModule(const tString& sRuntimeID);

  virtual void registerModuleOutputs();

  void reset();
  bool loadMP3(const tString& sFn);

private:
  bool loadMP3(const tString& sFn, MODEL::SampleBuffer& aDst);

  bool decode(unsigned char const* start, unsigned long length, MODEL::SampleBuffer& aDst);
};

} }

#endif 
