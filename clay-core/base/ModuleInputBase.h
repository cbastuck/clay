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

#ifndef MODULE_INPUT_BASE_H_
#define MODULE_INPUT_BASE_H_

#include "ClayDefines.h"
#include <clay-core/helper/XMLHelper.h>
#include <clay-data-types/base/ClayTypes.h>

#include <deque>
#include <boost/function.hpp>

namespace CLAY{

//forward declarations
class Module;
class ModuleOutputBase;

//---------------------------ModuleInputBase
class ModuleInputBase
{
public:
  CLAY_DLL_EXPORT ModuleInputBase();
  CLAY_DLL_EXPORT ModuleInputBase(ClayTypes::Type);
  CLAY_DLL_EXPORT virtual ~ModuleInputBase();

  CLAY_DLL_EXPORT virtual ModuleInputBase* clone() const = 0;
  CLAY_DLL_EXPORT virtual ModuleInputBase* createProxy() = 0;

  CLAY_DLL_EXPORT virtual bool save(XERCES::DOMElement* pParent) const;
  CLAY_DLL_EXPORT virtual bool load(XERCES::DOMElement* pParent);

  CLAY_DLL_EXPORT virtual bool canConnect(ModuleOutputBase* pOutput) = 0;

  CLAY_DLL_EXPORT virtual bool connect(ModuleOutputBase* pOutput, bool bCheckCompatibility);

  CLAY_DLL_EXPORT virtual bool disconnect();

  CLAY_DLL_EXPORT virtual void onInputDataChange();

  CLAY_DLL_EXPORT Module* getSourceModule();

  typedef boost::function0<void> tInputChangeCallback;
  CLAY_DLL_EXPORT void registerInputChangeCallback(tInputChangeCallback aCallback);

  CLAY_DLL_EXPORT virtual bool isConnected() const; //inline

  CLAY_DLL_EXPORT void setParentModule(Module* pParent); //inline
  CLAY_DLL_EXPORT Module* getParentModule(); //inline

  CLAY_DLL_EXPORT void setName(const tString& sName); //inline
  CLAY_DLL_EXPORT const tString& getName() const; //inline

  CLAY_DLL_EXPORT ModuleOutputBase* getOpponent(); //inline

  CLAY_DLL_EXPORT ClayTypes::Type getClayType() const; //inline

protected:
  typedef std::deque<tInputChangeCallback> tInChangeCallbackCollection;

  ClayTypes::Type             m_eClayType;
  Module*                     m_pParent;
  ModuleOutputBase*           m_pConnectedOutput;
  tString                     m_sName;
  tInChangeCallbackCollection m_collRegisteredInputChangeCallbacks;
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------isConnected
inline bool ModuleInputBase::isConnected() const
{ 
  return m_pConnectedOutput != NULL; 
}

//---------------------------------------------setParentModule
inline void ModuleInputBase::setParentModule(Module* pParent)
{ 
  m_pParent = pParent; 
}

//---------------------------------------------getParentModule
inline Module* ModuleInputBase::getParentModule()
{ 
  return m_pParent; 
}

//---------------------------------------------setName
inline void ModuleInputBase::setName(const tString& sName)
{ 
  m_sName = sName; 
}

//---------------------------------------------getName
inline const tString& ModuleInputBase::getName() const
{ 
  return m_sName; 
}

//---------------------------------------------getOpponent
inline ModuleOutputBase* ModuleInputBase::getOpponent()
{ 
  return m_pConnectedOutput; 
}

//---------------------------------------------getClayType
inline ClayTypes::Type ModuleInputBase::getClayType() const
{ 
  return m_eClayType; 
}

}

#endif 
