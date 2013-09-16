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

#ifndef CLAYNUGGET_H_
#define CLAYNUGGET_H_

#ifndef CLAY_MODULE_BUNDLE_NUGGET
#error "CLAY_MODULE_BUNDLE_NUGGET, must be defined (must be of type: boost::mpl::list<Module_1, Module_2, ...>)"
#endif

#include "IClayNugget.h"
#include "NuggetModuleRegistry.h"
#include "NuggetModuleUIRegistry.h"

NAMESPACE_CLAY_NUGGET_BEGIN

//---------------------------------------------nugget entry point
extern "C"
{
  CLAY_DLL_EXPORT CLAY::NUGGET::IClayNugget* getNugget();
};

//---------------------------------------------ClayNugget

class ClayNugget : public IClayNugget
{
public:
  //---------------------------------------------init
  void init()
  {
    
  }

  //---------------------------------------------deInit
  void deInit()
  {

  }

  //---------------------------------------------getModuleRegistry
  virtual const char* getModuleURI(unsigned int uIdx)
  {
    return m_aModuleRegistry.getModuleURI(uIdx);
  }

  //---------------------------------------------
  virtual tModuleFactory getModuleFactory(unsigned int uIdx)
  {
    return m_aModuleRegistry.getModuleFactory(uIdx);
  }

  //---------------------------------------------getModuleUIRegistry
  virtual UI::ModuleUIDescriptor* getModuleUIDescriptor(unsigned int uIdx)
  {
    return m_aModuleUIRegistry.getModuleUIDescriptor(uIdx);
  }

  //---------------------------------------------getNumModules
  virtual unsigned int getNumModules()
  {
    return m_aModuleRegistry.getNumRegisteredModules();
  }

  //---------------------------------------------getNumModuleUIs
  virtual unsigned int getNumModuleUIs()
  {
    return m_aModuleUIRegistry.getNumModuleUIs();
  }

  //---------------------------------------------init
  template<class T>
  void init()
  {
    this->init();
    m_aModuleRegistry.init<T>();
    m_aModuleUIRegistry.init<T>();
  }

private:
  NuggetModuleRegistry   m_aModuleRegistry;
  NuggetModuleUIRegistry m_aModuleUIRegistry;
};

//---------------------------------------------getNugget() - implementation
CLAY::NUGGET::IClayNugget* getNugget()
{
  static CLAY::NUGGET::ClayNugget* pNugget = NULL;
  if(!pNugget)
  {
    static CLAY::NUGGET::ClayNugget aNugget; //singleton instance for this nugget
    pNugget = &aNugget;

    typedef CLAY_MODULE_BUNDLE_NUGGET tList; //see defines on the top of this file
    pNugget->init<tList>();
  }

  return pNugget;
}

NAMESPACE_CLAY_NUGGET_END

#endif
