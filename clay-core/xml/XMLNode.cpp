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

#include "XMLNode.h"

#include "XMLNodeChildIterator.h"

#include <clay-core/helper/XMLHelper.h>
#include <clay-core/helper/LexicalConversion.h>

#include <xercesc/dom/DOMElement.hpp>

namespace CLAY{

//---------------------------------------------XMLNode
XMLNode::XMLNode(XERCES::DOMNode* pNode)
  : m_pNode(pNode)
{

}

//---------------------------------------------~XMLNode
XMLNode::~XMLNode()
{

}

//---------------------------------------------
XMLNode::operator bool()
{
  return m_pNode ? true : false;
}

//---------------------------------------------getNodeName
bool XMLNode::getNodeName(tString& sDst)
{
  if(!m_pNode)
  {
    return false;
  }

  sDst = X2S(m_pNode->getNodeName());
  return true;
}

//---------------------------------------------addChildNode
XMLNode XMLNode::addChildNode(const tString& sName)
{
  if(!m_pNode)
  {
    return NULL;
  }

  if(m_pNode->getNodeType() != XERCES::DOMNode::ELEMENT_NODE)
  {
    return NULL;
  }

  return HELPER::XercesXML::appendNode(static_cast<XERCES::DOMElement*>(m_pNode), sName.c_str());
}

//---------------------------------------------
XMLNode XMLNode::getFirstChildNode()
{
  if(!m_pNode)
   {
     return NULL;
   }

  XERCES::DOMNodeList* pChilds = m_pNode->getChildNodes();
  if(!pChilds->getLength())
  {
    return NULL;
  }

  return pChilds->item(0);
}

//---------------------------------------------
bool XMLNode::getFirstChildValue(tString& sDst)
{
  XMLNode aItem = getFirstChildNode();
  if(!aItem)
  {
    return false;
  }

  return HELPER::XercesXML::getNodeValue(aItem.m_pNode, sDst);
}

//---------------------------------------------
XMLNode XMLNode::findChildNode(const tString& sName, bool bRecursive)
{
  if(!m_pNode)
  {
    return NULL;
  }

  return HELPER::XercesXML::findChildNode(static_cast<XERCES::DOMElement*>(m_pNode), sName.c_str(), bRecursive);
}

//---------------------------------------------addAttribute
bool XMLNode::addAttribute(const tString& sKey, const tString& sValue)
{
  if(!m_pNode)
  {
    return false;
  }

  if(m_pNode->getNodeType() != XERCES::DOMNode::ELEMENT_NODE)
  {
    return false;
  }

  HELPER::XercesXML::addAttribute(static_cast<XERCES::DOMElement*>(m_pNode), sKey.c_str(), sValue.c_str());

  return true; //TODO: return the return value from forwarded method (actually returns void)
}

//---------------------------------------------addAttribute
bool XMLNode::addAttribute(const tString& sKey, int iValue)
{
  return addAttribute(sKey, CLAY::HELPER::LexicalConversion::toString(iValue).c_str());
}

//---------------------------------------------addAttribute
bool XMLNode::addAttribute(const tString& sKey, float fValue)
{
  return addAttribute(sKey, CLAY::HELPER::LexicalConversion::toString(fValue).c_str());
}

//---------------------------------------------addAttribute
bool XMLNode::addAttribute(const tString& sKey, const char* pBuffer, unsigned int uSize)
{
  tString sEncoded;
  ClayXML::encodeBinaryToBase64(pBuffer, uSize, sEncoded);
  return addAttribute(sKey, sEncoded.c_str());
}

//---------------------------------------------
bool XMLNode::getAttribute(const tString& sKey, tString& sDst) const
{
  if(!m_pNode)
  {
    return false;
  }

  if(m_pNode->getNodeType() != XERCES::DOMNode::ELEMENT_NODE)
  {
    return false;
  }

  return HELPER::XercesXML::getAttributeValue(static_cast<XERCES::DOMElement*>(m_pNode), sKey.c_str(), sDst);
}

}
