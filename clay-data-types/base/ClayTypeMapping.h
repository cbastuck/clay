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

#ifndef CLAY_TYPE_MAPPING_H_
#define CLAY_TYPE_MAPPING_H_

#include <clay-core/base/ClayDefines.h>

#include "ClayTypes.h"

#include "ClayTypeMappingDetail.h"

namespace CLAY{ namespace MODEL{ namespace ClayTypeMapping{

/*
 * mapping from CPP type to clay type at compile-time
 */
template<class T>
inline ClayTypes::Type getType();

/*
 * mapping from clay type to CPP type at compile-time
 */
template<ClayTypes::Type eType>
class MapType
{
public:
  typedef typename ClayType<eType>::tType tType;
};

} } }

#endif 
