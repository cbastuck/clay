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

#ifndef MODULEUIDESCRIPTOR_H_
#define MODULEUIDESCRIPTOR_H_

#include <clay-core/base/ClayDefines.h>
#include <clay-core/base/ModuleDescriptor.h>

#include <boost/function.hpp>

namespace CLAY{
class Host;
class Module;
}

namespace CLAY{ namespace UI{

class ModuleWidget;
class ModuleUIRegistry;

//------------------------------------ModuleUIDescriptorTraits
class ModuleUIDescriptorTraits
{
public:
  //typedef boost::function3<ModuleWidget*, Module*, Host*, void*> tFactoryMethod;
  //typedef boost::function1<void, ModuleWidget*>                  tDestroyMethod;

  typedef ModuleWidget* (*tFactoryMethod)(Module*, Host*, void*);
  typedef void          (*tDestroyMethod)(ModuleWidget*);
};

//------------------------------------ModuleUIDescriptor
class ModuleUIDescriptor
{
public:
  ModuleUIDescriptor();

  ModuleUIDescriptor(ModuleUIDescriptorTraits::tFactoryMethod aFactory,
                     ModuleUIDescriptorTraits::tDestroyMethod aDestroy,
                     ModuleDescriptorTraits::tModuleID        aModuleID,
                     ModuleDescriptorTraits::tNamespaceID     aNamespaceID);
  
  ModuleDescriptorTraits::tModuleID    getModuleId() const;
  ModuleDescriptorTraits::tNamespaceID getNamespaceId() const;

  ModuleUIDescriptorTraits::tFactoryMethod getFactory() const;
  ModuleUIDescriptorTraits::tDestroyMethod getDestroyer() const;

  ModuleUIRegistry* getModuleUIProvider() const;

private:
  ModuleUIDescriptorTraits::tFactoryMethod m_aFactory;
  ModuleUIDescriptorTraits::tDestroyMethod m_aDestroy;
  ModuleDescriptorTraits::tModuleID        m_aModuleID;
  ModuleDescriptorTraits::tNamespaceID     m_aNamespaceID;
};

//------------------------------------ModuleUIDescriptor
inline ModuleUIDescriptor::ModuleUIDescriptor()
  : m_aFactory(NULL),
    m_aModuleID(0),
    m_aNamespaceID(0)
{

}

//------------------------------------ModuleUIDescriptor
inline ModuleUIDescriptor::ModuleUIDescriptor(ModuleUIDescriptorTraits::tFactoryMethod aFactory,
                                              ModuleUIDescriptorTraits::tDestroyMethod aDestroy,
                                              ModuleDescriptorTraits::tModuleID        aModuleID,
                                              ModuleDescriptorTraits::tNamespaceID     aNamespaceID)
  : m_aFactory(aFactory),
    m_aDestroy(aDestroy),
    m_aModuleID(aModuleID),
    m_aNamespaceID(aNamespaceID)
{
  
}

//------------------------------------getModuleID
inline ModuleDescriptorTraits::tModuleID ModuleUIDescriptor::getModuleId() const
{
  return m_aModuleID;  
}

//------------------------------------getNamespaceID
inline ModuleDescriptorTraits::tNamespaceID ModuleUIDescriptor::getNamespaceId() const
{
  return m_aNamespaceID;
}

//------------------------------------getFactory
inline ModuleUIDescriptorTraits::tFactoryMethod ModuleUIDescriptor::getFactory() const
{
  return m_aFactory;
}

inline ModuleUIDescriptorTraits::tDestroyMethod ModuleUIDescriptor::getDestroyer() const
{
  return m_aDestroy;
}

} }

#endif 
