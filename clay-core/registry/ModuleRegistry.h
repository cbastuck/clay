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

#ifndef ModuleRegistry_H_
#define ModuleRegistry_H_

//clay-core
#include <clay-core/base/ClayDefines.h>
#include <clay-core/base/ModuleDescriptor.h>

//STL
#include <map>
#include <deque>

namespace CLAY{ namespace NUGGET{
class IClayNugget;
class IClaySingleModuleNugget;
class IClayModuleBundleNugget;
} }

namespace CLAY{ namespace HELPER{ class SharedLibrary; } }

namespace CLAY{

class Module;

class ModuleRegistry
{
public:
  typedef std::map<ModuleDescriptorTraits::tModuleID,    ModuleDescriptorBase>  tDescriptorCollection;
  typedef std::map<ModuleDescriptorTraits::tNamespaceID, tDescriptorCollection> tNamespaceCollection;

  typedef tDescriptorCollection::iterator       descr_iterator;
  typedef tDescriptorCollection::const_iterator const_descr_iterator;

  typedef tNamespaceCollection::iterator        namespace_iterator;
  typedef tNamespaceCollection::const_iterator  const_namespace_iterator;

  CLAY_DLL_EXPORT ModuleRegistry();
  CLAY_DLL_EXPORT ~ModuleRegistry();

  CLAY_DLL_EXPORT bool registerModules(const ModuleRegistry& aOther);
  template<class T>
  CLAY_DLL_EXPORT void registerModule(const T& aModule); //inline
  template<class T>
  CLAY_DLL_EXPORT void registerModule(); //inline 
  CLAY_DLL_EXPORT void registerModule(const ModuleDescriptorBase& aDescriptor);
  CLAY_DLL_EXPORT bool registerModules(NUGGET::IClayNugget* pNugget);

  template<class T>
  CLAY_DLL_EXPORT T* createItem(const tString& sRuntimeModuleId = ""); //inline

  CLAY_DLL_EXPORT Module* createItem(ModuleDescriptorTraits::tModuleID    aModuleId,
                                      ModuleDescriptorTraits::tNamespaceID aNamespaceId = ModuleDescriptorTraits::defaultNamespace,
                                      const tString& sRuntimeModuleId = "");
  CLAY_DLL_EXPORT void destroyItem(Module* pModule);

  CLAY_DLL_EXPORT unsigned int getNumModules() const;
  CLAY_DLL_EXPORT unsigned int getNumNamespaces() const;

  CLAY_DLL_EXPORT const ModuleDescriptorBase* getModuleDescriptor(unsigned int uIdx) const;

  CLAY_DLL_EXPORT namespace_iterator beginNamespace(); //inline 
  CLAY_DLL_EXPORT namespace_iterator endNamespace();   //inline 

  CLAY_DLL_EXPORT const_namespace_iterator beginNamespace() const; //inline
  CLAY_DLL_EXPORT const_namespace_iterator endNamespace()   const; //inline

protected:  
  CLAY_DLL_EXPORT void addItem(const ModuleDescriptorBase& aDescriptor);

private:
  tNamespaceCollection  m_collRegisteredNamespaces;
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------registerModule
template<class T>
inline void ModuleRegistry::registerModule(const T& aModule)
{
  //add a module to the registry - using the static descriptor information
  registerModule<T>(); //forward to zero-arguments implementation
}

//---------------------------------------------registerModule
template<class T>
inline void ModuleRegistry::registerModule()
{
  typedef typename T::tDescriptor tDescriptor;

  tDescriptor aDescriptor;
  tDescriptorCollection& aDstNamespace = m_collRegisteredNamespaces[tDescriptor::getNamespaceId()];

  tDescriptorCollection::value_type aModuleFactory(tDescriptor::getModuleId(), aDescriptor);

  aDstNamespace.insert(aModuleFactory);
}

//---------------------------------------------createItem
template<class T>
inline T* ModuleRegistry::createItem(const tString& sRuntimeModuleId)
{
  return static_cast<T*>(createItem(T::tDescriptor::eModuleId,
                                    T::tDescriptor::eNamespaceId, 
                                    sRuntimeModuleId));
}

//---------------------------------------------beginNamespace
inline ModuleRegistry::namespace_iterator ModuleRegistry::beginNamespace()
{ 
  return m_collRegisteredNamespaces.begin(); 
}

//---------------------------------------------endNamespace
inline ModuleRegistry::namespace_iterator ModuleRegistry::endNamespace()  
{ 
  return m_collRegisteredNamespaces.end(); 
}

//---------------------------------------------beginNamespace
inline ModuleRegistry::const_namespace_iterator ModuleRegistry::beginNamespace() const
{ 
  return m_collRegisteredNamespaces.begin(); 
}

//---------------------------------------------endNamespace
inline ModuleRegistry::const_namespace_iterator ModuleRegistry::endNamespace() const
{ 
  return m_collRegisteredNamespaces.end(); 
}

}

#endif
