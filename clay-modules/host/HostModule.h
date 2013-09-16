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

#ifndef HostModule_H_
#define HostModule_H_

//clay-core
#include <clay-core/host/Host.h>
#include <clay-core/base/Module.h>
#include <clay-core/helper/Mutex.h>

//STL
#include <map>
#include <deque>

namespace CLAY{ 
class ClayShaper;
class ClayExecutable; 
class ModuleRegistry;
}

namespace CLAY{ namespace MODULE{

class HostModule : public Module,
                   public Host
{
private:
  typedef std::deque<Module*> tInterfaceCollection;

public:   
  typedef tInterfaceCollection::iterator       tIterfaceInterator;
  typedef tInterfaceCollection::const_iterator tConstIterfaceInterator;

  static const char* staticModuleURI();

  HostModule(const tString& sId);
  HostModule(const tString& sId, ModuleRegistry* pModuleRegistry);
  virtual ~HostModule();

  virtual const char* getModuleURI() const;

  virtual bool init(XERCES::DOMNode* pNode);
  virtual void deInit(); 

  virtual bool save(XERCES::DOMElement* pNode);
  virtual bool load(XERCES::DOMElement* pNode, tConnectionMap* pInputConnections = NULL, tConnectionMap* pOutputConnections = NULL);

  virtual CompileResultCode shapeProcess(ClayShaper* pShaper, ClayExecutable& aTarget);

  virtual ProcessResultCode process();

  virtual void closeProject();

  bool save(const tString& sFn);
  bool load(const tString& sFn, tConnectionMap* pInputConnections = NULL, tConnectionMap* pOutputConnections = NULL);

  void shapeProcess();

  bool exportModuleOutput(Module* pHostedModule, const tString& sOutputName);
  bool exportModuleOutput(ModuleOutputBase* pOutput, const tString* pExportedName = NULL);

  bool exportModuleInput(Module* pHostedModule, const tString& sInputName);
  bool exportModuleInput(ModuleInputBase* pInput, const tString* pExportedName = NULL);

protected:
  void declareExternalModules(ClayShaper& aShaper);

  bool restoreOutput(XERCES::DOMElement* pNode);
  bool restoreInput(XERCES::DOMElement* pNode);

private:
  virtual void registerModuleInputs();
  virtual void registerModuleOutputs();

  typedef std::map<ModuleOutputBase*, Module*> tExportedOutOwnerMap;
  typedef std::map<ModuleInputBase*, Module*>  tExportedInOwnerMap;

  bool                 m_bModuleRegistryOwner;

  ClayExecutable*      m_pRuntimeExecutable;
  ClayExecutable*      m_pPendingExecutable;
  ClayExecutable*      m_pGarbageExectuable;

  Mutex                m_aPendingExecutableLock;
  Mutex                m_aRuntimeLock;

  tExportedOutOwnerMap m_aOutputOwnerMap;
  tExportedInOwnerMap  m_aInputOwnerMap;
};

} }

#endif

