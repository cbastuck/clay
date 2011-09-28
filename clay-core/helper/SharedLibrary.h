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

#ifndef SharedLibrary_h_
#define SharedLibrary_h_

#include <clay-core/base/ClayDefines.h>

#if CLAY_PLATFORM_WIN32
  #include "SharedLibraryImpl_WIN32.h"
  #define SHARED_LIBRARY_IMPL SharedLibraryImplWIN32
#elif CLAY_PLATFORM_UNIX
  #include "SharedLibraryImpl_UNIX.h"
  #define SHARED_LIBRARY_IMPL SharedLibraryImplUNIX
#else
  #error "Platform not supported"
#endif

namespace CLAY{ namespace HELPER{

class SharedLibrary
{
public:
  SharedLibrary();
  ~SharedLibrary();

  void deInit();

  bool loadLibrary(const tString& sFilename);

  template<class R>
  bool invoke(const tString& sMethodName, R& r)
  {
    return m_aImpl.invoke<R>(sMethodName, r);
  }

  template<class R, class T0>
  bool invoke(const tString& sMethodName, R& r, T0 t0)
  {
    return m_aImpl.invoke<R, T0>(r, t0);
  }

  const tString& getFilename() const{ return m_sFilename; }

private:
  tString m_sFilename;
  SHARED_LIBRARY_IMPL m_aImpl;
};

} }

#endif
