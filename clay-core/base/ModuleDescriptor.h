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

#ifndef ModuleDescriptor_H_
#define ModuleDescriptor_H_

#include <clay-core/base/ClayDefines.h>

#include <clay-core/helper/Types.h>

namespace CLAY
{
  class Module;

  //------------------------------------ModuleDescriptorTraits
  class ModuleDescriptorTraits
  {
  public:
    typedef Module* (*tFactoryMethod)(const tString& sId);
    typedef void    (*tDestroyMethod)(Module* pModule);
    typedef unsigned int tModuleID;
    typedef unsigned int tNamespaceID;
    typedef unsigned long long tCompoundIdentifier; //can hold namespace- and module-id

    //-------------------------------
    static tCompoundIdentifier encode(ModuleDescriptorTraits::tModuleID    aModuleId,
                                      ModuleDescriptorTraits::tNamespaceID aNamespaceId)
    {
      unsigned long long aCompound = 0; //init
      aCompound = aNamespaceId;
      aCompound = aCompound << (sizeof(ModuleDescriptorTraits::tNamespaceID) * 8);
      aCompound += aModuleId;
      return aCompound;
    }

    static void decode(const tCompoundIdentifier& aSrc,
                       tModuleID&                 aModuleIdDst,
                       tNamespaceID&              aNamespaceIdDst)
    {
      aModuleIdDst    = aSrc & 0x00000000FFFFFFFF;
      aNamespaceIdDst = (aSrc >> (sizeof(ModuleDescriptorTraits::tNamespaceID) * 8));
    }
    enum{ defaultNamespace = HELPER::IntegerEncoding<'M','I','S','C'>::value };
  };

  //------------------------------------ModuleDescriptorBase
  class ModuleDescriptorBase
  {
  public:
    ModuleDescriptorBase();

    ModuleDescriptorBase(ModuleDescriptorTraits::tFactoryMethod aFactory,
                         ModuleDescriptorTraits::tDestroyMethod aDestroyer,
                         ModuleDescriptorTraits::tModuleID      aModuleID,
                         ModuleDescriptorTraits::tNamespaceID   aNamespaceID);

    ModuleDescriptorTraits::tModuleID      getModuleId() const;

    ModuleDescriptorTraits::tNamespaceID   getNamespaceId() const;

    tString getModuleIdAsString() const;

    tString getNamespaceIdAsString() const;

    ModuleDescriptorTraits::tDestroyMethod getDestroyer() const;

    Module* create(const tString& sRuntimeModuleId) const;

  protected:
    ModuleDescriptorTraits::tFactoryMethod getFactory();

  private:
    ModuleDescriptorTraits::tFactoryMethod m_aFactory;
    ModuleDescriptorTraits::tDestroyMethod m_aDestroyer;
    ModuleDescriptorTraits::tModuleID      m_aModuleID;
    ModuleDescriptorTraits::tNamespaceID   m_aNamespaceID;
  };

  //------------------------------------ModuleDescriptor
  template< ModuleDescriptorTraits::tModuleID      aModuleId,
            ModuleDescriptorTraits::tFactoryMethod aFactory,
            ModuleDescriptorTraits::tDestroyMethod aDestroy,
            ModuleDescriptorTraits::tNamespaceID   aNamespaceId = ModuleDescriptorTraits::defaultNamespace >
  class ModuleDescriptor : public ModuleDescriptorBase
  {
  public:
    typedef ModuleDescriptor<aModuleId, 
                             aFactory, 
                             aDestroy, 
                             aNamespaceId> tThis;
    typedef ModuleDescriptorBase           tBase;

    enum 
    {
      eModuleId    = aModuleId,
      eNamespaceId = aNamespaceId
    };

    static Module* create(const tString& sRuntimeModuleId)      { return aFactory(sRuntimeModuleId); }
    static ModuleDescriptorTraits::tModuleID getModuleId()      { return aModuleId; }
    static ModuleDescriptorTraits::tNamespaceID getNamespaceId(){ return aNamespaceId; }

    ModuleDescriptor() 
      : tBase(aFactory, aDestroy, aModuleId, aNamespaceId)
    {

    }
  private:
  };

  //------------------------------------
  //------------------------------------inline implementations 
  //------------------------------------

  //------------------------------------ModuleDescriptorBase
  inline ModuleDescriptorBase::ModuleDescriptorBase()
    : m_aFactory(NULL),
      m_aDestroyer(NULL),
      m_aModuleID(0),
      m_aNamespaceID(0)
  {

  }

  //------------------------------------ModuleDescriptorBase
  inline ModuleDescriptorBase::ModuleDescriptorBase(ModuleDescriptorTraits::tFactoryMethod aFactory,
                                                    ModuleDescriptorTraits::tDestroyMethod aDestroyer,
                                                    ModuleDescriptorTraits::tModuleID      aModuleID,
                                                    ModuleDescriptorTraits::tNamespaceID   aNamespaceID)
    : m_aFactory(aFactory),
      m_aDestroyer(aDestroyer),
      m_aModuleID(aModuleID),
      m_aNamespaceID(aNamespaceID)
  {

  }

  //------------------------------------getModuleId
  inline ModuleDescriptorTraits::tModuleID ModuleDescriptorBase::getModuleId() const
  {
    return m_aModuleID;
  }

  //------------------------------------getNamespaceId
  inline ModuleDescriptorTraits::tNamespaceID ModuleDescriptorBase::getNamespaceId() const
  {
    return m_aNamespaceID;
  }

  //------------------------------------getModuleIdAsString
  inline tString ModuleDescriptorBase::getModuleIdAsString() const
  {
    return HELPER::IntegerDecoding::toString(&m_aModuleID);
  }

  //------------------------------------getNamespaceIdAsString
  inline tString ModuleDescriptorBase::getNamespaceIdAsString() const
  {
    return HELPER::IntegerDecoding::toString(&m_aNamespaceID);
  }

  //------------------------------------getDestroyer
  inline ModuleDescriptorTraits::tDestroyMethod ModuleDescriptorBase::getDestroyer() const
  {
    return m_aDestroyer;
  }

  //------------------------------------create
  inline Module* ModuleDescriptorBase::create(const tString& sRuntimeModuleId) const
  {
    return m_aFactory(sRuntimeModuleId);
  }

  //------------------------------------getFactory
  inline ModuleDescriptorTraits::tFactoryMethod ModuleDescriptorBase::getFactory()
  {
    return m_aFactory;
  }
}

#endif
