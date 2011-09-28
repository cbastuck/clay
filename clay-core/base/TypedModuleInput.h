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

#ifndef TYPED_MODULE_INPUT_H_
#define TYPED_MODULE_INPUT_H_

#include <clay-core/helper/Types.h>
#include <clay-core/base/ModuleInputBase.h>
#include <clay-core/base/ModuleInputProxy.h>
#include <clay-data-types/base/ClayTypeChecking.h>

namespace CLAY{

template<class T>
class TypedModuleInput : public ModuleInputBase
{
public:
  typedef T tInputType;

  TypedModuleInput();

  virtual ModuleInputBase* createProxy();

  virtual bool canConnect(ModuleOutputBase* pOutput);

  const T* getData() const;
  T* getData();

protected:
  void setConnectedOutput(ModuleOutputBase* pOutput);

};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------TypedModuleInput
template<class T>
TypedModuleInput<T>::TypedModuleInput()
{
}

//---------------------------------------------canConnect
template<class T>
inline bool TypedModuleInput<T>::canConnect(ModuleOutputBase* pOutput)
{
  return MODEL::ClayTypeChecking::getInstance()->isModuleOutputCompatible(this, pOutput);
}

//---------------------------------------------createProxy
template<class T>
inline ModuleInputBase* TypedModuleInput<T>::createProxy()
{
  return new ModuleInputProxy<T>(*this);
}

//---------------------------------------------getData
template<class T>
inline const T* TypedModuleInput<T>::getData() const
{ 
  if(m_pConnectedOutput)
  {
    const TypedModuleOutput<T>* pOutput = static_cast<const TypedModuleOutput<T>*>(m_pConnectedOutput);
    return &pOutput->getData(); 
  }
  return NULL;
}

//---------------------------------------------getData
template<class T>
inline T* TypedModuleInput<T>::getData()
{ 
  const TypedModuleInput<T>* pConstThis = this;
  return const_cast<T*>(pConstThis->getData());
}

//---------------------------------------------getData
template<class T>
inline void TypedModuleInput<T>::setConnectedOutput(ModuleOutputBase* pOutput)
{
	m_pConnectedOutput = pOutput;
}

}


#endif
