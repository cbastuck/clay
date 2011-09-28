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

#ifndef CLAY_MATRIX_OP_H_
#define CLAY_MATRIX_OP_H_

#include <clay-core/base/ClayDefines.h>
#include <clay-data-types/base/Matrix.h>
#include <clay-data-types/base/Vector.h>

#include <iostream>

namespace CLAY{ namespace MODEL{

template<class T>
inline std::ostream& operator<<(std::ostream& os, const Matrix<T>& aMat)
{
  os << aMat.getNumRows() << " x " << aMat.getNumColumns() << " Matrix: " << std::endl;
  for(unsigned int i=0, n=aMat.getNumRows(); i<n; ++i)
  {
    typename Matrix<T>::tConstVector aVec = aMat.row(i);
    for(unsigned int j=0, m=aVec().size(); j<m; ++j)
    {
      os << aVec()(j) << " ";
    }
    os << std::endl;
  }
  return os;
}

} }

#endif 
