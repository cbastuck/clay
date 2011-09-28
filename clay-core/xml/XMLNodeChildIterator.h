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

#ifndef XMLNODECHILDITERATOR_H_
#define XMLNODECHILDITERATOR_H_

#include "XMLNode.h"

namespace CLAY{

class XMLNodeChildIterator
{
public:
  enum IteratorFilter
  {
    VISIT_ALL_NODES,
    VISIT_ELEMENT_NODES
  };

  XMLNodeChildIterator(const XMLNode& aNode);
  XMLNodeChildIterator(const XMLNode& aNode, IteratorFilter eFilter);
  ~XMLNodeChildIterator();

  bool hasNext() const;
  XMLNodeChildIterator& operator++();

  XMLNode operator*();

protected:
  void advance();

private:
  XERCES::DOMNodeList* m_pChildNodes;
  int                  m_iCurrentChildIdx;
  IteratorFilter       m_eFilter;
};

}

#endif
