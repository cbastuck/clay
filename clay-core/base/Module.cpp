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

#include "Module.h"

//CLAY
#include <helper/XMLHelper.h>
#include <helper/LexicalConversion.h>
#include <clay-core/base/ClayShaper.h>
#include <clay-core/base/ClayExecutable.h>
#include <clay-core/base/ModuleDescriptor.h>
#include <clay-core/base/ModuleDependencies.h>

//STL
#include <algorithm>

//boost
#include <boost/bind.hpp>

#define XML_MODULE_ROOT_TAG               "module"
#define XML_MODULE_IO_TAG                 "module-io"
#define XML_MODULE_MODULE_INPUT_TAG       "module-input"
#define XML_MODULE_MODULE_OUTPUT_TAG      "module-output"
#define XML_MODULE_MODULE_INPUTS_TAG      "module-inputs"
#define XML_MODULE_MODULE_OUTPUTS_TAG     "module-outputs"
#define XML_MODULE_INPUT_CONNECTION_NODE  "input-connection"
#define XML_MODULE_OUTPUT_CONNECTION_NODE "output-connection"

#define XML_MODULE_RUNTIME_ID_ATTR        "module-runtime-id"
#define XML_MODULE_NAMESPACE_ATTR         "module-namespace"
#define XML_MODULE_OUTPUT_NAME_ATTR       "output-name"
#define XML_MODULE_OUTPUT_PROXY_ATTR      "proxy"
#define XML_MODULE_INPUT_NAME_ATTR        "input-name"
#define XML_MODULE_ID_ATTR                "module-id"

namespace CLAY{

//---------------------------------------------Module
Module::Module(const tString& sModuleRuntimeId)
  : m_sModuleRuntimeID(sModuleRuntimeId),
    m_uState(0)
{
}

//---------------------------------------------~Module
Module::~Module()
{

}

//---------------------------------------------init
bool Module::init(XERCES::DOMNode* pNode)
{
  setModuleState(MODULE_INITIALIZED);
  return true;
}

//---------------------------------------------deInit
void Module::deInit()
{
  while(getNumInputs())
  {
    unregisterInput(*beginInput());
  }

  while(getNumOutputs())
  {
    unregisterOutput(*beginOutput());
  }

  m_uState = MODULE_UNINITIALIZED;
}

//---------------------------------------------getModuleDescriptor
const ModuleDescriptorBase* Module::getModuleDescriptor() const
{
  return NULL;
}

//---------------------------------------------save
bool Module::save(XERCES::DOMElement* pNode)
{
  XERCES::DOMElement* pModuleRoot = XercesXML::appendNode(pNode, XML_MODULE_ROOT_TAG);
  XercesXML::addAttribute(pModuleRoot, XML_MODULE_RUNTIME_ID_ATTR, m_sModuleRuntimeID.c_str());

  const ModuleDescriptorBase* pDescr = getModuleDescriptor();
  if(pDescr)
  {
    XercesXML::addAttribute(pModuleRoot, "module-id",        pDescr->getModuleIdAsString().c_str());
    XercesXML::addAttribute(pModuleRoot, "module-namespace", pDescr->getNamespaceIdAsString().c_str());
  }

  //save the module-inputs
  XERCES::DOMElement* pModuleIONode    = XercesXML::appendNode(pModuleRoot,   XML_MODULE_IO_TAG);
  XERCES::DOMElement* pInputsNode      = XercesXML::appendNode(pModuleIONode, XML_MODULE_MODULE_INPUTS_TAG);
  tInputCollection::const_iterator it  = m_collInputs.begin();
  tInputCollection::const_iterator end = m_collInputs.end();
  for(; it!=end; ++it)
  {
    ModuleInputBase* pModuleInput = *it;
    if(!saveModuleInput(pInputsNode, pModuleInput))
    {
      return false;
    }
  }

  //save the module-outputs
  XERCES::DOMElement* pOutputsNode      = XercesXML::appendNode(pModuleIONode, XML_MODULE_MODULE_OUTPUTS_TAG);
  tOutputCollection::const_iterator ti  = m_collOutputs.begin();
  tOutputCollection::const_iterator dne = m_collOutputs.end();
  for(; ti!=dne; ++ti)
  {
    ModuleOutputBase* pModuleOutput = *ti;
    if(!saveModuleOutput(pOutputsNode, pModuleOutput))
    {
      return false;
    }
  }

  return true;
}

//---------------------------------------------load
bool Module::load(XERCES::DOMElement* pNode, tConnectionMap* pInputConnections, tConnectionMap* pOutputConnections)
{
  if(!XercesXML::nodeHasName(pNode, XML_MODULE_ROOT_TAG))
  {
    pNode = XercesXML::findChildNode(pNode, XML_MODULE_ROOT_TAG, true);
  }
  
  if(pNode && 
    XercesXML::getAttributeValue(pNode, XML_MODULE_RUNTIME_ID_ATTR, m_sModuleRuntimeID))
  {
    tString sModuleIdAsString;
    XercesXML::getAttributeValue(pNode, XML_MODULE_ID_ATTR, sModuleIdAsString);

    tString sModuleNamespaceAsString;
    XercesXML::getAttributeValue(pNode, XML_MODULE_NAMESPACE_ATTR, sModuleIdAsString);

    //load the module inputs
    XERCES::DOMElement* pModuleInputsNode  = XercesXML::findChildNode(pNode, XML_MODULE_MODULE_INPUTS_TAG, true);
    if(!pModuleInputsNode)
    {
      return false;
    }
    XercesXML::visitChildNodes(pModuleInputsNode, boost::bind(&Module::loadModuleInput, this, _1, boost::ref(pInputConnections)));

    //load the module outputs
    XERCES::DOMElement* pModuleOutputsNode = XercesXML::findChildNode(pNode, XML_MODULE_MODULE_OUTPUTS_TAG, true);
    if(!pModuleOutputsNode)
    {
      return false;    
    }
    XercesXML::visitChildNodes(pModuleOutputsNode, boost::bind(&Module::loadModuleOutput, this, _1, boost::ref(pOutputConnections)));

    return true;
  }

  resetModuleState(); //this module no state - i.e. is invalid
  return false; //a module runtime-id MUST always be available
}

//---------------------------------------------saveModuleInput
bool Module::saveModuleInput(XERCES::DOMElement* pParent, ModuleInputBase* pModuleInput)
{
  XERCES::DOMElement* pModuleInputNode = XercesXML::appendNode(pParent, XML_MODULE_MODULE_INPUT_TAG);
  XercesXML::addAttribute(pModuleInputNode, XML_MODULE_INPUT_NAME_ATTR, pModuleInput->getName());

  if(!pModuleInput->save(pModuleInputNode))
  {
    return false;
  }

  if(pModuleInput->isConnected()) //write connection details if one is available
  {
    ModuleOutputBase* pOpponentOutput = pModuleInput->getOpponent();
    CLAY_ASSERT(pOpponentOutput);
    Module* pOpponentModule = pOpponentOutput->getParentModule();
    CLAY_ASSERT(pOpponentModule);
      
    XERCES::DOMElement* pConnectionNode = XercesXML::appendNode(pModuleInputNode, XML_MODULE_INPUT_CONNECTION_NODE);
    XercesXML::addAttribute(pConnectionNode, XML_MODULE_RUNTIME_ID_ATTR,  pOpponentModule->getRuntimeModuleID());
    XercesXML::addAttribute(pConnectionNode, XML_MODULE_OUTPUT_NAME_ATTR, pOpponentOutput->getName());
  }
  return true;
}

//---------------------------------------------loadModuleInput
bool Module::loadModuleInput(XERCES::DOMElement* pNode, tConnectionMap* pInputConnections)
{
  CLAY_ASSERT(XercesXML::nodeHasName(pNode, XML_MODULE_MODULE_INPUT_TAG));

  tString sInputName;
  if(!XercesXML::getAttributeValue(pNode, XML_MODULE_INPUT_NAME_ATTR, sInputName))
  {
    return false;
  }

  ModuleInputBase* pInput = getInput(sInputName);
  if(pInput)
  {
    if(!pInput->load(pNode))
    {
      return false;
    }
  }

  XERCES::DOMElement* pInputCnxNode = XercesXML::findChildNode(pNode, XML_MODULE_INPUT_CONNECTION_NODE);
  if(pInputCnxNode)
  {
    tString sOpponentModuleRuntimeId;
    if(!XercesXML::getAttributeValue(pInputCnxNode, XML_MODULE_RUNTIME_ID_ATTR, sOpponentModuleRuntimeId))
    {
      return false;
    }

    tString sOpponentOutputName;
    if(!XercesXML::getAttributeValue(pInputCnxNode, XML_MODULE_OUTPUT_NAME_ATTR, sOpponentOutputName))
    {
      return false;
    }

    if(pInputConnections)
    {
      tConnectionColl& aCollection = (*pInputConnections)[this];
      ModuleConnectionDescriptor aConnection;
      aConnection.inputModule  = getRuntimeModuleID();
      aConnection.inputName    = sInputName;
      aConnection.outputModule = sOpponentModuleRuntimeId;
      aConnection.outputName   = sOpponentOutputName;
      aCollection.push_back(aConnection);
    }

    return true;
  }
  return false;
}

//---------------------------------------------saveModuleOutput
bool Module::saveModuleOutput(XERCES::DOMElement* pParent, ModuleOutputBase* pModuleOutput)
{
  XERCES::DOMElement* pModuleOutputNode = XercesXML::appendNode(pParent, XML_MODULE_MODULE_OUTPUT_TAG);
  XercesXML::addAttribute(pModuleOutputNode, XML_MODULE_OUTPUT_NAME_ATTR, pModuleOutput->getName());

  if(!pModuleOutput->save(pModuleOutputNode))
  {
    return false;
  }

  return true;
}

//---------------------------------------------loadModuleOutput
bool Module::loadModuleOutput(XERCES::DOMElement* pNode, tConnectionMap* pOutputConnections)
{
  CLAY_ASSERT(XercesXML::nodeHasName(pNode, XML_MODULE_MODULE_OUTPUT_TAG));

  tString sOutputName;
  if(!XercesXML::getAttributeValue(pNode, XML_MODULE_OUTPUT_NAME_ATTR, sOutputName))
  {
    return false;
  }

  ModuleOutputBase* pOutput = getOutput(sOutputName);
  if(pOutput)
  {
    Module::tConnectionColl& aCollection = (*pOutputConnections)[this];
    if(!pOutput->load(pNode, aCollection))
    {
      return false;
    }
  }

  return false;
}

//---------------------------------------------getInput
ModuleInputBase* Module::getInput(tSize i)
{
  CLAY_ASSERT(i < getNumInputs());
  return m_collInputs[i];
}

//---------------------------------------------getInput
ModuleInputBase*  Module::getInput(const tString& sInputName)
{
  tInputCollection::iterator it  = m_collInputs.begin();
  tInputCollection::iterator end = m_collInputs.end();
  for(; it!=end; ++it)
  {
    ModuleInputBase* pInput = *it;
    if(pInput->getName() == sInputName)
    { 
      return pInput;
    }
  }
  return NULL;
}

//---------------------------------------------getOutput
ModuleOutputBase* Module::getOutput(tSize i)
{
  CLAY_ASSERT(i < getNumOutputs());
  return m_collOutputs[i];
}

//---------------------------------------------getOutput
ModuleOutputBase* Module::getOutput(const tString& sOutputName)
{
  tOutputCollection::iterator it  = m_collOutputs.begin();
  tOutputCollection::iterator end = m_collOutputs.end();
  for(; it!=end; ++it)
  {
    ModuleOutputBase* pOutput = *it;
    if(pOutput->getName() == sOutputName)
    { 
      return pOutput;
    }
  }
  return NULL;
}

//---------------------------------------------duplicateModuleOutput
ModuleOutputBase* Module::duplicateModuleOutput(ModuleOutputBase* pOriginal)
{
  CLAY_ASSERT(pOriginal->getParentModule() == this);

  ModuleOutputBase* pProxy = pOriginal->createProxy();
  if(registerOutput(pProxy, pOriginal->getName() + LexicalConversion::toString(getNumOutputs())))
  {
    return pProxy;
  }

  delete pProxy;
  return NULL;
}

//---------------------------------------------connectFrom
Module::ConnectResultCode Module::connectFrom(Module* pPredecessor, ConnectionStrategy eConnectionStrategy)
{
  CLAY_ASSERT(pPredecessor);
  if(!pPredecessor)
  {
    return Module::Connect_ERROR;
  }

  bool bConnectionAvailable = false;
  if(eConnectionStrategy == CLEVER_CONNECT_MODE || 
      eConnectionStrategy == TEST_CLEVER_CONNECT_MODE)
  {
    tInputCollection::iterator inIt  = m_collInputs.begin();
    tInputCollection::iterator inEnd = m_collInputs.end();
    for(; inIt!=inEnd; ++inIt)
    {
      ModuleInputBase* pCurrentInput = *inIt;
      for(unsigned int i=0, n=pPredecessor->getNumOutputs(); (i<n) && (!pCurrentInput->isConnected()); ++i)
      {
        ModuleOutputBase* pCurrentOutput = pPredecessor->getOutput(i);
        if(!pCurrentOutput->isConnected()) //try to connect if not already connected
        {
          if(pCurrentOutput->canConnect(pCurrentInput) && pCurrentInput->canConnect(pCurrentOutput))
          {
            if(eConnectionStrategy == CLEVER_CONNECT_MODE)
            {
              //establish a bidirectional connection - compatibility already checked
              if(pCurrentOutput->connect(pCurrentInput,  false) &&
                  pCurrentInput->connect(pCurrentOutput, false))
              {
                signalModuleOutputConnected.emit1(pCurrentOutput);
                signalModuleInputConnected.emit1(pCurrentInput);
              }
            }
            bConnectionAvailable = true;
          }
        }
      }
    }
  }
  else
  {
    CLAY_FAIL(); //strategy not implemented
  }

  if(bConnectionAvailable)
  {
    return Module::Connect_OK;
  }
  return Module::NoSuitableConnection_ERROR;
}

//---------------------------------------------connectFrom
Module::ConnectResultCode Module::connectFrom(const tString& sInputname, Module* pPredecessor, const tString& sOutputname)
{
  CLAY_ASSERT(pPredecessor);
  if(!pPredecessor)
  {
    return Module::Connect_ERROR;
  }

  ModuleOutputBase* pOutput = pPredecessor->getOutput(sOutputname);
  ModuleInputBase*  pInput  = getInput(sInputname);
  if(pOutput && pInput)
  {
    if(pOutput->connect(pInput,  false) &&
        pInput->connect(pOutput, false))
    {
      signalModuleOutputConnected.emit1(pOutput);
      signalModuleInputConnected.emit1(pInput);
      
      return Module::Connect_OK;
    }
  }

  CLAY_FAIL();
  return Module::Connect_ERROR;
}

//---------------------------------------------shapeProcess
Module::CompileResultCode Module::shapeProcess(ClayShaper* pCompiler, ClayExecutable& aTarget)
{
  aTarget.appendMethod(boost::bind(&Module::process, this), this);
  return Module::Compile_OK;
}

//---------------------------------------------process
Module::ProcessResultCode Module::process()
{
  return Module::ProcessNotImplemented_ERROR;
}

//---------------------------------------------canConnectFrom
bool Module::canConnectFrom(Module* pPredecessor)
{
  return connectFrom(pPredecessor, TEST_CLEVER_CONNECT_MODE) == Connect_OK; //test if connection is possible
}

//---------------------------------------------disconnect
void Module::disconnect()
{  
  tOutputCollection::iterator it  = m_collOutputs.begin();
  tOutputCollection::iterator end = m_collOutputs.end();
  for(; it!=end; ++it)
  {
    ModuleOutputBase* pOutput = *it;
    disconnect(pOutput);
  }
  
  tInputCollection::iterator ti  = m_collInputs.begin();
  tInputCollection::iterator dne = m_collInputs.end();
  for(; ti!=dne; ++ti)
  {
    ModuleInputBase* pInput = *ti;
    disconnect(pInput);
  }
}

//---------------------------------------------disconnect
void Module::disconnect(ModuleOutputBase* pOutput)
{
  for(unsigned int i=0, n=pOutput->getNumConnections(); i<n; ++i)
  {
    ModuleInputBase* pOpponentInput = pOutput->getOpponent(i);
    Module* pOpponentModule = pOpponentInput->getParentModule();
    CLAY_ASSERT(pOpponentInput);
    CLAY_ASSERT(pOpponentModule);
    pOpponentInput->disconnect();
    pOpponentModule->onInputDisconnected(pOpponentInput);
  }    

  pOutput->disconnect();
  onOutputDisconnected(pOutput);
}

//---------------------------------------------disconnect
void Module::disconnect(ModuleInputBase* pInput)
{
  if(pInput->isConnected())
  {
    ModuleOutputBase* pOpponentOutput = pInput->getOpponent();
    Module* pOpponentModule = pInput->getSourceModule();
    CLAY_ASSERT(pOpponentModule);
    CLAY_ASSERT(pOpponentOutput);
    pOpponentOutput->disconnect(pInput);
    pOpponentModule->onOutputDisconnected(pOpponentOutput);

    pInput->disconnect();
    onInputDisconnected(pInput);
  }
}

//---------------------------------------------onInputDisconnected
void Module::onInputDisconnected(ModuleInputBase* pInput)
{
  signalModuleInputDisconnected.emit1(pInput);
  //derived classes can react on this action by overloading this method
}

//---------------------------------------------onOutputDisconnected
void Module::onOutputDisconnected(ModuleOutputBase* pOutput)
{
  signalModuleOutputDisconnected.emit1(pOutput);
  //derived classes can react on this action by overloading this method
}

//---------------------------------------------registerInput
bool Module::registerInput(ModuleInputBase* pInput, const tString& sName)
{
  if(std::find(m_collInputs.begin(), m_collInputs.end(), pInput) == m_collInputs.end())
  {
    pInput->setParentModule(this);
    pInput->setName(sName);
    m_collInputs.push_back(pInput);
    signalModuleInputRegistered.emit1(pInput);
    return true;
  }
  return false;
}

//---------------------------------------------registerOutput
bool Module::registerOutput(ModuleOutputBase* pOutput, const tString& sName)
{
  if(std::find(m_collOutputs.begin(), m_collOutputs.end(), pOutput) == m_collOutputs.end())
  {
    pOutput->setParentModule(this);
    pOutput->setName(sName);
    m_collOutputs.push_back(pOutput);
    signalModuleOutputRegistered.emit1(pOutput);
    return true;
  }
  return false;
}

//---------------------------------------------unregisterInput
bool Module::unregisterInput(ModuleInputBase* pInput, bool bDelete)
{
  tInputCollection::iterator pos = std::find(m_collInputs.begin(), m_collInputs.end(), pInput);
  if(pos != m_collInputs.end())
  {
    disconnect(pInput);

    m_collInputs.erase(pos);
    signalModuleInputUnregistered.emit1(pInput);

    if(bDelete)
    {
      delete pInput;
    }      
    return true;
  }
  return false;
}

//---------------------------------------------unregisterOutput
bool Module::unregisterOutput(ModuleOutputBase* pOutput, bool bDelete)
{
  tOutputCollection::iterator pos = std::find(m_collOutputs.begin(), m_collOutputs.end(), pOutput);
  if(pos != m_collOutputs.end())
  {
    disconnect(pOutput);

    m_collOutputs.erase(pos);
    signalModuleOutputUnregistered.emit1(pOutput);
      
    if(bDelete)
    {
      delete pOutput;
    }
    return true;
  }
  return false;
}

//---------------------------------------------unregisterInputs
bool Module::unregisterInputs(bool bDelete)
{
  bool bAccumulatedSuccess = true;
  tInputCollection::iterator it  = m_collInputs.begin();
  tInputCollection::iterator end = m_collInputs.end();
  while(m_collInputs.size())
  {
    if(!unregisterInput(*m_collInputs.begin(), bDelete))
    {
      bAccumulatedSuccess = false;
    }
  }
  return bAccumulatedSuccess;
}

//---------------------------------------------unregisterOutputs
bool Module::unregisterOutputs(bool bDelete)
{
  bool bAccumulatedSuccess = true;
  tOutputCollection::iterator it  = m_collOutputs.begin();
  tOutputCollection::iterator end = m_collOutputs.end();
  while(m_collOutputs.size())
  {
    if(!unregisterOutput(*m_collOutputs.begin(), bDelete))
    {
      bAccumulatedSuccess = false;
    }
  }
  return bAccumulatedSuccess;
}

//---------------------------------------------getInputConnectionsFrom
void Module::getInputConnectionsFrom(Module* pOther, Module::tInputCollection& collDst)
{
  tInputCollection::iterator it  = m_collInputs.begin();
  tInputCollection::iterator end = m_collInputs.end();
  for(; it!=end; ++it)
  {
    ModuleInputBase* pCurrent = *it;
    if(pCurrent->isConnected())
    {
      if(pCurrent->getOpponent()->getParentModule() == pOther)
      {
        collDst.push_back(pCurrent);
      }
    }
  }
}

//---------------------------------------------getOutputConnectionsTo
void Module::getOutputConnectionsTo(Module* pOther, Module::tOutputCollection& collDst)
{
  tOutputCollection::iterator it  = m_collOutputs.begin();
  tOutputCollection::iterator end = m_collOutputs.end();
  for(; it!=end; ++it)
  {
    ModuleOutputBase* pCurrent = *it;
    for(unsigned int i=0, n=pCurrent->getNumConnections(); i<n; ++i)
    {
      if(pCurrent->getOpponent(i)->getParentModule() == pOther)
      {
        collDst.push_back(pCurrent);
      }
    }
  }
}

//---------------------------------------------setModuleState
void Module::setModuleState(ModuleState eModuleState)
{
  bool bAlreadySet = false;
  if(m_uState & eModuleState)
  {
    bAlreadySet = true;
  }
  if(!bAlreadySet)
  {
    m_uState |= eModuleState;
    if(eModuleState == MODULE_INITIALIZED)
    {
      signalModuleInitialized.emit0();
    }
    else if(eModuleState == MODULE_ACTIVE)
    {
      signalModuleActive.emit0();
    }
    else 
    {
      CLAY_FAIL();
      return;
    }
  }
}

//---------------------------------------------setModuleState
void Module::setModuleState(NegativeModuleState eModuleState)
{
  bool bAlreadyUnset = !(m_uState & eModuleState);
  if(!bAlreadyUnset)
  {
    m_uState &= ~eModuleState;
    if(eModuleState == MODULE_UNINITIALIZED)
    {
      signalModuleUnitialized.emit0();
    }
    else if(eModuleState == MODULE_INACTIVE)
    {
      signalModuleInactive.emit0();
    }
    else 
    {
      CLAY_FAIL();
      return;
    }
  }
}

//---------------------------------------------resetModuleState
void Module::resetModuleState()
{
  m_uState = 0;
}

//---------------------------------------------isInitialized
bool Module::isInitialized() const
{ 
  if(m_uState & MODULE_INITIALIZED)
  {
    return true;
  }
  return false;
}

//---------------------------------------------isActive
bool Module::isActive() const
{ 
  if(m_uState & MODULE_ACTIVE)
  {
    return true;
  }
  return false;
}

}
