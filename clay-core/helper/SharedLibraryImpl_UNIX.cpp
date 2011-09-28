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

#include "SharedLibraryImpl_UNIX.h"

namespace CLAY{ namespace HELPER{

//---------------------------------------------
SharedLibraryImplUNIX::SharedLibraryImplUNIX()
  : m_pHandle(NULL)
{

}

//---------------------------------------------
SharedLibraryImplUNIX::~SharedLibraryImplUNIX()
{

}

//---------------------------------------------
void SharedLibraryImplUNIX::deInit()
{
  if(m_pHandle)
  {
    dlclose(m_pHandle);
  }
}

//---------------------------------------------
bool SharedLibraryImplUNIX::loadLibrary(const tString& sFilename)
{
  m_pHandle = dlopen(sFilename.c_str(), RTLD_LAZY); //RTLD_NOW

  if(!m_pHandle)
  {
    char* pError = dlerror();
    CLAY_LOG_ERROR(pError);
  }

  return m_pHandle;
}

} }
