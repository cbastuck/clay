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

#include "ClayRuntime.h"
#include "ClayExecutable.h"

namespace CLAY{

//---------------------------------------------ClayRuntime
ClayRuntime::ClayRuntime()
{
}

//---------------------------------------------~ClayRuntime
ClayRuntime::~ClayRuntime()
{
}

//---------------------------------------------execute
Module::ProcessResultCode ClayRuntime::execute(ClayExecutable& aProgram)
{
  Module::ProcessResultCode aResult = Module::Process_OK;

  ClayExecutable::iterator it  = aProgram.begin();
  ClayExecutable::iterator end = aProgram.end();
  for(; (it!=end) && (aResult==Module::Process_OK); ++it)
  {
    aResult = it->execute();
    CLAY_ASSERT(aResult != Module::Process_ERROR);
  }

  return aResult;
}

}