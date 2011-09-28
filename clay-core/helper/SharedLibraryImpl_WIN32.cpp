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

#include "SharedLibraryImpl_WIN32.h"

namespace CLAY
{
  namespace HELPER
  {
    //---------------------------------------------SharedLibraryImplWIN32
    SharedLibraryImplWIN32::SharedLibraryImplWIN32()
      : m_aInstance(NULL)
    {

    }

    //---------------------------------------------~SharedLibrary
    SharedLibraryImplWIN32::~SharedLibraryImplWIN32()
    {

    }

    //---------------------------------------------
    void SharedLibraryImplWIN32::deInit()
    {
      if(m_aInstance)
      {
        FreeLibrary(m_aInstance);
      }
    }

    //---------------------------------------------load
    bool SharedLibraryImplWIN32::loadLibrary(const tString& sFilename)
    {
      m_aInstance = LoadLibraryA(sFilename.c_str());
      if(!m_aInstance)
        return false;
      return true;
    }
  }
}
