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

#ifndef CLAY_XML_H_
#define CLAY_XML_H_

#include <clay-core/base/ClayDefines.h>

//STL includes
#include <vector>

/*
 * we are using this Xerces version currently in order to have no dependency to
 * the Xerces sources when including this file, we define the namespace here
 * but keep in mind:
 *
 * YOU MUST CHANGE THIS NAMESPACE WHEN CHANGING THE USED XERCES VERSION!
 */

#define XERCES xercesc_3_1

//forward declarations to XERCES classes
namespace XERCES
{
  class DOMNode;
  class DOMDocument;
  class DOMElement;
  class DOMLSParser;
  class XercesDOMParser;
  class DOMNodeList;
  class MemBufFormatTarget;
}

//xerces wrapper classes
namespace CLAY
{
  class XMLNode;
  class XMLBuffer;
  class XMLDocument;
  class XMLNodeChildIterator;
}

/*
 * Use this class to before using clay XML types in order to initialize
 * and de-initialize the classes.
 * It is safe to call init() and deInit() multiple times even if it
 * is already initialized!
 */
namespace CLAY{

class ClayXML
{
public:
  enum XMLImplementation
  {
    CLAY_XML_XERCES_IMPL = 0,
  };

  CLAY_DLL_EXPORT static XMLImplementation getImplementation();

  CLAY_DLL_EXPORT static void init();
  CLAY_DLL_EXPORT static void deInit();

  static void encodeBinaryToBase64(const char* pInputBuffer, unsigned int uInputSize, tString& sDst);
  static void decodeBase64ToBinary(const tString& sInputStr, std::vector<char>& collDstBinaryBuffer);
protected:
  static bool setInitialized(bool bStatus);
};

}

#endif 
