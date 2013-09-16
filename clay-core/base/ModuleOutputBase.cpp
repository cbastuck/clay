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

#include "ModuleOutputBase.h"

#include "Module.h"

#define XML_ROOT_NODE               "output-root"
#define XML_OUTPUT_NAME_ATTR        "output-name"
#define XML_CONNECTION_NODE         "output-connection"
#define XML_MODULE_RUNTIME_ID_ATTR  "module-id"
#define XML_MODULE_INPUT_NAME_ATTR  "input-name"

namespace CLAY{

//---------------------------------------------ModuleOutputBase
ModuleOutputBase::ModuleOutputBase() 
  : m_eClayType(ClayTypes::UNKNOWN_TYPE),
    m_pParent(NULL)
    //,m_pConnectedInput(NULL)
{

}

//---------------------------------------------ModuleOutputBase
ModuleOutputBase::ModuleOutputBase(ClayTypes::Type eType)
  : m_eClayType(eType),
    m_pParent(NULL)
    //,m_pConnectedInput(NULL)
{

}

//---------------------------------------------~ModuleOutputBase
ModuleOutputBase::~ModuleOutputBase()
{
  //disconnect before deleting 
  CLAY_ASSERT(!isConnected());
  //CLAY_ASSERT(!m_pConnectedInput);
}

//---------------------------------------------save
bool ModuleOutputBase::save(XERCES::DOMElement* pParent) const
{
  XERCES::DOMElement* pOutputNode = XercesXML::appendNode(pParent, XML_ROOT_NODE);

  XercesXML::addAttribute(pOutputNode, XML_OUTPUT_NAME_ATTR, m_sName);

  if(isConnected()) //write connection details if connected
  {
    tConnectedInputs::const_iterator it = m_aConnectedInputs.begin();
    tConnectedInputs::const_iterator ed = m_aConnectedInputs.end();
    for(; it!=ed; ++it)
    {
      XERCES::DOMElement* pConnectionNode = XercesXML::appendNode(pOutputNode, XML_CONNECTION_NODE);

      ModuleInputBase* pOpponentInput = *it;
      Module* pOpponentModule = pOpponentInput->getParentModule();
      CLAY_ASSERT(pOpponentModule);
      
      XercesXML::addAttribute(pConnectionNode, XML_MODULE_RUNTIME_ID_ATTR, pOpponentModule->getRuntimeModuleID());
      XercesXML::addAttribute(pConnectionNode, XML_MODULE_INPUT_NAME_ATTR, pOpponentInput->getName());
    }
  }
  return true;
}

//---------------------------------------------load
bool ModuleOutputBase::load(XERCES::DOMElement* pParent, tConnectionDescriptorCollection& aConnections)
{
  XERCES::DOMElement* pOutputNode = XercesXML::findChildNode(pParent, XML_ROOT_NODE);
  if(!pOutputNode)
  {
    return false;
  }

  tString sOutputName; 
  XercesXML::getAttributeValue(pOutputNode, XML_OUTPUT_NAME_ATTR, sOutputName);
  CLAY_ASSERT(sOutputName == m_sName);

  XERCES::DOMElement* pOutputCnxNode = XercesXML::findChildNode(pOutputNode, XML_CONNECTION_NODE);
  if(!pOutputCnxNode)
  {
    return false;
  }

  XercesXML::visitChildNodes(pOutputCnxNode, boost::bind(&ModuleOutputBase::restoreConnection, this, _1, boost::ref(aConnections))); 

  return true;
}

//---------------------------------------------connect
bool ModuleOutputBase::connect(ModuleInputBase* pInput, bool bCheckCompatibility)
{
  if(pInput && (!bCheckCompatibility || canConnect(pInput)))
  {
    //m_pConnectedInput = pInput;
    if(std::find(m_aConnectedInputs.begin(), m_aConnectedInputs.end(), pInput) == m_aConnectedInputs.end())
    {
      m_aConnectedInputs.push_back(pInput);
    }
    return true;
  }
  return false;
}

//---------------------------------------------disconnect
bool ModuleOutputBase::disconnect()
{
  if(isConnected()) //otherwise this call does not make any sense
  {
    m_aConnectedInputs.clear();

    CLAY_ASSERT(m_pParent);
    m_pParent->onOutputDisconnected(this);
    return true;
  }
  return false;
}

//---------------------------------------------disconnect
bool ModuleOutputBase::disconnect(ModuleInputBase* pInput)
{
  tConnectedInputs::iterator pos = std::find(m_aConnectedInputs.begin(), m_aConnectedInputs.end(), pInput);
  if(pos != m_aConnectedInputs.end())
  {
    m_aConnectedInputs.erase(pos);

    //TODO: send onOutputDisconnected() with parameter pInput
    return true;
  }
  return false;
}

//---------------------------------------------onOutputDataChanged
void ModuleOutputBase::onOutputDataChanged()
{
  if(isConnected())
  {
    tConnectedInputs::iterator it = m_aConnectedInputs.begin();
    tConnectedInputs::iterator ed = m_aConnectedInputs.end();
    for(; it!=ed; ++it)
    {
      ModuleInputBase* pInput = *it;
      pInput->onInputDataChange();
    }
  }
}

//---------------------------------------------getTargetModule
Module* ModuleOutputBase::getTargetModule(unsigned int uIdx)
{
  if(!isConnected())
  {
    return NULL;
  }

  CLAY_ASSERT(getNumConnections() > uIdx);
  return m_aConnectedInputs[uIdx]->getParentModule();
}

//---------------------------------------------restoreConnection
bool ModuleOutputBase::restoreConnection(XERCES::DOMElement* pNode, tConnectionDescriptorCollection& aConnections)
{
  tString sOpponentModuleRuntimeId;
  if(!XercesXML::getAttributeValue(pNode, XML_MODULE_RUNTIME_ID_ATTR, sOpponentModuleRuntimeId))
  {
    return false;
  }

  tString sOpponentInputName;
  if(!XercesXML::getAttributeValue(pNode, XML_MODULE_INPUT_NAME_ATTR, sOpponentInputName))
  {
    return false;
  }

  ModuleConnectionDescriptor aConnection;
  aConnection.inputModule  = sOpponentModuleRuntimeId;
  aConnection.inputName    = sOpponentInputName;
  aConnection.outputModule = getParentModule()->getRuntimeModuleID();
  aConnection.outputName   = m_sName;
  aConnections.push_back(aConnection);

  return true;
}

}
