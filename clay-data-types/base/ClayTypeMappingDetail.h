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

#ifndef ClayTypeMapppingDetail_H_
#define ClayTypeMapppingDetail_H_

namespace CLAY{ namespace MODEL{ namespace ClayTypeMapping{


/****************************************************************
 * --- template specializations from CPP type to CLAY type ---- *
 ****************************************************************/
template<class T>
inline ClayTypes::Type getType()
{
  CLAY_FAIL(); //if you arrive here a template specialization is missing, see cpp file
  return ClayTypes::UNKNOWN_TYPE;
}

//---------------------------------------------
template<>
inline ClayTypes::Type getType<unsigned int>()
{
  return ClayTypes::UNSIGNED_INT;
}

//---------------------------------------------
template<>
inline ClayTypes::Type getType<int>()
{
  return ClayTypes::INT;
}

//---------------------------------------------
template<>
inline ClayTypes::Type getType<unsigned short>()
{
  return ClayTypes::UNSIGNED_SHORT;
}

//---------------------------------------------
template<>
inline ClayTypes::Type getType<short>()
{
  return ClayTypes::SHORT;
}

//---------------------------------------------
template<>
inline ClayTypes::Type getType<float>()
{
  return ClayTypes::FLOAT;
}

//---------------------------------------------
template<>
inline ClayTypes::Type getType<double>()
{
  return ClayTypes::DOUBLE;
}

/****************************************************************
 * --- template specializations from CLAY type to CPP type ---- *
 ****************************************************************/
template<ClayTypes::Type>
class ClayType
{
public:
  typedef void tType; //if you arrive here a class template specialization is missing
};

//---------------------------------------------
template<>
class ClayType<ClayTypes::UNSIGNED_INT>
{
public:
  typedef unsigned int tType;
};

//---------------------------------------------
template<>
class ClayType<ClayTypes::INT>
{
public:
  typedef int tType;
};

//---------------------------------------------
template<>
class ClayType<ClayTypes::UNSIGNED_SHORT>
{
public:
  typedef unsigned short tType;
};

//---------------------------------------------
template<>
class ClayType<ClayTypes::SHORT>
{
public:
  typedef short tType;
};

//---------------------------------------------
template<>
class ClayType<ClayTypes::FLOAT>
{
public:
  typedef float tType;
};

//---------------------------------------------
template<>
class ClayType<ClayTypes::DOUBLE>
{
public:
  typedef double tType;
};

} } }

#endif
