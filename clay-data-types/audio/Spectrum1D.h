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

#ifndef SPECTRUM1D_H_
#define SPECTRUM1D_H_

//#include <clay-data-types/base/MatrixModel.h>
#include <clay-data-types/base/Matrix.h>

#include <complex>

namespace CLAY{ namespace MODEL{

class Spectrum1D : public Matrix< std::complex<float> >
{
private:
  typedef Matrix< std::complex<float> > tBase;
  typedef float tFFTWComplex[2]; //supports import()

public:
  Spectrum1D();
  ~Spectrum1D();

  void import(tFFTWComplex* pIn, unsigned int uNumCoeff);

  void abs(Matrix<float>& aDst) const;

private:
};

} } 

#endif
