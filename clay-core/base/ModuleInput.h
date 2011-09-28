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

#ifndef MODULEINPUT_H_
#define MODULEINPUT_H_

#include <clay-core/base/TypedModuleInput.h>
#include <clay-core/base/ModuleOutputBase.h>

namespace CLAY{

//forward declarations
template<class T> class TypedModuleOutput;

template<class T>
class ModuleInput : public TypedModuleInput<T>
{
private:
  typedef TypedModuleInput<T> tBase;

public:
  ModuleInput();

  virtual ModuleInputBase* clone() const;
  
private:
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------ModuleInput
template<class T>
inline ModuleInput<T>::ModuleInput()
{
  
}

//---------------------------------------------clone
template<class T>
inline ModuleInputBase* ModuleInput<T>::clone() const
{
  return new ModuleInput<T>(*this);
}


}

#endif
