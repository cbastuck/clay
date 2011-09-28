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

#ifndef XML_DOCUMENT_H_
#define XML_DOCUMENT_H_

#include <clay-core/xml/ClayXML.h>

#include "XMLNode.h"

#include <clay-core/base/ModuleIO.h>

namespace CLAY{

//---------------------------------------------XMLDocument
class XMLDocument
{
public:
  XMLDocument();
  ~XMLDocument();

  XMLNode getDocumentRoot();

  bool createRoot(const tString& sStr);

  bool readFromFile(const tString& sXMLFilename);
  bool writeToFile(const tString& sFn);
  bool readFromMemory(const char* pBuffer, unsigned int uNumBytes);
  bool writeToMemory(XMLBuffer& aDst);

private:
  XERCES::DOMDocument* m_pDocument;
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------ClayXMLDocumentOutput
class XMLDocumentOutput : public ModuleOutput<XMLDocument>
{
public:

private:
};

//---------------------------------------------ClayXMLDocumentInput
class XMLDocumentInput : public ModuleInput<XMLDocument>
{
public:

private:
};

}

#endif
