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

#ifndef TYPES_MODULE_OUTPUT_H_
#define TYPES_MODULE_OUTPUT_H_

#include "ModuleIOBase.h"
#include "ModuleOutputProxy.h"

#include <clay-core/helper/Types.h>
#include <clay-data-types/base/ClayTypeChecking.h>

namespace CLAY{

template<class T>
class TypedModuleOutput : public ModuleOutputBase
{
private:
  typedef ModuleOutputBase tBase;

public:
  typedef T tOutputType;

  TypedModuleOutput(T* pOutputData);

  virtual ModuleOutputBase* createProxy();

  virtual bool canConnect(ModuleInputBase* pInput);
  virtual bool connect(ModuleInputBase* pInput, bool bCheckCompatibility);

  void setData(const T& aData);
  const T& getData() const;
  T& getData();

private:
  T* m_pOutputData;
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------TypedModuleOutput
template<class T>
inline TypedModuleOutput<T>::TypedModuleOutput(T* pOutputData) 
  : m_pOutputData(pOutputData)        
{

}

//---------------------------------------------createProxy
template<class T>
inline ModuleOutputBase* TypedModuleOutput<T>::createProxy()
{
  ModuleOutputProxy<T>* pProxy = new ModuleOutputProxy<T>(*this);
  return pProxy;
}

//---------------------------------------------canConnect
template<class T>
inline bool TypedModuleOutput<T>::canConnect(ModuleInputBase* pInput)
{
  return MODEL::ClayTypeChecking::getInstance()->isModuleInputCompatible(this, pInput);
}

//---------------------------------------------connect
template<class T>
inline bool TypedModuleOutput<T>::connect(ModuleInputBase* pInput, bool bCheckCompatibility)
{
  if(tBase::connect(pInput, bCheckCompatibility))
  {
    return true;
  }
  return false;
}

//---------------------------------------------setData
template<class T>
inline void TypedModuleOutput<T>::setData(const T& aData)
{ 
  *m_pOutputData = aData; 
  onOutputDataChanged();
}

//---------------------------------------------getData
template<class T>
inline const T& TypedModuleOutput<T>::getData() const
{ 
  return *m_pOutputData; 
}

//---------------------------------------------getData
template<class T>
inline T& TypedModuleOutput<T>::getData()
{ 
  return *m_pOutputData; 
}

}

#endif 
