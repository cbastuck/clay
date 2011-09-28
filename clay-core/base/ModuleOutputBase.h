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

#ifndef MODULE_OUTPUT_BASE_H_
#define MODULE_OUTPUT_BASE_H_

#include "ClayDefines.h"
#include <clay-core/helper/XMLHelper.h>
#include <clay-data-types/base/ClayTypes.h>
#include <clay-core/base/ModuleConnectionDescriptor.h>

//STL 
#include <deque>
#include <bitset>

namespace CLAY{

class Module;
class ModuleInputBase;

class ModuleOutputBase
{
public:
  typedef std::bitset<32> tProperties;
  enum OutputProperty
  {
  };

  CLAY_DLL_EXPORT ModuleOutputBase();
  CLAY_DLL_EXPORT ModuleOutputBase(ClayTypes::Type);
  CLAY_DLL_EXPORT virtual ~ModuleOutputBase();

  CLAY_DLL_EXPORT virtual ModuleOutputBase* createProxy() = 0;

  CLAY_DLL_EXPORT virtual bool save(XERCES::DOMElement* pParent) const;
  CLAY_DLL_EXPORT virtual bool load(XERCES::DOMElement* pParent, tConnectionDescriptorCollection& aConnections);

  CLAY_DLL_EXPORT virtual bool canConnect(ModuleInputBase* pInput) = 0;
  CLAY_DLL_EXPORT virtual bool connect(ModuleInputBase* pInput, bool bCheckCompatibility);
  CLAY_DLL_EXPORT virtual bool disconnect();
  CLAY_DLL_EXPORT virtual bool disconnect(ModuleInputBase* pInput);
  CLAY_DLL_EXPORT bool isConnected() const; //inline
  CLAY_DLL_EXPORT unsigned int getNumConnections() const; //inline

  CLAY_DLL_EXPORT virtual void onOutputDataChanged();
    
  CLAY_DLL_EXPORT Module* getTargetModule(unsigned int uIdx);
  CLAY_DLL_EXPORT ModuleInputBase* getOpponent(unsigned int uIdx); //inline
  
  CLAY_DLL_EXPORT void setParentModule(Module* pParent); //inline
  CLAY_DLL_EXPORT Module* getParentModule(); //inline

  CLAY_DLL_EXPORT void setName(const tString& sName); //inline
  CLAY_DLL_EXPORT const tString& getName() const; //inline

  CLAY_DLL_EXPORT ClayTypes::Type getClayType() const; //inline

  CLAY_DLL_EXPORT bool getProperty(OutputProperty aProperty) const; //inline 

protected:
  CLAY_DLL_EXPORT bool restoreConnection(XERCES::DOMElement* pNode, tConnectionDescriptorCollection& aConnections);
  CLAY_DLL_EXPORT void setProperty(OutputProperty aProperty, bool bValue); //inline 

protected:
  typedef std::deque<ModuleInputBase*> tConnectedInputs;

  ClayTypes::Type  m_eClayType;
  Module*          m_pParent;
  //ModuleInputBase* m_pConnectedInput;
  tConnectedInputs m_aConnectedInputs;
  tString          m_sName;
  tProperties      m_aProperties;
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------isConnected
inline bool ModuleOutputBase::isConnected() const
{
  return getNumConnections() > 0; //m_pConnectedInput != NULL; 
}

//---------------------------------------------getNumConnections
inline unsigned int ModuleOutputBase::getNumConnections() const
{
  return m_aConnectedInputs.size();
}

//---------------------------------------------getOpponent
inline ModuleInputBase* ModuleOutputBase::getOpponent(unsigned int uIdx)
{ 
  CLAY_ASSERT(getNumConnections() > uIdx);
  return m_aConnectedInputs[uIdx]; //m_pConnectedInput;
}

//---------------------------------------------setParentModule
inline void ModuleOutputBase::setParentModule(Module* pParent)
{ 
  m_pParent = pParent; 
}

//---------------------------------------------getParentModule
inline Module* ModuleOutputBase::getParentModule()
{ 
  return m_pParent; 
}

//---------------------------------------------setName
inline void ModuleOutputBase::setName(const tString& sName)
{ 
  m_sName = sName; 
}

//---------------------------------------------getName
inline const tString& ModuleOutputBase::getName() const
{ 
  return m_sName; 
}

//---------------------------------------------getClayType
inline ClayTypes::Type ModuleOutputBase::getClayType() const
{ 
  return m_eClayType; 
}

//---------------------------------------------getProperty
inline bool ModuleOutputBase::getProperty(OutputProperty aProperty) const
{
  return m_aProperties[aProperty];
}

//---------------------------------------------setProperty
inline void ModuleOutputBase::setProperty(OutputProperty aProperty, bool bValue)
{
  m_aProperties[aProperty] = bValue;
}

}

#endif
