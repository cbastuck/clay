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

#ifndef CLAY_VECTOR_OP_H_
#define CLAY_VECTOR_OP_H_

#include <clay-core/base/ClayDefines.h>
#include <clay-data-types/base/Vector.h>

//STL
#include <iostream>

namespace CLAY{ namespace MODEL{

//---------------------------------------------operator<<
template<class T>
inline std::ostream& operator<<(std::ostream& os, const Vector<T>& aVec)
{
  os << "Vector [" << aVec.size() << "]" << std::endl;
  for(unsigned int i=0, n=aVec.size(); i<n; ++i)
  {
    os << aVec(i) << " ";
  }
  os << std::endl;
}

//---------------------------------------------min
template<class T>
inline T min(const Vector<T>& aVec)
{
  T aMin = std::numeric_limits<T>::max();
  for(unsigned int i=0, n=aVec.size(); i<n; ++i)
  {
    const T& aVal = aVec(i);
    aMin = (aVal < aMin) ? aVal : aMin;
  }
  return aMin;
}

//---------------------------------------------max
template<class T>
inline T max(const Vector<T>& aVec)
{
  CLAY_ASSERT(aVec.size());

  T aMax = aVec(0);
  for(unsigned int i=0, n=aVec.size(); i<n; ++i)
  {
    const T& aVal = aVec(i);
    aMax = (aVal > aMax) ? aVal : aMax;
  }
  return aMax;
}

//---------------------------------------------sum
template<class T>
inline T sum(const Vector<T>& aVec)
{
  T aSum = 0;
  for(unsigned int i=0, n=aVec.size(); i<n; ++i)
  {
    aSum += aVec(i);
  }
  return aSum;
}

//---------------------------------------------mean
template<class T>
inline T mean(const Vector<T>& aVec)
{
  return sum(aVec) / (T) aVec.size();
}

} }

#endif
