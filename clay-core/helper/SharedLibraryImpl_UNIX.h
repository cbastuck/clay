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

#ifndef SHARED_LIBRARY_IMPL_WIN32_H_
#define SHARED_LIBRARY_IMPL_WIN32_H_

#include <clay-core/base/ClayDefines.h>

#if !CLAY_PLATFORM_UNIX
  #error "Target specific compile target - compile only on UNIX"
#endif

#include <dlfcn.h>

namespace CLAY{ namespace HELPER{

class SharedLibraryImplUNIX
{
public:
  SharedLibraryImplUNIX();
  ~SharedLibraryImplUNIX();

  void deInit();
  bool loadLibrary(const tString& sFilename);

  template<class R>
  bool invoke(const tString& sMethodName, R& r);

  template<class R, class T0>
  bool invoke(const tString& sMethodName, R& r, T0 t0);

private:
  void* m_pHandle;
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------
template<class R>
inline bool SharedLibraryImplUNIX::invoke(const tString& sMethodName, R& r)
{
  if(!m_pHandle)
  {
    return false;
  }

  void* pFn = dlsym(m_pHandle, sMethodName.c_str());
  if(!pFn)
  {
    return false;
  }

  char* pError;
  if((pError = dlerror()) != NULL)
  {
     return false;
  }

  typedef R (*tFn)();
  r = ((tFn)(pFn))(); //invoke the method

  return true;
}

//---------------------------------------------
template<class R, class T0>
inline bool SharedLibraryImplUNIX::invoke(const tString& sMethodName, R& r, T0 t0)
{
  if(!m_pHandle)
  {
    return false;
  }

  void* pFn = dlsym(m_pHandle, sMethodName.c_str());
  if(!pFn)
  {
    return false;
  }

  char* pError;
  if((pError = dlerror()) != NULL)
  {
     return false;
  }

  typedef R (*tFn)(T0);
  r = ((tFn)(pFn))(t0); //invoke the method

  return true;
}

} }

#endif
