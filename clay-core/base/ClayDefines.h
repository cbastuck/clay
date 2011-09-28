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

#ifndef CLAYDEFINES_H_
#define CLAYDEFINES_H_

//guess the build environment and set MF specific variables
#include <clay-core/arch/Platform.h>

//use the clay allocator
#include <clay-core/base/ClayAllocator.h>

//define literal for NULL-pointers
#ifndef NULL
  #define NULL 0
#endif

#include <math.h>

//use memcpy implementation from here
#include <string.h>

//define assertions
#include <assert.h> //TODO: does this exist on every platform (??)

#if CLAY_PLATFORM_WIN32
  #define CLAY_FAIL() __debugbreak(); //invoke the MSVC debugger
#else
  #define CLAY_FAIL() assert(false);
#endif

#if 1 //treat failed assertions as CLAY_FAIL
  #define CLAY_ASSERT(x) if(!(x)) CLAY_FAIL();
#else
  #define CLAY_ASSERT(x) assert(x);
#endif

//define the used string type
#include <string>
namespace CLAY
{
  typedef std::string tString;
}

#include <iostream>
#define CLAY_ERR_OUT std::cout
#define CLAY_STD_OUT std::cout
#define CLAY_ENDL    std::endl

//clay logger
#include <clay-core/helper/ClayLogger.h>
#define CLAY_LOG_ERROR(x) ClayLogger::getInstance().printError(x);
#define CLAY_LOG_INFO(x) ClayLogger::getInstance().printInfo(x);

//namespace declarations
#define NAMESPACE_CLAY_BEGIN namespace CLAY{
#define NAMESPACE_CLAY_END   }

#define NAMESPACE_CLAY_HELPER_BEGIN namespace CLAY{ namespace HELPER{
#define NAMESPACE_CLAY_HELPER_END } }

#define NAMESPACE_CLAY_MODULE_BEGIN namespace CLAY{ namespace MODULE{
#define NAMESPACE_CLAY_MODULE_END } }

#define NAMESPACE_CLAY_UI_BEGIN namespace CLAY{ namespace UI{ 
#define NAMESPACE_CLAY_UI_END } }

#define NAMESPACE_CLAY_MODEL_BEGIN namespace CLAY{ namespace MODEL{
#define NAMESPACE_CLAY_MODEL_END } }

#define NAMESPACE_CLAY_NUGGET_BEGIN namespace CLAY{ namespace NUGGET{ 
#define NAMESPACE_CLAY_NUGGET_END  } }

#endif
