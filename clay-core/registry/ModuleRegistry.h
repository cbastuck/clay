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

#include <boost/function.hpp>
#include <boost/functional/factory.hpp>

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
  typedef boost::function<Module*(const tString&)> tModuleFactory;
  typedef std::map<tString, tModuleFactory> tFactoryCollection;

  typedef tFactoryCollection::iterator       iterator;
  typedef tFactoryCollection::const_iterator const_iterator;
  
  CLAY_DLL_EXPORT ModuleRegistry();
  CLAY_DLL_EXPORT ~ModuleRegistry();

  CLAY_DLL_EXPORT bool registerModule(const char* moduleURI, tModuleFactory fac);
  CLAY_DLL_EXPORT bool registerModules(const ModuleRegistry& aOther);
  template<class T>
  CLAY_DLL_EXPORT void registerModule(const T& aModule); //inline
  template<class T>
  CLAY_DLL_EXPORT void registerModule(); //inline 
  CLAY_DLL_EXPORT bool registerModules(NUGGET::IClayNugget* pNugget);

  template<class T>
  CLAY_DLL_EXPORT T* createItem(const tString& sRuntimeModuleId); //inline

  CLAY_DLL_EXPORT Module* createItem(const char* sModuleURI, const tString& sRuntimeModuleId);
  CLAY_DLL_EXPORT void destroyItem(Module* pModule);

  CLAY_DLL_EXPORT unsigned int getNumRegisteredModules() const;

  CLAY_DLL_EXPORT const char* getModuleURI(unsigned int uIdx) const;
  CLAY_DLL_EXPORT tModuleFactory getModuleFactory(unsigned int uIdx) const;

  CLAY_DLL_EXPORT const_iterator begin() const; //inline 
  CLAY_DLL_EXPORT const_iterator end() const;   //inline 
  
protected:  
  CLAY_DLL_EXPORT bool addItem(const char* moduleURI, tModuleFactory fac);

  const_iterator getEntry(unsigned int uIdx) const;

private:
  tFactoryCollection m_registeredModules;
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
  m_registeredModules.insert(std::make_pair(T::staticModuleURI(), boost::factory<T*>()));
}

//---------------------------------------------createItem
template<class T>
inline T* ModuleRegistry::createItem(const tString& sRuntimeModuleId)
{
  return static_cast<T*>(createItem(T::getModuleURI(), sRuntimeModuleId));
}

//---------------------------------------------begin
inline ModuleRegistry::const_iterator ModuleRegistry::begin() const
{ 
  return m_registeredModules.begin(); 
}

//---------------------------------------------end
inline ModuleRegistry::const_iterator ModuleRegistry::end() const
{ 
  return m_registeredModules.end(); 
}

}

#endif
