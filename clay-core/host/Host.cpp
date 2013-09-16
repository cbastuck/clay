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

#include "Host.h"

#include <clay-core/base/Module.h>
#include <clay-core/base/ClayRuntime.h>
#include <clay-core/base/ClayExecutable.h>
#include <clay-core/base/ClayShaper.h>
#include <clay-core/registry/ModuleRegistry.h>
#include <clay-core/nugget/ClayNuggetLoader.h>

#include <boost/bind.hpp>

#define XML_HOST_ROOT_NODE         "host-root"
#define XML_HOST_MODULES_NODE      "hosted-modules"
#define XML_HOST_MODULE_NODE       "hosted-module"
#define XML_HOST_MODULE_ATTR_URI   "hosted-module-id"
#define XML_HOST_MODULE_ATTR_RT_ID "hosted-module-rt-id"
#define XML_HOST_PRIORITY_LIST     "priority-list"
#define XML_HOSTED_NUGGETS_NODE    "hosted-nuggets"
#define XML_NUM_NUGGETS_ATTR       "num-hosted-nuggets"
#define XML_HOSTED_NUGGET_NODE     "hosted-nugget"
#define XML_HOSTED_NUGGET_ATTR     "filename"

namespace CLAY{

//---------------------------------------------Host
Host::Host(ModuleRegistry* pRegistry)
  : m_pRegistry(pRegistry),
    m_pNuggetLoader(NULL)
{

}

//---------------------------------------------~Host
Host::~Host()
{
  deInit();
}

//---------------------------------------------init
bool Host::init()
{
  CLAY_ASSERT(!m_pNuggetLoader);
  m_pNuggetLoader = new NUGGET::ClayNuggetLoader();
  m_pNuggetLoader->init();
  return true;
}

//---------------------------------------------deInit
void Host::deInit()
{
  tModuleCollection::iterator it  = m_collModules.begin();
  tModuleCollection::iterator end = m_collModules.end();
  for(; it!=end; ++it)
  {
    destroyModule(*it);
  }
  m_collModules.clear();

  if(m_pNuggetLoader)
  {
    m_pNuggetLoader->deInit();
    delete m_pNuggetLoader;
    m_pNuggetLoader = NULL;
  }
}

//---------------------------------------------connect
bool Host::connect(const tString& sLhsMod, const tString& sRhsMod)
{
  Module* pLhsMod = getModule(sLhsMod);
  Module* pRhsMod = getModule(sRhsMod);
  if(pLhsMod && pRhsMod)
    return connect(pLhsMod, pRhsMod);
  return false;
}

//---------------------------------------------connect
bool Host::connect(Module* pFrom, Module* pTo)
{
  Module::ConnectResultCode eResult = pTo->connectFrom(pFrom);
  if(eResult == CLAY::Module::Connect_OK)
  {
    signalConnectionEstablished.emit2(pFrom, pTo);
    return true;
  }
  return false;
}

//---------------------------------------------connect
bool Host::connect(Module* pFrom, ModuleOutputBase* pFromOut, Module* pTo, ModuleInputBase* pToIn, bool bCheckCompatibility)
{
  /*
  if(pFromOut->isConnected())
  {
    pFromOut = pFrom->duplicateModuleOutput(pFromOut);
  }
  */

  if(pFromOut->connect(pToIn, bCheckCompatibility))
  {
    if(pToIn->connect(pFromOut, bCheckCompatibility))
    {
      signalConnectionEstablished.emit2(pFrom, pTo);
      return true;
    }
    CLAY_FAIL();
  }

  //TODO: remove duplicate as connection could not be established
  return false;
}

//---------------------------------------------saveProject
bool Host::saveProject(const tString& sFilename)
{
  XERCES::DOMDocument* pDoc = XercesXML::createDOMDoc(XML_HOST_ROOT_NODE);
  XERCES::DOMElement* pRoot = pDoc->getDocumentElement();
  if(saveProject(pRoot))
  {
    return XercesXML::saveDOMElement(pRoot, sFilename.c_str());
  }
  return false;    
}

//---------------------------------------------saveProject
bool Host::saveProject(XERCES::DOMElement* pNode)
{
  CLAY_ASSERT(m_pNuggetLoader);

  //store all loaded nuggets
  XERCES::DOMElement* pHostedNuggetsNode = XercesXML::appendNode(pNode, XML_HOSTED_NUGGETS_NODE);
  unsigned int uNumNuggets = m_pNuggetLoader->getNumLoadedNuggets();
  XercesXML::addAttribute(pHostedNuggetsNode, XML_NUM_NUGGETS_ATTR, (int)uNumNuggets);
  for(unsigned int i=0; i<uNumNuggets; ++i)
  {
    const tString& sCurrentFn = m_pNuggetLoader->getNuggetFilename(i);
    XERCES::DOMElement* pNuggetNode = XercesXML::appendNode(pHostedNuggetsNode, XML_HOSTED_NUGGET_NODE);
    XercesXML::addAttribute(pNuggetNode, XML_HOSTED_NUGGET_ATTR, sCurrentFn);
  }

  //store all hosted modules
  XERCES::DOMElement* pModulesNode = XercesXML::appendNode(pNode, XML_HOST_MODULES_NODE);
  tModuleCollection::const_iterator it  = m_collModules.begin();
  tModuleCollection::const_iterator end = m_collModules.end();
  for(; it!=end; ++it)
  {
    Module* pCurrentModule = *it;
    if(!saveModule(pModulesNode, pCurrentModule)) //give derived classes the opportunity to save custom stuff by calling virtual function
    {
      return false;
    }
  }

  //save priority list
  XERCES::DOMElement* pPriorityListNode = XercesXML::appendNode(pNode, XML_HOST_PRIORITY_LIST);
  {
    tModuleCollection::const_iterator it = m_aPriorityModules.begin();
    tModuleCollection::const_iterator ed = m_aPriorityModules.end();
    for(; it!=ed; ++it)
    {
      Module* pModule = *it;
      XERCES::DOMElement* pPriorityNode = XercesXML::appendNode(pPriorityListNode, "priority-module");
      XercesXML::addAttribute(pPriorityNode, "module-id", pModule->getRuntimeModuleID());
    }
  }

  return true;
}

//---------------------------------------------saveModule
bool Host::saveModule(XERCES::DOMElement* pParent, Module* pCurrentModule)
{
  XERCES::DOMElement* pModuleNode = XercesXML::appendNode(pParent, XML_HOST_MODULE_NODE);
  XercesXML::addAttribute(pModuleNode, XML_HOST_MODULE_ATTR_URI,   pCurrentModule->getModuleURI());
  XercesXML::addAttribute(pModuleNode, XML_HOST_MODULE_ATTR_RT_ID, pCurrentModule->getRuntimeModuleID());

  if(!pCurrentModule->save(pModuleNode)) //now save the actual module
  {
    return false;
  }
  return true;
}

//---------------------------------------------loadProject
bool Host::loadProject(const tString& sFilename)
{
  XERCES::DOMDocument* pDoc = XercesXML::loadXML(sFilename);
  if(pDoc)
  {
    bool bSuccess = false;
    XERCES::DOMElement* pRoot = pDoc->getDocumentElement();
    if(pRoot)
    {
      bSuccess = loadProject(pRoot);
    }
    delete pDoc;

    return bSuccess;
  }
  return false;
}

//---------------------------------------------loadProject
bool Host::loadProject(XERCES::DOMElement* pNode)
{
  if(!m_pRegistry) //module registry needed for restoring
  {
    return false;
  }

  //restore nuggets
  XERCES::DOMElement* pHostedNuggetsNode = XercesXML::findChildNode(pNode, XML_HOSTED_NUGGETS_NODE);
  if(pHostedNuggetsNode)
  {
    XercesXML::visitChildNodes(pHostedNuggetsNode, boost::bind(&Host::restoreNugget, this, _1));
  }

  //restore modules
  XERCES::DOMElement* pModulesNode = XercesXML::findChildNode(pNode, XML_HOST_MODULES_NODE, true);
  if(pModulesNode)
  {
    Module::tConnectionMap collInputConnections;
    Module::tConnectionMap collOutputConnections;
    //call virtual load method to give derived class the opportunity to add read custom information
    XercesXML::visitChildNodes(pModulesNode, boost::bind(&Host::loadModule, this, _1, &collInputConnections, &collOutputConnections));
    
    //restore connections
    if(!restoreInputConnections (collInputConnections))
    {
      CLAY_FAIL();
      return false;
    }
    if(!restoreOutputConnections(collOutputConnections))
    {
      CLAY_FAIL();
      return false;
    }
  }

  //restore priority list
  XERCES::DOMElement* pPriorityListNode = XercesXML::findChildNode(pNode, XML_HOST_PRIORITY_LIST);
  if(pPriorityListNode)
  {
    XercesXML::visitChildNodes(pPriorityListNode, boost::bind(&Host::restorePriorityModule, this, _1));
  }

  return true;
}

//---------------------------------------------loadModule
Module* Host::loadModule(XERCES::DOMElement* pNode, Module::tConnectionMap* pInputConnections, Module::tConnectionMap* pOutputConnections)
{
  CLAY_ASSERT(m_pRegistry);

  if(XercesXML::nodeHasName(pNode, XML_HOST_MODULE_NODE))
  {
    tString sModuleURI;
    if(!XercesXML::getAttributeValue(pNode, XML_HOST_MODULE_ATTR_URI, sModuleURI))
    {
      return false;
    }

    tString sRuntimeModuleId;
    if(!XercesXML::getAttributeValue(pNode, XML_HOST_MODULE_ATTR_RT_ID, sRuntimeModuleId))
    {
      return false;
    }
      
    Module* pModule = createModule(sModuleURI.c_str(), 
                                   sRuntimeModuleId, 
                                   pNode->getFirstChild(), 
                                   true);
    if(pModule)
    {
      if(!pModule->load(pNode, pInputConnections, pOutputConnections))
      {
        CLAY_FAIL();
        delete pModule; //something went wrong, destroy the module and return NULL
        pModule = NULL;
      }
      return pModule;
    }
  }
  return NULL;
}

//---------------------------------------------closeProject
void Host::closeProject()
{
  tModuleCollection::iterator it  = m_collModules.begin();
  tModuleCollection::iterator end = m_collModules.end();
  for(; it!=end; ++it)
  {
    Module* pModule = *it;
    removeModule(pModule, false);
  }
  m_collModules.clear();
}

//---------------------------------------------createModule
Module* Host::createModule(const char* sModuleURI,
                           const tString& sRuntimeModuleId, 
                           XERCES::DOMNode* pConfigNode,
                           bool bAddToManagedModules)
{
  Module* pModule = m_pRegistry->createItem(sModuleURI, sRuntimeModuleId);
  if(!pModule)
  {
    return NULL;
  }

  if(pConfigNode)
  {
    pModule->init(pConfigNode); //TODO: invoke load instead of init (RAII)
  }

  if(bAddToManagedModules)
  {
    addModule(pModule);
  }

  return pModule;
}

//---------------------------------------------removeModule
void Host::removeModule(Module* pModule, bool bRemoveFromManagedModules)
{
  CLAY_ASSERT(pModule);
  tModuleCollection::iterator pos = std::find(m_collModules.begin(), m_collModules.end(), pModule);
  if(pos != m_collModules.end())
  {
    if(bRemoveFromManagedModules)
    {
      m_collModules.erase(pos);
    }

    pModule->deInit();

    m_pRegistry->destroyItem(pModule);
  }
  else
  {
    CLAY_FAIL();
  }
}
  
//---------------------------------------------getModule
Module* Host::getModule(const tString& sRuntimeModuleId) const
{
  tModuleCollection::const_iterator it  = m_collModules.begin();
  tModuleCollection::const_iterator end = m_collModules.end();
  for(; it!=end; ++it)
  {
    if((*it)->getRuntimeModuleID() == sRuntimeModuleId)
      return *it;
  }
  return NULL;
}

//---------------------------------------------addPriorityModule
bool Host::addPriorityModule(Module* pModule)
{
  if(std::find(m_aPriorityModules.begin(), m_aPriorityModules.end(), pModule) == m_aPriorityModules.end())
  {
    m_aPriorityModules.push_back(pModule);
    return true;
  }
  return false;
}

//---------------------------------------------clearPriorityList
void Host::clearPriorityList()
{
  m_aPriorityModules.clear();
}

//---------------------------------------------play
Module::ProcessResultCode Host::play()
{
  if(m_collModules.size())
  {
    CLAY::ClayShaper aShaper(&m_aPriorityModules);

    CLAY::ClayExecutable aProgram;

    aShaper.shapeProcess(m_collModules, aProgram);

    CLAY::ClayRuntime aRuntime;
    return aRuntime.execute(aProgram);
  }
  return CLAY::Module::Process_ERROR;
}

//---------------------------------------------loadNugget
NUGGET::IClayNugget* Host::loadNugget(const tString& sNuggetFn)
{
  CLAY_ASSERT(m_pNuggetLoader);
  NUGGET::IClayNugget* pNugget = m_pNuggetLoader->loadNugget(sNuggetFn);
  if(pNugget)
  {
    if(m_pRegistry->registerModules(pNugget))
    {
      signalClayNuggetLoaded.emit1(pNugget);
    }
  }
  return pNugget;
}

//---------------------------------------------restoreNugget
NUGGET::IClayNugget* Host::restoreNugget(XERCES::DOMElement* pNode)
{
  tString sNuggetFn;
  if(XercesXML::getAttributeValue(pNode, XML_HOSTED_NUGGET_ATTR, sNuggetFn))
  {
    NUGGET::IClayNugget* pNugget = loadNugget(sNuggetFn);
    if(pNugget)
    {
      signalClayNuggetLoaded.emit1(pNugget);
    }
  }
  return  NULL;   
}

//---------------------------------------------restoreInputConnections
bool Host::restoreInputConnections(const Module::tConnectionMap& collInputConnections)
{
  Module::tConnectionMap::const_iterator it  = collInputConnections.begin();
  Module::tConnectionMap::const_iterator end = collInputConnections.end();
  for(; it!=end; ++it)
  {
    Module* pModule = it->first;
    const Module::tConnectionColl& aCollection = it->second;

    Module::tConnectionColl::const_iterator ti = aCollection.begin();
    Module::tConnectionColl::const_iterator de = aCollection.end();
    for(; ti!=de; ++ti)
    {
      const ModuleConnectionDescriptor& aConnection = *ti;

      Module* pOpponent = getModule(aConnection.outputModule);
      if(pOpponent)
      {
        Module::ConnectResultCode aRes = pModule->connectFrom(aConnection.inputName, pOpponent, aConnection.outputName);
        if(aRes != Module::Connect_OK)
        {
          CLAY_FAIL();
          return false;
        }
      }
    }
  }
  return true;
}

//---------------------------------------------restoreOutputConnections
bool Host::restoreOutputConnections(const Module::tConnectionMap& collOutputConnections)
{
  Module::tConnectionMap::const_iterator it  = collOutputConnections.begin();
  Module::tConnectionMap::const_iterator end = collOutputConnections.end();
  for(; it!=end; ++it)
  {
    Module* pModule = it->first;
    const Module::tConnectionColl& aCollection = it->second;

    Module::tConnectionColl::const_iterator ti = aCollection.begin();
    Module::tConnectionColl::const_iterator de = aCollection.end();
    for(; ti!=de; ++ti)
    {
      const ModuleConnectionDescriptor& aConnection = *ti;
    
      Module* pOpponent = getModule(aConnection.inputModule);
      if(pModule && pOpponent)
      {
        pOpponent->connectFrom(pModule); //TODO: connect only the given in- and outputs - no clever connect here!!
      }
      else
      {
        CLAY_FAIL();
        return false;
      }
    }
  }
  return true;
}

//---------------------------------------------restorePriorityModule
bool Host::restorePriorityModule(XERCES::DOMElement* pNode)
{
  tString sModuleID;
  if(!XercesXML::getAttributeValue(pNode, "module-id", sModuleID))
  {
    return false;
  }
  
  Module* pModule = getModule(sModuleID);
  if(!pModule)
  {
    CLAY_FAIL();
    return false;
  }

  return addPriorityModule(pModule);
}

//---------------------------------------------destroyModule
void Host::destroyModule(Module* pModule)
{
  m_pRegistry->destroyItem(pModule);
}

//---------------------------------------------addModule
void Host::addModule(Module* pModule)
{
  m_collModules.push_back(pModule);
}

}
