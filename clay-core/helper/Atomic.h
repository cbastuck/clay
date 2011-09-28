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

#ifndef Atomic_H_
#define Atomic_H_

//#include <atomic> //use STL atomic implementation - NOT AVAILABLE 
#include<boost/interprocess/detail/atomic.hpp>

namespace MF
{
  class Atomic
  {
  public:
    unsigned int atomicDecrement(volatile unsigned int* pUint)
    {
      return boost::interprocess::detail::atomic_dec32(pUint);
    }

    unsigned int atomicIncrement(volatile unsigned int* pUint)
    {
      return boost::interprocess::detail::atomic_inc32(pUint);
    }

    /*
     * Compare an "pUint" value with "cmp".
     * If they are the same swap the value with "uSwapVal"
     * Returns the old value of "pUint"
     */
    unsigned int atomicCompareAndSwap(volatile unsigned int* pUint, unsigned int uSwapVal, unsigned int uCmpVal)
    {
      return boost::interprocess::detail::atomic_cas32(pUint, uSwapVal, uCmpVal);
    }

  };
}

#endif
