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

#ifndef SHARED_LIBRARY_IMPL_UNIX_H_
#define SHARED_LIBRARY_IMPL_UNIX_H_

#include <clay-core/arch/Platform.h>

#if !CLAY_PLATFORM_WIN32
  #error "Target specific compile target - compile only on WIN32"
#endif

#include <clay-core/base/ClayDefines.h>
#include <Windows.h>

namespace CLAY
{
  namespace HELPER
  {
  class SharedLibraryImplWIN32
      {
      public:
        SharedLibraryImplWIN32();
        ~SharedLibraryImplWIN32();

        void deInit();

        bool loadLibrary(const tString& sFilename);

        template<class R>
        bool invoke(const tString& sMethodName, R& r)
        {
          typedef R(*tEntryPoint)();
          tEntryPoint aEntryPoint = (tEntryPoint)GetProcAddress(m_aInstance, sMethodName.c_str());
          if(aEntryPoint)
          {
            r = aEntryPoint();
            return true;
          }
          return false;
        }

        template<class R, class T0>
        bool invoke(const tString& sMethodName, R& r, T0 t0)
        {
          typedef R(*tEntryPoint)(T0);
          tEntryPoint aEntryPoint = (tEntryPoint)GetProcAddress(m_aInstance, sMethodName.c_str());
          if(aEntryPoint)
          {
            r = aEntryPoint(t0);
            return true;
          }
          return false;
        }

      private:
        HINSTANCE m_aInstance;
      };

  }
}

#endif
