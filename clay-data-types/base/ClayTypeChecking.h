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

#ifndef CLAY_TYPE_CHECKING_H_
#define CLAY_TYPE_CHECKING_H_

#include <clay-core/base/ClayDefines.h>
#include <clay-core/base/ModuleIOBase.h>

namespace CLAY{
  template<class T> class TypedModuleInput;
  template<class T> class TypedModuleOutput;
}

namespace CLAY{ namespace MODEL{

class ClayTypeConversion;

class ClayTypeChecking
{
public:
  static ClayTypeChecking* getInstance(); //singleton access

  static void init();

  static void deInit();

  bool isConvertible(ModuleInputBase* pModuleInput, ModuleOutputBase* pModuleOutput);

  template<class T>
  bool isModuleInputCompatible(T* pModuleOutput, ModuleInputBase* pModuleInput)
  {
    typedef typename T::tOutputType tType;

    if(dynamic_cast<TypedModuleInput<tType>*>(pModuleInput))
      return true;
    return false;

  }

  template<class T>
  bool isModuleOutputCompatible(T* pModuleInput, ModuleOutputBase* pModuleOutput)
  {
    typedef typename T::tInputType tType;

    if(dynamic_cast<TypedModuleOutput<tType>*>(pModuleOutput))
      return true;
    return false;
  }

private:
  ClayTypeChecking();  //use singleton access

  ~ClayTypeChecking(); //use singleton access

  ClayTypeConversion* m_pClayTypeConverter;
};

} }

#endif
