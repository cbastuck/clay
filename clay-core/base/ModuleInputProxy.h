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

#ifndef MODULE_INPUT_PROXY_H_
#define MODULE_INPUT_PROXY_H_

namespace CLAY {

template<class T>
class TypedModuleInput;

template<class T>
class ModuleInputProxy: public TypedModuleInput<T>
{
private:
  typedef TypedModuleInput<T> tBase;

public:
  ModuleInputProxy(TypedModuleInput<T>& aMaster);

  virtual ModuleInputBase* clone() const;

  virtual bool canConnect(ModuleOutputBase* pOutput);
  virtual bool connect(ModuleOutputBase* pOutput, bool bCheckCompatibility);

private:
  TypedModuleInput<T>* m_pOrigin;
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------ModuleInputProxy
template<class T>
inline ModuleInputProxy<T>::ModuleInputProxy(TypedModuleInput<T>& aMaster) :
    m_pOrigin(&aMaster)
{
}

//---------------------------------------------clone
template<class T>
inline ModuleInputBase* ModuleInputProxy<T>::clone() const
{
  return new ModuleInputProxy<T>(*this);
}

//---------------------------------------------canConnect
template<class T>
bool ModuleInputProxy<T>::canConnect(ModuleOutputBase* pOutput)
{
  return m_pOrigin->canConnect(pOutput);
}

//---------------------------------------------connect
template<class T>
bool ModuleInputProxy<T>::connect(ModuleOutputBase* pOutput, bool bCheckCompatibility)
{
  if(m_pOrigin->connect(pOutput, bCheckCompatibility))
  {
    tBase::setConnectedOutput(pOutput);
    return true;
  }
  return false;
}

}

#endif
