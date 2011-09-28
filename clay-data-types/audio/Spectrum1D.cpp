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

#include "Spectrum1D.h"

namespace CLAY{ namespace MODEL{

//---------------------------------------------Spectrum1D
Spectrum1D::Spectrum1D()
{
  //tBase::init();
}

//---------------------------------------------~Spectrum1D
Spectrum1D::~Spectrum1D()
{
  //tBase::deInit();
}

//---------------------------------------------import
void Spectrum1D::import(tFFTWComplex* pIn, unsigned int uNumCoeff)
{
  if(getNumColumns() < uNumCoeff)
  {
    tBase::resize(1, uNumCoeff);
  }

  for(unsigned int i=0; i<uNumCoeff; ++i)
  {
    tBase::operator()(0, i) = std::complex<float>(pIn[i][0], pIn[i][1]);
  }
}

//---------------------------------------------abs
void Spectrum1D::abs(Matrix<float>& aDst) const
{
  unsigned int uNumCoeff = getNumColumns(); 
  aDst.resize(1, uNumCoeff);

  for(unsigned int i=0; i<uNumCoeff; ++i)
  {
    aDst(0, i) = std::abs(tBase::operator()(0, i));
  }
}

} }
