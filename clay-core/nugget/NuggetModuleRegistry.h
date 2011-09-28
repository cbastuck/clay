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

#ifndef NUGGET_MODULE_REGISTRY_H_
#define NUGGET_MODULE_REGISTRY_H_

#include <clay-core/base/ModuleDescriptor.h>
#include <clay-core/registry/ModuleRegistry.h>

NAMESPACE_CLAY_NUGGET_BEGIN

class NuggetModuleRegistry : public CLAY::ModuleRegistry
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
    typedef boost::mpl::begin<T>  tIterator;
    const size_t n = boost::mpl::size<T>::type::value;
    AddModuleHelper<tIterator, n>::addModule(this);
  }

private:
  //meta-programming helper classes for iterating boost::list instance - middle of recursion
  template<typename T, size_t size>
  class AddModuleHelper
  {
  public:
    static void addModule(NuggetModuleRegistry* pRegistry)
    {
      typedef typename boost::mpl::deref<typename T::type>::type tCurrentNuggetItem; //is of type NuggetItem<x,y>
      typedef typename tCurrentNuggetItem::tModule tModule;
      pRegistry->registerModule<tModule>();

      typedef typename boost::mpl::next<typename T::type> tNext;

      AddModuleHelper<tNext, size-1>::addModule(pRegistry);          
    }
  };

  //end of recursion
  template<typename T>
  class AddModuleHelper<T, 0>
  {
  public:
    static void addModule(NuggetModuleRegistry*){ /* nothing to do here */ }
  };

private: 
};

NAMESPACE_CLAY_NUGGET_END

#endif
