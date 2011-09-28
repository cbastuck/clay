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

#ifndef XMLNODE_H_
#define XMLNODE_H_

#include <clay-core/xml/ClayXML.h>

namespace CLAY{

class XMLNodeChildIterator;

class XMLNode
{
public:
  XMLNode(XERCES::DOMNode* pNode);
  ~XMLNode();

  operator bool();

  bool getNodeName(tString& sDst);

  XMLNode addChildNode(const tString& sName);
  XMLNode getFirstChildNode();
  XMLNode findChildNode(const tString& sName, bool bRecursive=false);
  bool getFirstChildValue(tString& sDst);

  bool addAttribute(const tString& sKey, const tString& sValue);
  bool addAttribute(const tString& sKey, int iValue);
  bool addAttribute(const tString& sKey, float fValue);
  bool addAttribute(const tString& sKey, const char* pBuffer, unsigned int uSize);

  bool getAttribute(const tString& sKey, tString& sDst) const;

private:
  friend class XMLNodeChildIterator;
  XERCES::DOMNode* m_pNode;
};

}

#endif 
