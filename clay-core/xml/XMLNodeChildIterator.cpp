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

#include "XMLNodeChildIterator.h"

#include <clay-core/xml/ClayXML.h>
#include <clay-core/xml/XMLNodeChildIterator.h>

//shortcuts and defines for xerces
#include <xercesc/util/Base64.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMNodeList.hpp>

namespace CLAY{

//---------------------------------------------XMLNodeChildIterator
XMLNodeChildIterator::XMLNodeChildIterator(const XMLNode& aNode)
  : m_pChildNodes(NULL),
    m_iCurrentChildIdx(-1),
    m_eFilter(VISIT_ALL_NODES)
{
  m_pChildNodes = aNode.m_pNode->getChildNodes();
  advance();
}

//---------------------------------------------XMLNodeChildIterator
XMLNodeChildIterator::XMLNodeChildIterator(const XMLNode& aNode, IteratorFilter eFilter)
  : m_pChildNodes(NULL),
    m_iCurrentChildIdx(-1),
    m_eFilter(eFilter)
{
  m_pChildNodes = aNode.m_pNode->getChildNodes();
  advance();
}

//---------------------------------------------~XMLNodeChildIterator
XMLNodeChildIterator::~XMLNodeChildIterator()
{

}

//---------------------------------------------advance
void XMLNodeChildIterator::advance()
{
  for(bool bAgain=true; hasNext() && bAgain; )
  {
    ++m_iCurrentChildIdx;

    if(m_eFilter == VISIT_ALL_NODES)
    {
      bAgain = false;
    }
    else
    {
      if(m_eFilter == VISIT_ELEMENT_NODES)
      {
        XERCES::DOMNode* pNode = m_pChildNodes->item(m_iCurrentChildIdx);
        bAgain = (pNode->getNodeType() != XERCES::DOMNode::ELEMENT_NODE);
      }
    }
  }
}

//---------------------------------------------hasNext
bool XMLNodeChildIterator::hasNext() const
{
  if(!m_pChildNodes)
  {
    return false;
  }

  int iNumChildNodes = m_pChildNodes->getLength();
  if(!iNumChildNodes)
  {
    return false;
  }

  return iNumChildNodes > (m_iCurrentChildIdx+1);
}

//---------------------------------------------operator++
XMLNodeChildIterator& XMLNodeChildIterator::operator++()
{
  advance();
  return *this;
}

//---------------------------------------------operator*
XMLNode XMLNodeChildIterator::operator*()
{
  XERCES::DOMNode* pNode = m_pChildNodes->item(m_iCurrentChildIdx);
  return XMLNode(pNode);
}

}
