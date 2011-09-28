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

#ifndef PLATFORM_H_
#define PLATFORM_H_

//--------------------- try to detect the build environment ----------------------
#if defined(WIN32) || defined(_WIN32)
  #define CLAY_PLATFORM_WIN32 1
#endif

#if defined(UNIX) || defined(__unix__)
  #define CLAY_PLATFORM_UNIX 1
#endif

#if defined(__APPLE__)
  #define CLAY_PLATFORM_APPLE 1
#endif

#if defined(DEBUG) || defined(_DEBUG)
  #define CLAY_DEBUG(x) x
#else
  #define CLAY_DEBUG(x) 
#endif 

//----------------------- do some platform-specific stuff  -----------------------

#if CLAY_PLATFORM_WIN32
  //include the standard windows header
  #include <windows.h>

  //disarm the w32 min/max macros
  #ifdef max
    #undef max
  #endif
  #ifdef min
    #undef min
  #endif

  //add a macro to export methods to in a DLL (windows only)
  //#define CLAY_DLL_EXPORT extern "C" __declspec(dllexport)
  #define CLAY_DLL_EXPORT __declspec(dllexport)
#else
  //TODO: additional platform specific checks
  #define CLAY_DLL_EXPORT

#endif

//include some platform specific define helpers
#include "PlatformSpecImpl.h"

#endif
