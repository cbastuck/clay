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

#ifndef TYPE_CONVERSION_H_
#define TYPE_CONVERSION_H_

#include "ClayTypes.h"
#include <clay-core/base/ClayDefines.h>

#include <boost/function.hpp>

#include <map>

namespace CLAY{
class Module;

namespace MODEL{

class ClayTypeConversion
{
private:
  typedef boost::function<Module*(const tString&)>             tConversionModuleFactory;
  typedef std::map<ClayTypes::Type, tConversionModuleFactory>  tConvertModules;
  typedef std::map<ClayTypes::Type, tConvertModules>           tTypeMappingMatrix;

public:
  ClayTypeConversion();
  ~ClayTypeConversion();

  void init();
  void deInit();

  tConversionModuleFactory getConversionModuleFactory(ClayTypes::Type eSrcType, ClayTypes::Type eDstType);

  bool isConversionAvailable(ClayTypes::Type eSrcType, ClayTypes::Type eDstType);

private:
  tTypeMappingMatrix m_collTypeMappings;
};

} }

#endif 
