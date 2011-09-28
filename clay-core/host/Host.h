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

#ifndef HOST_H_
#define HOST_H_

#include <clay-core/base/Module.h>
#include <clay-core/event/Signal.h>
#include <clay-core/base/ModuleDescriptor.h>

#include <deque>
#include <xercesc/dom/DOMElement.hpp>

namespace CLAY{ namespace NUGGET{ 
class IClayNugget;
class ClayNuggetLoader;
} }

namespace CLAY
{

class ModuleRegistry;

class Host
{
public:
  typedef std::deque<Module*>               tModuleCollection;
  typedef tModuleCollection::iterator       tModuleIterator; 
  typedef tModuleCollection::const_iterator tConstModuleIterator;
  
  CLAY_DLL_EXPORT Host(ModuleRegistry* pRegistry);
  CLAY_DLL_EXPORT ~Host();

  CLAY_DLL_EXPORT virtual bool init();
  CLAY_DLL_EXPORT virtual void deInit();

  CLAY_DLL_EXPORT virtual bool connect(const tString& sLhsMod, const tString& sRhsMod);
  CLAY_DLL_EXPORT virtual bool connect(Module* pFrom, Module* pTo); //do clever connection
  CLAY_DLL_EXPORT virtual bool connect(Module* pFrom, ModuleOutputBase* pFromOut, Module* pTo, ModuleInputBase* pToIn, bool bCheckCompatibility=true);

  CLAY_DLL_EXPORT virtual bool saveProject(const tString& sFilename);
  CLAY_DLL_EXPORT virtual bool saveProject(XERCES::DOMElement* pNode);
  CLAY_DLL_EXPORT virtual bool saveModule (XERCES::DOMElement* pNode, Module* pModule);

  CLAY_DLL_EXPORT virtual bool loadProject(const tString& sFilename);
  CLAY_DLL_EXPORT virtual bool loadProject(XERCES::DOMElement* pNode);
  CLAY_DLL_EXPORT virtual Module* loadModule (XERCES::DOMElement* pNode, Module::tConnectionMap* pInputConnections, Module::tConnectionMap* pOutputConnections);

  CLAY_DLL_EXPORT virtual void closeProject();

  template<class MODULE>
  CLAY_DLL_EXPORT MODULE* createModule(const tString&   sRuntimeModuleId, 
                                       XERCES::DOMNode* pConfigNode, 
                                       bool             bAddToManagedModules); //inline

  CLAY_DLL_EXPORT Module* createModule(ModuleDescriptorTraits::tModuleID    aModuleId, 
                                        ModuleDescriptorTraits::tNamespaceID aNamespaceId,
                                        const tString&                       sRuntimeModuleId, 
                                        XERCES::DOMNode*                     pConfigNode, 
                                        bool                                 bAddToManagedModules);

  CLAY_DLL_EXPORT void removeModule(Module* pModule, bool bRemoveFromManagedModules=true);

  CLAY_DLL_EXPORT Module* getModule(const tString& sRuntimeModuleId) const;

  CLAY_DLL_EXPORT bool addPriorityModule(Module* pModule);
  CLAY_DLL_EXPORT void clearPriorityList();

  CLAY_DLL_EXPORT CLAY::Module::ProcessResultCode play();

  CLAY_DLL_EXPORT NUGGET::IClayNugget* loadNugget(const tString& sNuggetFn);
  CLAY_DLL_EXPORT NUGGET::IClayNugget* restoreNugget(XERCES::DOMElement* pNode);

  CLAY_DLL_EXPORT tConstModuleIterator beginModules() const; //inline 
  CLAY_DLL_EXPORT tConstModuleIterator endModules() const;   //inline 
  CLAY_DLL_EXPORT tModuleIterator      beginModules();       //inline
  CLAY_DLL_EXPORT tModuleIterator      endModules();         //inline 

  CLAY_DLL_EXPORT ModuleRegistry* getRegistry() const;       //inline
  CLAY_DLL_EXPORT unsigned int    getNumHostedModules() const;  //inline

  //----- signals that are emitted by this class
  Signal<boost::function2<void, Module*, Module*> >     signalConnectionEstablished;
  Signal<boost::function1<void, NUGGET::IClayNugget*> > signalClayNuggetLoaded;

protected:
  CLAY_DLL_EXPORT virtual bool restoreInputConnections (const Module::tConnectionMap& collInputConnections);
  CLAY_DLL_EXPORT virtual bool restoreOutputConnections(const Module::tConnectionMap& collOutputConnections);
  CLAY_DLL_EXPORT bool restorePriorityModule(XERCES::DOMElement* pNode);

  CLAY_DLL_EXPORT virtual void destroyModule(Module* pModule);

  CLAY_DLL_EXPORT const tModuleCollection& getHostedModules() const; //inline 
  CLAY_DLL_EXPORT const tModuleCollection& getPriorityModules() const; //inline 

private:
  void addModule(Module* pModule);

  tModuleCollection         m_collModules;
  ModuleRegistry*           m_pRegistry;
  NUGGET::ClayNuggetLoader* m_pNuggetLoader;
  tModuleCollection         m_aPriorityModules;
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------createModule
template<class MODULE>
inline MODULE* Host::createModule(const tString&   sRuntimeModuleId, 
                                  XERCES::DOMNode* pConfigNode, 
                                  bool             bAddToManagedModules)
{
  return static_cast<MODULE*>(createModule(MODULE::tDescriptor::eModuleId, 
                                            MODULE::tDescriptor::eNamespaceId, 
                                            sRuntimeModuleId,
                                            pConfigNode, 
                                            bAddToManagedModules));
}

//---------------------------------------------beginModules
inline Host::tConstModuleIterator Host::beginModules() const
{ 
  return m_collModules.begin(); 
}

//---------------------------------------------endModules
inline Host::tConstModuleIterator Host::endModules() const  
{ 
  return m_collModules.end(); 
}

//---------------------------------------------beginModules
inline Host::tModuleIterator Host::beginModules()  
{ 
  return m_collModules.begin(); 
}

//---------------------------------------------endModules
inline Host::tModuleIterator Host::endModules()    
{ 
  return m_collModules.end(); 
}

//---------------------------------------------getRegistry
inline ModuleRegistry* Host::getRegistry() const
{ 
  return m_pRegistry; 
}

//---------------------------------------------getNumHostedModules
inline unsigned int Host::getNumHostedModules() const
{ 
  return m_collModules.size(); 
}

//---------------------------------------------getHostedModules
inline const Host::tModuleCollection& Host::getHostedModules() const
{ 
  return m_collModules; 
}

//---------------------------------------------getPriorityModules
inline const Host::tModuleCollection& Host::getPriorityModules() const
{
  return m_aPriorityModules;
}

}

#endif 
