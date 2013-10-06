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

//CLAY
#include <cstddef>
#include <clay-core/arch/Platform.h>

//STL
#include <new>

#ifndef CLAY_ALLOCATOR_H_
#define CLAY_ALLOCATOR_H_

class CLAY_DLL_EXPORT ClayAllocator
{
public:
  static void* clayNew(size_t size);
  static void  clayDelete(void* p);

  static void* clayArrayNew(size_t size);
  static void  clayArrayDelete(void *p);
};

//---------------------------------------------
inline void* operator new (size_t size)
{
  return ClayAllocator::clayNew(size);
}

//---------------------------------------------
inline void operator delete (void* p)
{
  ClayAllocator::clayDelete(p);
}

//---------------------------------------------
inline void *operator new[](size_t size)
{
  return ClayAllocator::clayArrayNew(size);
}

//---------------------------------------------
inline void operator delete[](void *p)
{
  ClayAllocator::clayArrayDelete(p);
}

#endif
