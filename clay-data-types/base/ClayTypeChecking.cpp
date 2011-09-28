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

#include "ClayTypeChecking.h"

#include "ClayTypeConversion.h"

namespace CLAY{ namespace MODEL{

//---------------------------------------------getInstance
ClayTypeChecking* ClayTypeChecking::getInstance()
{
  static ClayTypeChecking* pTypeChecker = NULL;

  if(!pTypeChecker)
  {
    pTypeChecker = new ClayTypeChecking();
  }

  return pTypeChecker;
}

//---------------------------------------------init
void ClayTypeChecking::init()
{
  ClayTypeChecking* pThis = getInstance();

  if(!pThis->m_pClayTypeConverter)
  {
    pThis->m_pClayTypeConverter = new ClayTypeConversion();
    pThis->m_pClayTypeConverter->init();
  }
}

//---------------------------------------------deInit
void ClayTypeChecking::deInit()
{
  ClayTypeChecking* pThis = getInstance();

  if(pThis->m_pClayTypeConverter)
  {
    pThis->m_pClayTypeConverter->deInit();
    delete pThis->m_pClayTypeConverter;
    pThis->m_pClayTypeConverter = NULL;
  }

  delete pThis; //destructor is private - self destruction on deInit();
}

//---------------------------------------------isModuleInputConvertible
bool ClayTypeChecking::isConvertible(ModuleInputBase* pModuleInput, ModuleOutputBase* pModuleOutput)
{
  CLAY_ASSERT(m_pClayTypeConverter);
  return m_pClayTypeConverter->isConversionAvailable(pModuleInput->getClayType(), pModuleOutput->getClayType());
}

//---------------------------------------------ClayTypeChecking
ClayTypeChecking::ClayTypeChecking()
  : m_pClayTypeConverter(NULL)
{

}

//---------------------------------------------~ClayTypeChecking
ClayTypeChecking::~ClayTypeChecking()
{
  CLAY_ASSERT(!m_pClayTypeConverter); //did you forget to call deInit()?
}

} }
