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

#include <clay-core/base/ClayDefines.h>

#ifndef ICLAYNUGGET_H_
#define ICLAYNUGGET_H_

#include <boost/function.hpp>

//currently using boost::mpl::list to store the modules in the bundle
#include <boost/mpl/list.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/size.hpp>

//hide to the outside that we are using boost::mpl::list (too spooky)
#define MODULE_LIST(x) boost::mpl::list<x>

namespace CLAY{
class Module;
class Host;
}

namespace CLAY{ namespace UI{
class ModuleWidget;
} }

//items of mpl::list are of type NuggetItem (bundles module and corresponding UI)
namespace CLAY{ namespace NUGGET{

class NuggetItemNoUI
{
public:
  static UI::ModuleWidget* create(Module*, Host*, void*){ return NULL; }
  static void destroy(UI::ModuleWidget*){}
}; //empty class - used as default UI type

template<class MODULE, class UI=NuggetItemNoUI>
class NuggetItem
{
public:
  typedef MODULE tModule;
  typedef UI     tUI;
};

} }

#define NUGGET_ITEM_MODULE_UI(x,y) CLAY::NUGGET::NuggetItem<x,y>
#define NUGGET_ITEM_MODULE(x)      CLAY::NUGGET::NuggetItem<x>

namespace CLAY{
class Module;
class ModuleDescriptorBase;
}

namespace CLAY{ namespace UI{
class ModuleUIDescriptor;
} }

namespace CLAY{ namespace NUGGET{

typedef boost::function<Module*(const tString&)> tModuleFactory;

//---------------------------------------------IClayNugget
class IClayNugget
{
public:
  virtual ~IClayNugget(){}
  virtual void                          init()                                   = 0;
  virtual void                          deInit()                                 = 0;
  virtual unsigned int                  getNumModules()                          = 0;
  virtual unsigned int                  getNumModuleUIs()                        = 0;
  virtual const char*                   getModuleURI(unsigned int uIdx)          = 0;
  virtual tModuleFactory                getModuleFactory(unsigned int uIdx)      = 0;
  virtual const UI::ModuleUIDescriptor* getModuleUIDescriptor(unsigned int uIdx) = 0;
};

} }

#endif
