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

#include "XMLDocument.h"

#include <clay-core/xml/XMLBuffer.h>
#include <clay-core/helper/XMLHelper.h>

//shortcuts and defines for xerces
#include <xercesc/util/Base64.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOMLSSerializer.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/Wrapper4InputSource.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

namespace CLAY{

//---------------------------------------------XMLDocument
XMLDocument::XMLDocument()
  : m_pDocument(NULL)
{

}

//---------------------------------------------~XMLDocument
XMLDocument::~XMLDocument()
{
  if(m_pDocument)
  {
    delete m_pDocument;
  }
}

//---------------------------------------------getDocumentRoot
XMLNode XMLDocument::getDocumentRoot()
{
  if(m_pDocument)
  {
    return m_pDocument->getDocumentElement();
  }
  else
  {
    CLAY_FAIL();
    return NULL;
  }
}

//---------------------------------------------
bool XMLDocument::createRoot(const tString& sStr)
{
  if(m_pDocument)
  {
    delete m_pDocument;
  }

  std::string sRootNode = "<" + sStr + "/>";
  m_pDocument = HELPER::XercesXML::createDOMDoc(sRootNode.c_str(), sRootNode.size());
  
  return m_pDocument ? true : false;
}

//---------------------------------------------readFromFile
bool XMLDocument::readFromFile(const tString& sXMLFilename)
{
  if(m_pDocument)
  {
    delete m_pDocument;
    m_pDocument = NULL;
  }
  m_pDocument = HELPER::XercesXML::loadXML(sXMLFilename);
  
  return m_pDocument ? true : false;
}

//---------------------------------------------writeToFile
bool XMLDocument::writeToFile(const tString& sFileName)
{
  if(!m_pDocument)
  {
    return false;
  }

  return HELPER::XercesXML::saveDOMElement(m_pDocument->getDocumentElement(), sFileName);
}

//---------------------------------------------readFromMemory
bool XMLDocument::readFromMemory(const char* pBuffer, unsigned int uNumBytes)
{
  if(m_pDocument)
  {
    delete m_pDocument;
    m_pDocument = NULL;
  }

  m_pDocument = HELPER::XercesXML::createDOMDoc(pBuffer, uNumBytes);

  return m_pDocument ? true : false;
}

//---------------------------------------------
bool XMLDocument::writeToMemory(XMLBuffer& aDst)
{
  if(!m_pDocument)
  {
    return false;
  }

  aDst = HELPER::XercesXML::serializeToMemory(m_pDocument->getDocumentElement());

  return aDst.getSize() > 0;
}

}
