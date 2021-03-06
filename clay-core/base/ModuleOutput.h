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

#ifndef MODULEOUTPUT_H_
#define MODULEOUTPUT_H_

#include "ModuleIOBase.h"
#include "TypedModuleOutput.h"

namespace CLAY{

template<class T>
class ModuleOutput : public TypedModuleOutput<T> 
{
private:
  typedef TypedModuleOutput<T> tBase;

public:
  ModuleOutput();
  ModuleOutput(const T& aPrototype);

private:
  T m_aData;
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------ModuleOutput
template<class T>
inline ModuleOutput<T>::ModuleOutput()
  : tBase(&m_aData)
{
}

//---------------------------------------------ModuleOutput
template<class T>
inline ModuleOutput<T>::ModuleOutput(const T& aPrototype)
  : tBase(&m_aData),
    m_aData(aPrototype)
{
}

}

#endif
