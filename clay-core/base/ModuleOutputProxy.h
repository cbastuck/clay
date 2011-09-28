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

#ifndef MODULEOUTPUTPROXY_H_
#define MODULEOUTPUTPROXY_H_

#define XML_MASTER_OUPUT_ATTR "master-output"

#include <clay-core/helper/XMLHelper.h>

namespace CLAY{

template<class T>
class TypedModuleOutput;

//---------------------------------------------ModuleOutputProxyBase
class ModuleOutputProxyIO
{
public:
  static bool getProxyMaster(XERCES::DOMElement* pParent, tString& sDst);
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------ModuleOutputProxy
inline bool ModuleOutputProxyIO::getProxyMaster(XERCES::DOMElement* pParent, tString& sDst)
{
  return HELPER::XercesXML::getAttributeValue(pParent, XML_MASTER_OUPUT_ATTR, sDst);
}

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------ModuleOutputProxy
template<class T>
class ModuleOutputProxy : public TypedModuleOutput<T>
{
private:
  typedef TypedModuleOutput<T> tBase;

public:
  ModuleOutputProxy(TypedModuleOutput<T>& aMaster);

  virtual bool save(XERCES::DOMElement* pParent) const;

private:
  TypedModuleOutput<T>* m_pOrigin;
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------ModuleOutputProxy
template<class T>
inline ModuleOutputProxy<T>::ModuleOutputProxy(TypedModuleOutput<T>& aMaster)
  : tBase(&aMaster.getData()),
    m_pOrigin(&aMaster)
{
}

//---------------------------------------------save
template<class T>
inline bool ModuleOutputProxy<T>::save(XERCES::DOMElement* pParent) const
{
  HELPER::XercesXML::addAttribute(pParent, XML_MASTER_OUPUT_ATTR, m_pOrigin->getName());
  return tBase::save(pParent);
}

}

#endif 
