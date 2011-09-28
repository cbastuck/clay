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

#include "ModuleInputBase.h"
#include "Module.h"

namespace CLAY{ 
  
//---------------------------------------------ModuleInputBase
ModuleInputBase::ModuleInputBase() 
  : m_eClayType(ClayTypes::UNKNOWN_TYPE),
    m_pParent(NULL),
    m_pConnectedOutput(NULL)
{

}

//---------------------------------------------ModuleInputBase
ModuleInputBase::ModuleInputBase(ClayTypes::Type eType) 
  : m_eClayType(eType),
    m_pParent(NULL),
    m_pConnectedOutput(NULL)
{

}

//---------------------------------------------~ModuleInputBase
ModuleInputBase::~ModuleInputBase()
{
  //check if the input is correctly disconnected
  CLAY_ASSERT(!m_pConnectedOutput);
}

//---------------------------------------------save
bool ModuleInputBase::save(XERCES::DOMElement*) const
{
  return true;
}

//---------------------------------------------load
bool ModuleInputBase::load(XERCES::DOMElement*)
{
  return true;
}

//---------------------------------------------connect
bool ModuleInputBase::connect(ModuleOutputBase* pOutput, bool bCheckCompatibility)
{
  if(pOutput && (!bCheckCompatibility || canConnect(pOutput)))
  {
    m_pConnectedOutput = pOutput;
    return true;
  }
  return false;
}

//---------------------------------------------disconnect
bool ModuleInputBase::disconnect()
{
  if(isConnected()) //otherwise this does not make any sense
  {
    m_pConnectedOutput = NULL;
    CLAY_ASSERT(m_pParent);
    m_pParent->onInputDisconnected(this);
    return true;
  }
  return false;
}

//---------------------------------------------onInputDataChange
void ModuleInputBase::onInputDataChange()
{
  tInChangeCallbackCollection::iterator it  = m_collRegisteredInputChangeCallbacks.begin();
  tInChangeCallbackCollection::iterator end = m_collRegisteredInputChangeCallbacks.end();
  for(; it!=end; ++it)
  {
    (*it)();
  }
}

//---------------------------------------------getSourceModule
Module* ModuleInputBase::getSourceModule()
{
  if(isConnected())
  {
    return m_pConnectedOutput->getParentModule();    
  }

  return NULL;
}

//---------------------------------------------registerInputChangeCallback
void ModuleInputBase::registerInputChangeCallback(tInputChangeCallback aCallback)
{
  m_collRegisteredInputChangeCallbacks.push_back(aCallback);
}

}