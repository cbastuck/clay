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

#ifndef MODULEHELPER_H_
#define MODULEHELPER_H_

#include <clay-core/base/Module.h>
#include <clay-core/base/ModuleDescriptor.h>

namespace CLAY{

template<class tDerived,
          ModuleDescriptorTraits::tModuleID    aModuleId,
          ModuleDescriptorTraits::tNamespaceID aNamespaceId  = ModuleDescriptorTraits::defaultNamespace>
class ModuleHelper : public Module
{
public:
  static Module* create(const tString& sId);
  static void destroy(Module* pModule);

  typedef Module tBase;
  typedef ModuleHelper<tDerived, aModuleId, aNamespaceId> tThis;
  typedef ModuleDescriptor<aModuleId, tThis::create, tThis::destroy, aNamespaceId> tDescriptor;

  ModuleHelper(const tString& sId);

  virtual bool init(XERCES::DOMNode* pNode);
  virtual void deInit();

  virtual void registerModuleOutputs();
  virtual void registerModuleInputs();

  virtual ProcessResultCode process();

  virtual const ModuleDescriptorBase* getModuleDescriptor() const;

private:
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------create
template<class tDerived,
         ModuleDescriptorTraits::tModuleID    aModuleId,
         ModuleDescriptorTraits::tNamespaceID aNamespaceId>
Module* ModuleHelper<tDerived, aModuleId, aNamespaceId>::create(const tString& sId)
{ 
  return new tDerived(sId); //factory method
} 

//---------------------------------------------destroy
template<class tDerived,
         ModuleDescriptorTraits::tModuleID    aModuleId,
         ModuleDescriptorTraits::tNamespaceID aNamespaceId>
void ModuleHelper<tDerived, aModuleId, aNamespaceId>::destroy(Module* pModule)
{
  delete pModule; //destroy method
} 

//---------------------------------------------ModuleHelper
template<class tDerived,
         ModuleDescriptorTraits::tModuleID    aModuleId,
         ModuleDescriptorTraits::tNamespaceID aNamespaceId>
ModuleHelper<tDerived, aModuleId, aNamespaceId>::ModuleHelper(const tString& sId)
  : tBase(sId)
{

}

//---------------------------------------------init
template<class tDerived,
         ModuleDescriptorTraits::tModuleID    aModuleId,
         ModuleDescriptorTraits::tNamespaceID aNamespaceId>
bool ModuleHelper<tDerived, aModuleId, aNamespaceId>::init(XERCES::DOMNode* pNode)
{
  bool bRes = tBase::init(pNode);
  registerModuleOutputs();
  registerModuleInputs();
  return bRes;
}

//---------------------------------------------deInit
template<class tDerived,
         ModuleDescriptorTraits::tModuleID    aModuleId,
         ModuleDescriptorTraits::tNamespaceID aNamespaceId>
void ModuleHelper<tDerived, aModuleId, aNamespaceId>::deInit()
{
  tBase::deInit();
}


//---------------------------------------------registerModuleOutputs
template<class tDerived,
         ModuleDescriptorTraits::tModuleID    aModuleId,
         ModuleDescriptorTraits::tNamespaceID aNamespaceId>
void ModuleHelper<tDerived, aModuleId, aNamespaceId>::registerModuleOutputs()
{
}

//---------------------------------------------registerModuleInputs
template<class tDerived,
         ModuleDescriptorTraits::tModuleID    aModuleId,
         ModuleDescriptorTraits::tNamespaceID aNamespaceId>
void ModuleHelper<tDerived, aModuleId, aNamespaceId>::registerModuleInputs()
{
}

//---------------------------------------------process
template<class tDerived,
         ModuleDescriptorTraits::tModuleID    aModuleId,
         ModuleDescriptorTraits::tNamespaceID aNamespaceId>
Module::ProcessResultCode ModuleHelper<tDerived, aModuleId, aNamespaceId>::process()
{
  return Module::Process_OK; 
}

//---------------------------------------------getModuleDescriptor
template<class tDerived,
         ModuleDescriptorTraits::tModuleID    aModuleId,
         ModuleDescriptorTraits::tNamespaceID aNamespaceId>
const ModuleDescriptorBase* ModuleHelper<tDerived, aModuleId, aNamespaceId>::getModuleDescriptor() const
{
  static tDescriptor aDescriptor;
  return &aDescriptor;
}

}

#endif
