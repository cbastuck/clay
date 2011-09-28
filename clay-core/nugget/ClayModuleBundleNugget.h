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

#ifndef CLAY_MODULE_BUNDLE_NUGGET_H_
#define CLAY_MODULE_BUNDLE_NUGGET_H_

#ifndef CLAY_MODULE_BUNDLE_NUGGET
#error "CLAY_MODULE_BUNDLE_NUGGET, must be defined (must be of type: boost::mpl::list<Module_1, Module_2, ...>)"
#endif

#include "IClayModuleBundleNugget.h"

#include <clay-core/base/ModuleDescriptor.h>
#include <clay-core/registry/ModuleRegistry.h>

extern "C"
{
  CLAY_DLL_EXPORT CLAY::NUGGET::IClayNugget* getNugget();
};

//---------------------------------------------class ClayModuleBundleNugget
namespace CLAY{ namespace NUGGET{

class ModuleBundleRegistry : public CLAY::ModuleRegistry
{
public:
  template<class T>
  void init()
  {
    addModules<T>();
  }

protected:
  template<typename T>
  void addModules()
  {
    typedef typename boost::mpl::begin<T>  tIterator;
    const size_t n = boost::mpl::size<T>::type::value;
    AddModuleHelper<tIterator, n>::addModule(this);
  }

private:
  //meta-programming helper classes for iterating boost::tuple instance - middle in compile time recursion
  template<typename T, size_t size>
  class AddModuleHelper
  {
  public:
    static void addModule(ModuleBundleRegistry* pRegistry)
    {
      typedef typename boost::mpl::deref<typename T::type>::type tCurrentType;
      pRegistry->registerModule<tCurrentType>();

      typedef typename boost::mpl::next<typename T::type> tNext;

      AddModuleHelper<tNext, size-1>::addModule(pRegistry);
    }
  };

  //end of compile time recursion
  template<typename T>
  class AddModuleHelper<T, 0>
  {
  public:
    static void addModule(ModuleBundleRegistry*){ /* finished - end of compile-time recursion */ }
  };
};

//---------------------------------------------

template<class T>
class ClayModuleBundleNugget : public IClayModuleBundleNugget
{
private:
  typedef T tList;

public:
  //---------------------------------------------ClayModuleBundleNugget
  ClayModuleBundleNugget()
  {
     m_aRegistry.init<tList>();
  }

  //---------------------------------------------getRegistry
  virtual ModuleRegistry* getRegistry()
  {
    return &m_aRegistry;
  }

  //---------------------------------------------getType
  virtual IClayNugget::Type getType() const
  {
    return IClayNugget::MODULE_BUNDLE_NUGGET;
  }

  //---------------------------------------------init
  virtual bool init()
  {
    return true;
  }

  //---------------------------------------------deInit
  virtual bool deInit()
  {
    return true;
  }

private:
  ModuleBundleRegistry m_aRegistry;
};

} }

//---------------------------------------------
//--------------------------------------------- Nugget Entry point
//---------------------------------------------

//---------------------------------------------
CLAY::NUGGET::IClayNugget* getNugget()
{
  typedef CLAY_MODULE_BUNDLE_NUGGET tList; //see defines on the top of this file

  static CLAY::NUGGET::ClayModuleBundleNugget<tList>* pNugget = NULL;
  if(!pNugget)
  {
    static CLAY::NUGGET::ClayModuleBundleNugget<tList> aModuleBundleNugget; //singleton instance for this shared library
    pNugget = &aModuleBundleNugget;
    pNugget->init();
  }
  
  return pNugget;
}

#endif
