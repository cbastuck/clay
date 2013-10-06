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

#include "HostModule.h"

#include <clay-core/base/ClayShaper.h>
#include <clay-core/base/ClayRuntime.h>
#include <clay-modules/StandardRegistry.h>
#include <clay-core/base/ClayExecutable.h>

namespace CLAY{ namespace MODULE{

//---------------------------------------------
const char* HostModule::staticModuleURI()
{
  return "http://claymodules.org/host";
}

//---------------------------------------------HostModule
HostModule::HostModule(const tString& sId) 
  : Module(sId),
    Host(new ModuleRegistry),
    m_bModuleRegistryOwner(true),
    m_pRuntimeExecutable(NULL),
    m_pPendingExecutable(NULL),
    m_pGarbageExectuable(NULL)
{

}

//---------------------------------------------HostModule
HostModule::HostModule(const tString& sId, ModuleRegistry* pModuleRegistry)
  : Module(sId),
    Host(pModuleRegistry),
    m_bModuleRegistryOwner(false),
    m_pRuntimeExecutable(NULL),
    m_pPendingExecutable(NULL),
    m_pGarbageExectuable(NULL)
{
}

//---------------------------------------------~HostModule
HostModule::~HostModule()
{
  if(m_bModuleRegistryOwner)
  {
    delete getRegistry();
  }
}

//---------------------------------------------
const char* HostModule::getModuleURI() const
{
  return staticModuleURI();
}

//---------------------------------------------init
bool HostModule::init(XERCES::DOMNode* pNode)
{
  if(!Host::init())
  {
    return false;
  }

  if(!Module::init(pNode))
  {
      return false;
  }

  registerModuleInputs();
  registerModuleOutputs();

  return true;
}

//---------------------------------------------deInit
void HostModule::deInit()
{
  closeProject();
  Host::deInit();
  Module::deInit();
}

//---------------------------------------------save
bool HostModule::save(XERCES::DOMElement* pNode)
{
  XERCES::DOMElement* pHostModuleNode = XercesXML::appendNode(pNode, "host-module");
  XERCES::DOMElement* pHostBaseNode   = XercesXML::appendNode(pHostModuleNode, "host-base");

  if(!Host::saveProject(pHostBaseNode))
  {
    return false;
  }

  XERCES::DOMElement* pModuleBaseNode = XercesXML::appendNode(pHostModuleNode, "module-base");
  if(!Module::save(pModuleBaseNode))
  {
    return false;
  }

  XERCES::DOMElement* pModuleSpecNode = XercesXML::appendNode(pHostModuleNode, "module-specific");
  
  //save exported outputs
  XERCES::DOMElement* pOutputsNode = XercesXML::appendNode(pModuleSpecNode, "exported-outputs");
  {
    tOutputIterator it = beginOutput();
    tOutputIterator ed = endOutput();
    for(; it !=ed; ++it)
    {
      ModuleOutputBase* pOutput = *it;

      tExportedOutOwnerMap::const_iterator pos = m_aOutputOwnerMap.find(pOutput);
      if(pos == m_aOutputOwnerMap.end())
      {
        CLAY_FAIL();
        return false;
      }

      XERCES::DOMElement* pOutputNode = XercesXML::appendNode(pOutputsNode, "exported-output");
      XercesXML::addAttribute(pOutputNode, "module-id", pos->second->getRuntimeModuleID());
      XercesXML::addAttribute(pOutputNode, "output-id", pOutput->getName());
    }
  }

  //save exported inputs
  XERCES::DOMElement* pInputsNode = XercesXML::appendNode(pModuleSpecNode, "exported-inputs");
  {
    tInputIterator ti = beginInput();
    tInputIterator de = endInput();
    for(; ti !=de; ++ti)
    {
      ModuleInputBase* pInput = *ti;

      tExportedInOwnerMap::const_iterator pos = m_aInputOwnerMap.find(pInput);
      if(pos == m_aInputOwnerMap.end())
      {
        CLAY_FAIL();
        return false;
      }

      XERCES::DOMElement* pInputNode = XercesXML::appendNode(pInputsNode, "exported-input");
      XercesXML::addAttribute(pInputNode, "module-id", pos->second->getRuntimeModuleID());
      XercesXML::addAttribute(pInputNode, "input-id", pInput->getName());
    }
  }

  return true;
}

//---------------------------------------------load
bool HostModule::load(XERCES::DOMElement* pNode, tConnectionMap* pInputConnections, tConnectionMap* pOutputConnections)
{
  XERCES::DOMElement* pHostModuleNode = XercesXML::findChildNode(pNode, "host-module", true);
  if(!pHostModuleNode)
  {
    return false;
  }

  XERCES::DOMElement* pHostBaseNode = XercesXML::findChildNode(pHostModuleNode, "host-base");
  if(!pHostBaseNode)
  {
    return false;
  }

  if(!Host::loadProject(pHostBaseNode))
  {
    return false;
  }

  XERCES::DOMElement* pModuleBaseNode = XercesXML::findChildNode(pHostModuleNode, "module-base");
  if(!pModuleBaseNode)
  {
    return false;
  }

  if(!Module::load(pModuleBaseNode, pInputConnections, pOutputConnections))
  {
    return false;
  }

  XERCES::DOMElement* pModuleSpecNode = XercesXML::findChildNode(pHostModuleNode, "module-specific");
  if(!pModuleSpecNode)
  {
    return false;
  }

  //restore outputs
  XERCES::DOMElement* pOutputNode = XercesXML::findChildNode(pModuleSpecNode, "exported-outputs");
  if(!pOutputNode)
  {
    return false;
  }
  XercesXML::visitChildNodes(pOutputNode, boost::bind(&HostModule::restoreOutput, this, _1));

  //restore inputs
  XERCES::DOMElement* pInputNode = XercesXML::findChildNode(pModuleSpecNode, "exported-inputs");
  if(!pInputNode)
  {
    return false;
  }
  XercesXML::visitChildNodes(pInputNode, boost::bind(&HostModule::restoreInput, this, _1));

  return true;
}

//---------------------------------------------shapeProcess
Module::CompileResultCode HostModule::shapeProcess(ClayShaper* pShaper, ClayExecutable& aTarget)
{
  shapeProcess();
  return Module::shapeProcess(pShaper, aTarget);
}

//---------------------------------------------process
Module::ProcessResultCode HostModule::process()
{
  if(m_aRuntimeLock.try_lock()) //lock when entering process()
  {
    if(m_pPendingExecutable)
    {
        if(m_aPendingExecutableLock.try_lock())
        {
          CLAY_ASSERT(!m_pGarbageExectuable);
          std::swap(m_pPendingExecutable, m_pRuntimeExecutable);
          std::swap(m_pGarbageExectuable, m_pPendingExecutable);
          m_aPendingExecutableLock.unlock();
        }
    }

    Module::ProcessResultCode aRes = Module::Process_OK;
    if(m_pRuntimeExecutable)
    {
      ClayRuntime aRuntime;
      Module::ProcessResultCode aRes = aRuntime.execute(*m_pRuntimeExecutable);
      CLAY_ASSERT(aRes == Module::Process_OK);
    }
    m_aRuntimeLock.unlock(); //unock when leaving process()
    return aRes; 
  }
  return Module::Process_NotPossible; //is locked - bug in closing the project
}

//---------------------------------------------closeProject
void HostModule::closeProject()
{
  m_aPendingExecutableLock.lock();

  m_aRuntimeLock.lock();
  ClayExecutable* pBack = m_pRuntimeExecutable;
  m_pRuntimeExecutable  = NULL; //stops process()
  m_aRuntimeLock.unlock();
  delete pBack; //now we can savely delete the runtime executable

  if(m_pPendingExecutable)
  {
    delete m_pPendingExecutable;
    m_pPendingExecutable = NULL;
  }

  if(m_pGarbageExectuable)
  {
    delete m_pGarbageExectuable;
    m_pGarbageExectuable = NULL;
  }

  m_aPendingExecutableLock.unlock();

  Host::closeProject();
}

//---------------------------------------------save
bool HostModule::save(const tString& sFn)
{
  XERCES::DOMDocument* pDoc = XercesXML::createDOMDoc("host-module-root");
  XERCES::DOMElement* pRoot = pDoc->getDocumentElement();
  if(!save(pRoot))
  {
    return false;
  }

  return XercesXML::saveDOMElement(pRoot, sFn.c_str());
}

//---------------------------------------------load
bool HostModule::load(const tString& sFn, tConnectionMap* pInputConnections, tConnectionMap* pOutputConnections)
{
  XERCES::DOMDocument* pDoc = XercesXML::loadXML(sFn);
  if(!pDoc)
  {
    return false;
  }

  XERCES::DOMElement* pRoot = pDoc->getDocumentElement();
  if(!pRoot)
  {
    return false;
  }
  
  return load(pRoot, pInputConnections, pOutputConnections);
}

//---------------------------------------------shapeProcess
void HostModule::shapeProcess()
{
  m_aPendingExecutableLock.lock();

  if(m_pGarbageExectuable)
  {
    delete m_pGarbageExectuable; //we created it before
    m_pGarbageExectuable = NULL;
  }

  if(m_pPendingExecutable)
  {
    delete m_pPendingExecutable;
    m_pPendingExecutable = NULL;
  }
  m_pPendingExecutable = new ClayExecutable();
      
  ClayShaper aShaper(&getPriorityModules());
  declareExternalModules(aShaper);
  Module::CompileResultCode aErr = aShaper.shapeProcess(getHostedModules(), *m_pPendingExecutable);
  CLAY_ASSERT(aErr != Module::Compile_ERROR);

  m_aPendingExecutableLock.unlock();
}

//---------------------------------------------exportModuleOutput
bool HostModule::exportModuleOutput(Module* pHostedModule, const tString& sOutputName)
{
  ModuleOutputBase* pExportedOutput = pHostedModule->getOutput(sOutputName);
  if(pExportedOutput)
  {
    return exportModuleOutput(pExportedOutput, &sOutputName);
  }
  return false;  
}

//---------------------------------------------exportModuleOutput
bool HostModule::exportModuleOutput(ModuleOutputBase* pOutput, const tString* pExportedName)
{
  tString sOutputName = pOutput->getName(); //this is the default name
  if(pExportedName)
  {
    sOutputName = *pExportedName;
  }

  ModuleOutputBase* pProxy  = pOutput->createProxy();
  if(pProxy)
  {
    m_aOutputOwnerMap[pProxy] = pOutput->getParentModule();
    return registerOutput(pProxy, sOutputName);
  }
  return false;
}

//---------------------------------------------exportModuleInput
bool HostModule::exportModuleInput(Module* pHostedModule, const tString& sInputName)
{
  ModuleInputBase* pExportedInput = pHostedModule->getInput(sInputName);
  if(pExportedInput)
  {
    return exportModuleInput(pExportedInput, &sInputName);
  }
  return false; 
}

//---------------------------------------------exportModuleInput
bool HostModule::exportModuleInput(ModuleInputBase* pInput, const tString* pExportedName)
{
  tString sInputName = pInput->getName(); //this is the default name
  if(pExportedName)
  {
    sInputName = *pExportedName;
  }

  ModuleInputBase* pProxy  = pInput->createProxy();
  if(pProxy)
  {
    m_aInputOwnerMap[pProxy] = pInput->getParentModule();
    return registerInput(pProxy, sInputName);
  }
  return false;
}



//---------------------------------------------declareExternalModules
void HostModule::declareExternalModules(ClayShaper& aShaper)
{
  aShaper.clearExternals();

  tExportedInOwnerMap::iterator it = m_aInputOwnerMap.begin();
  tExportedInOwnerMap::iterator ed = m_aInputOwnerMap.end();
  for(; it!=ed; ++it)
  {
    ModuleInputBase* pInput = it->first;
    if(pInput->isConnected())
    {
      Module* pModule = pInput->getSourceModule();
      aShaper.addExternalModule(pModule);
    }
  }
}

//---------------------------------------------restoreOutput
bool HostModule::restoreOutput(XERCES::DOMElement* pNode)
{
  tString sModuleID;
  if(!XercesXML::getAttributeValue(pNode, "module-id", sModuleID))
  {
    return false;
  }

  tString sOutputName;
  if(!XercesXML::getAttributeValue(pNode, "output-id", sOutputName))
  {
    return false;
  }

  Module* pModule = getModule(sModuleID);
  if(!pModule)
  {
    return false;
  }

  return exportModuleOutput(pModule, sOutputName);
}

//---------------------------------------------restoreInput
bool HostModule::restoreInput(XERCES::DOMElement* pNode)
{
  tString sModuleID;
  if(!XercesXML::getAttributeValue(pNode, "module-id", sModuleID))
  {
    return false;
  }

  tString sInputName;
  if(!XercesXML::getAttributeValue(pNode, "input-id", sInputName))
  {
    return false;
  }

  Module* pModule = getModule(sModuleID);
  if(!pModule)
  {
    return false;
  }

  return exportModuleInput(pModule, sInputName);
}

//---------------------------------------------registerModuleInputs
void HostModule::registerModuleInputs()
{
      
}

//---------------------------------------------registerModuleOutputs
void HostModule::registerModuleOutputs()
{
}

} }
