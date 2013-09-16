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

#ifndef MODULE_H_
#define MODULE_H_

//CLAY includes
#include "ClayDefines.h"
#include "ModuleIO.h"

#include <clay-core/event/Signal.h>
#include <clay-core/helper/XMLHelper.h>

//STL includes
#include <deque>
#include <map>
#include <set>

//boost includes
#include <boost/function.hpp>

using namespace CLAY::HELPER; //no need to for explicit qualification for helpers

namespace CLAY{

//forward declarations
class ClayExecutable;
class ClayShaper;
class ModuleDependencies;
class ModuleDescriptorBase;

//---------------------------Module
class Module
{
protected:
  typedef std::deque<ModuleInputBase*>  tInputCollection;
  typedef std::deque<ModuleOutputBase*> tOutputCollection;

public:
  enum ProcessResultCode{Process_OK, Process_ERROR, ProcessNotImplemented_ERROR, Process_NotPossible};
  enum CompileResultCode{Compile_OK, Compile_ERROR, UnresolvedDependencies_ERROR, NothingToCompile_Error};
  enum ConnectResultCode{Connect_OK, Connect_ERROR, NoSuitableConnection_ERROR};
  enum ConnectionStrategy{ CLEVER_CONNECT_MODE, TEST_CLEVER_CONNECT_MODE };

  typedef tInputCollection::iterator  tInputIterator;
  typedef tOutputCollection::iterator tOutputIterator;

  typedef std::deque<ModuleConnectionDescriptor> tConnectionColl;
  typedef std::map<Module*, tConnectionColl>     tConnectionMap;

  CLAY_DLL_EXPORT Module(const tString& sModuleRuntimeId);
  CLAY_DLL_EXPORT virtual ~Module();

  CLAY_DLL_EXPORT bool init(const tString& xml);
  CLAY_DLL_EXPORT virtual bool init(XERCES::DOMNode* pNode);
  CLAY_DLL_EXPORT virtual void deInit();

  //instance/class identification 
  CLAY_DLL_EXPORT virtual const char* getModuleURI() const = 0;
  CLAY_DLL_EXPORT inline const tString& getRuntimeModuleID() const;

  //persistence
  CLAY_DLL_EXPORT virtual bool save(XERCES::DOMElement* pNode);
  CLAY_DLL_EXPORT virtual bool load(XERCES::DOMElement* pNode);
  CLAY_DLL_EXPORT virtual bool load(XERCES::DOMElement* pNode, tConnectionMap* pInputConnections, tConnectionMap* pOutputConnections);

  //inputs - outputs
  CLAY_DLL_EXPORT inline virtual size_t getNumInputs() const;
  CLAY_DLL_EXPORT inline virtual size_t getNumOutputs() const;

  CLAY_DLL_EXPORT inline tInputIterator beginInput();
  CLAY_DLL_EXPORT inline tInputIterator endInput();

  CLAY_DLL_EXPORT inline tOutputIterator beginOutput();
  CLAY_DLL_EXPORT inline tOutputIterator endOutput();

  CLAY_DLL_EXPORT virtual ModuleInputBase*  getInput (size_t i) const;
  CLAY_DLL_EXPORT virtual ModuleInputBase*  getInput (const tString& sInputName) const;
  CLAY_DLL_EXPORT virtual ModuleOutputBase* getOutput(size_t i) const;
  CLAY_DLL_EXPORT virtual ModuleOutputBase* getOutput(const tString& sOutputName) const;
  CLAY_DLL_EXPORT virtual ModuleOutputBase* duplicateModuleOutput(ModuleOutputBase* pOriginal);
    
  //io connections
  CLAY_DLL_EXPORT virtual bool canConnectFrom(Module* pPredecessor);
  CLAY_DLL_EXPORT virtual ConnectResultCode connectFrom(Module* pPredecessor);
  CLAY_DLL_EXPORT virtual ConnectResultCode connectFrom(Module* pPredecessor, ConnectionStrategy eConnectionStrategy);
  CLAY_DLL_EXPORT virtual ConnectResultCode connectFrom(const tString& sInputname, Module* pPredecessor, const tString& sOutputname);

  CLAY_DLL_EXPORT virtual void disconnect();
  CLAY_DLL_EXPORT virtual void disconnect(ModuleOutputBase* pOutput);
  CLAY_DLL_EXPORT virtual void disconnect(ModuleInputBase* pInput);

  CLAY_DLL_EXPORT void getInputConnectionsFrom(Module* pOther, tInputCollection& collDst);
  CLAY_DLL_EXPORT void getOutputConnectionsTo (Module* pOther, tOutputCollection& collDst);
  
  //processing
  CLAY_DLL_EXPORT virtual CompileResultCode shapeProcess(ClayShaper* pShaper, ClayExecutable& aTarget);
  CLAY_DLL_EXPORT virtual ProcessResultCode process();

  //traversal
  template<class tCallback>
  void visitTraversal(tCallback aVisitCallback);

  //signals
  typedef CLAY::Signal<boost::function<void(ModuleOutputBase*)> > tOutputActivity;
  tOutputActivity signalModuleOutputRegistered;
  tOutputActivity signalModuleOutputUnregistered;
  tOutputActivity signalModuleOutputConnected;
  tOutputActivity signalModuleOutputDisconnected;

  typedef CLAY::Signal<boost::function1<void, ModuleInputBase*> > tInputActivity;
  tInputActivity signalModuleInputRegistered;
  tInputActivity signalModuleInputUnregistered;
  tInputActivity signalModuleInputConnected;
  tInputActivity signalModuleInputDisconnected;

protected:
  CLAY_DLL_EXPORT virtual bool loadImpl(XERCES::DOMElement* pNode, tConnectionMap* pInputConnections, tConnectionMap* pOutputConnections);
  CLAY_DLL_EXPORT virtual bool saveModuleInput(XERCES::DOMElement* pParent, ModuleInputBase*  pModuleInput);
  CLAY_DLL_EXPORT virtual bool loadModuleInput(XERCES::DOMElement* pNode, tConnectionMap* pInputConnections  = NULL);

  CLAY_DLL_EXPORT virtual bool saveModuleOutput(XERCES::DOMElement* pParent, ModuleOutputBase* pModuleOutput);
  CLAY_DLL_EXPORT virtual bool loadModuleOutput(XERCES::DOMElement* pNode, tConnectionMap* pOutputConnections = NULL);

  CLAY_DLL_EXPORT virtual void onInputDisconnected (ModuleInputBase* pInput);
  CLAY_DLL_EXPORT virtual void onOutputDisconnected(ModuleOutputBase* pOutput);

  CLAY_DLL_EXPORT bool registerInput (ModuleInputBase*  pInput,  const tString& sName);
  CLAY_DLL_EXPORT bool registerOutput(ModuleOutputBase* pOutput, const tString& sName);

  CLAY_DLL_EXPORT bool unregisterInput (ModuleInputBase*  pInput,  bool bDelete=true);
  CLAY_DLL_EXPORT bool unregisterOutput(ModuleOutputBase* pOutput, bool bDelete=true);
  CLAY_DLL_EXPORT bool unregisterInputs (bool bDelete=true);
  CLAY_DLL_EXPORT bool unregisterOutputs(bool bDelete=true);

  CLAY_DLL_EXPORT inline void setRuntimeModuleID(const tString& sID);

  //traverse the graph of this module starting - follow connected modules
  template<class tCallback>
  void visitTraversal(tCallback aVisitCallback, std::set<Module*>& collVisitedModules);

private:
  friend class ModuleInputBase;
  friend class ModuleOutputBase;

  tInputCollection m_collInputs;
  tOutputCollection m_collOutputs;
  tString m_sModuleRuntimeID;
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------
inline size_t Module::getNumInputs() const
{ 
  return m_collInputs.size(); 
}

//---------------------------------------------
inline size_t Module::getNumOutputs() const
{
  return m_collOutputs.size(); 
}

//---------------------------------------------
inline Module::tInputIterator Module::beginInput()
{
  return m_collInputs.begin(); 
}

//---------------------------------------------
inline Module::tInputIterator Module::endInput()  
{ 
  return m_collInputs.end(); 
}

//---------------------------------------------
inline Module::tOutputIterator Module::beginOutput()
{
  return m_collOutputs.begin(); 
}

//---------------------------------------------
inline Module::tOutputIterator Module::endOutput()  
{ 
  return m_collOutputs.end(); 
}

//---------------------------------------------
inline const tString& Module::getRuntimeModuleID() const  
{ 
  return m_sModuleRuntimeID; 
}

//---------------------------------------------
inline void Module::setRuntimeModuleID(const tString& sID)
{ 
  m_sModuleRuntimeID = sID; 
}

//---------------------------------------------
template<class tCallback>
inline void Module::visitTraversal(tCallback aVisitCallback)
{
  std::set<Module*> collVisited; //use this data structure to keep track of visited modules
  visitTraversal(aVisitCallback, collVisited);
}

//---------------------------------------------
template<class tCallback>
void Module::visitTraversal(tCallback aVisitCallback, std::set<Module*>& collVisitedModules)
{
  collVisitedModules.insert(this); //mark this module as visited - avoid cycles ...
  aVisitCallback(this); // ... and invoke the callback on it

  tOutputIterator it  = beginOutput();
  tOutputIterator end = endOutput();
  for(; it!=end; ++it)
  {
    ModuleOutputBase* pCurrent = *it;
    for(unsigned int i=0, n=pCurrent->getNumConnections(); i<n; ++i)
    {
      Module* pParent = pCurrent->getTargetModule(i);
      CLAY_ASSERT(pParent);
      if(collVisitedModules.find(pParent) == collVisitedModules.end()) //then the node has not been visited yet
      {
        pParent->visitTraversal(aVisitCallback, collVisitedModules);
      }
    }
  }
}

}

#endif
