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

#ifndef MATRIX_MODDEL_GOBAL_OPERATORS_H_
#define MATRIX_MODDEL_GOBAL_OPERATORS_H_

#include "MatrixModel.h"

#include <iostream>

namespace CLAY{ namespace MODEL{

template<class tElementType>
std::ostream& operator<<(std::ostream& os, const MatrixModel<tElementType>& aRhs)
{
  os << "-------------- " << aRhs.getNumRows() << "x" << aRhs.getNumCols() << "-matrix" << "-------------- " << std::endl;
  for(unsigned int i=0, n=aRhs.getNumRows(); i<n; ++i)
  {
    const tElementType* pRow = aRhs.getRow(i);
    for(unsigned int j=0, m=aRhs.getNumCols(); j<m; ++j)
    {
      os << pRow[j] << " ";
    }
    os << std::endl;
  }
  os << "--------------------------------------------" << std::endl;
  return os;
}

} }

#endif

