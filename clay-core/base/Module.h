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

  typedef std::deque<int>::size_type tSize; //int is just a dummy

  typedef tInputCollection::iterator  tInputIterator;
  typedef tOutputCollection::iterator tOutputIterator;

  typedef std::deque<ModuleConnectionDescriptor> tConnectionColl;
  typedef std::map<Module*, tConnectionColl>     tConnectionMap;

  CLAY_DLL_EXPORT Module(const tString& sModuleRuntimeId);
  CLAY_DLL_EXPORT virtual ~Module();

  CLAY_DLL_EXPORT virtual bool init(XERCES::DOMNode* pNode);
  CLAY_DLL_EXPORT virtual void deInit();

  CLAY_DLL_EXPORT virtual const ModuleDescriptorBase* getModuleDescriptor() const;

  CLAY_DLL_EXPORT virtual bool save(XERCES::DOMElement* pNode);
  CLAY_DLL_EXPORT virtual bool load(XERCES::DOMElement* pNode, tConnectionMap* pInputConnections = NULL, tConnectionMap* pOutputConnections = NULL);

  CLAY_DLL_EXPORT virtual bool saveModuleInput(XERCES::DOMElement* pParent, ModuleInputBase*  pModuleInput);
  CLAY_DLL_EXPORT virtual bool loadModuleInput(XERCES::DOMElement* pNode, tConnectionMap* pInputConnections  = NULL);

  CLAY_DLL_EXPORT virtual bool saveModuleOutput(XERCES::DOMElement* pParent, ModuleOutputBase* pModuleOutput);
  CLAY_DLL_EXPORT virtual bool loadModuleOutput(XERCES::DOMElement* pNode, tConnectionMap* pOutputConnections = NULL);

  CLAY_DLL_EXPORT virtual ModuleInputBase*  getInput (tSize i) const;
  CLAY_DLL_EXPORT virtual ModuleInputBase*  getInput (const tString& sInputName) const;
  CLAY_DLL_EXPORT virtual ModuleOutputBase* getOutput(tSize i) const;
  CLAY_DLL_EXPORT virtual ModuleOutputBase* getOutput(const tString& sOutputName) const;
  CLAY_DLL_EXPORT virtual ModuleOutputBase* duplicateModuleOutput(ModuleOutputBase* pOriginal);

  enum ConnectionStrategy{ CLEVER_CONNECT_MODE, TEST_CLEVER_CONNECT_MODE };
  CLAY_DLL_EXPORT virtual ConnectResultCode connectFrom(Module* pPredecessor, ConnectionStrategy eConnectionStrategy=CLEVER_CONNECT_MODE);
  CLAY_DLL_EXPORT virtual ConnectResultCode connectFrom(const tString& sInputname, Module* pPredecessor, const tString& sOutputname);
  CLAY_DLL_EXPORT virtual CompileResultCode shapeProcess(ClayShaper* pShaper, ClayExecutable& aTarget);
  CLAY_DLL_EXPORT virtual ProcessResultCode process();

  CLAY_DLL_EXPORT virtual bool canConnectFrom(Module* pPredecessor);

  CLAY_DLL_EXPORT virtual void disconnect();
  CLAY_DLL_EXPORT virtual void disconnect(ModuleOutputBase* pOutput);
  CLAY_DLL_EXPORT virtual void disconnect(ModuleInputBase* pInput);

  CLAY_DLL_EXPORT virtual void onInputDisconnected (ModuleInputBase* pInput);
  CLAY_DLL_EXPORT virtual void onOutputDisconnected(ModuleOutputBase* pOutput);

  CLAY_DLL_EXPORT bool registerInput (ModuleInputBase*  pInput,  const tString& sName);
  CLAY_DLL_EXPORT bool registerOutput(ModuleOutputBase* pOutput, const tString& sName);

  CLAY_DLL_EXPORT bool unregisterInput (ModuleInputBase*  pInput,  bool bDelete=true);
  CLAY_DLL_EXPORT bool unregisterOutput(ModuleOutputBase* pOutput, bool bDelete=true);
  CLAY_DLL_EXPORT bool unregisterInputs (bool bDelete=true);
  CLAY_DLL_EXPORT bool unregisterOutputs(bool bDelete=true);

  CLAY_DLL_EXPORT void getInputConnectionsFrom(Module* pOther, tInputCollection& collDst);
  CLAY_DLL_EXPORT void getOutputConnectionsTo (Module* pOther, tOutputCollection& collDst);

  //inline stuff
  CLAY_DLL_EXPORT virtual tSize getNumInputs()  const{ return m_collInputs.size(); }
  CLAY_DLL_EXPORT virtual tSize getNumOutputs() const{ return m_collOutputs.size(); }

  CLAY_DLL_EXPORT tInputIterator beginInput(){ return m_collInputs.begin(); }
  CLAY_DLL_EXPORT tInputIterator endInput()  { return m_collInputs.end(); }

  CLAY_DLL_EXPORT tOutputIterator beginOutput(){ return m_collOutputs.begin(); }
  CLAY_DLL_EXPORT tOutputIterator endOutput()  { return m_collOutputs.end(); }

  CLAY_DLL_EXPORT const tString& getRuntimeModuleID() const  { return m_sModuleRuntimeID; }
  CLAY_DLL_EXPORT void setRuntimeModuleID(const tString& sID){ m_sModuleRuntimeID = sID; }

  //methods to query the module state
  CLAY_DLL_EXPORT bool isInitialized() const;
  CLAY_DLL_EXPORT bool isActive() const;

  //signals that are emitted by this class
  typedef boost::function0<void> tEmptyFunc;
  CLAY::Signal<tEmptyFunc> signalModuleInitialized;
  CLAY::Signal<tEmptyFunc> signalModuleActive;
  CLAY::Signal<tEmptyFunc> signalModuleUnitialized;
  CLAY::Signal<tEmptyFunc> signalModuleInactive;

  typedef boost::function1<void, ModuleOutputBase*> tOutputFunc;
  CLAY::Signal<tOutputFunc> signalModuleOutputRegistered;
  CLAY::Signal<tOutputFunc> signalModuleOutputUnregistered;
  CLAY::Signal<tOutputFunc> signalModuleOutputConnected;
  CLAY::Signal<tOutputFunc> signalModuleOutputDisconnected;

  typedef boost::function1<void, ModuleInputBase*> tInputFunc;
  CLAY::Signal<tInputFunc> signalModuleInputRegistered;
  CLAY::Signal<tInputFunc> signalModuleInputUnregistered;
  CLAY::Signal<tInputFunc> signalModuleInputConnected;
  CLAY::Signal<tInputFunc> signalModuleInputDisconnected;

  //use this template for traversal
  template<class tCallback>
  void visitTraversal(tCallback aVisitCallback)
  {
    std::set<Module*> collVisited; //use this data structure to keep track of visited modules
    visitTraversal(aVisitCallback, collVisited);
  }

  //traverse the graph of this module starting - follow connected modules
  template<class tCallback>
  void visitTraversal(tCallback aVisitCallback,
                      std::set<Module*>& collVisitedModules)
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

protected: //methods

  //module states enumeration
  enum ModuleState
  {
    MODULE_INITIALIZED   = 1 << 0,
    MODULE_ACTIVE        = 1 << 1
  };

  enum NegativeModuleState
  {
    MODULE_UNINITIALIZED = 1 << 0,
    MODULE_INACTIVE      = 1 << 1
  };

  CLAY_DLL_EXPORT void setModuleState(ModuleState eModuleState);
  CLAY_DLL_EXPORT void setModuleState(NegativeModuleState eModuleState);

  void resetModuleState();

protected: //members
  tInputCollection m_collInputs;
  tOutputCollection m_collOutputs;

  tString m_sModuleRuntimeID;

  unsigned int m_uState;
};

}

#endif
