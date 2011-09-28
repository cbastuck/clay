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

#ifndef XMLHelper_H_
#define XMLHelper_H_

#include <clay-core/base/ClayDefines.h>
#include <clay-core/xml/ClayXML.h>

//XERCES
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMElement.hpp>

//STL
#include <vector>

namespace CLAY{ namespace HELPER{

// ---------------------------------------------------------------------------
//  This is a simple class that lets us do easy (though not terribly efficient)
//  trancoding of char* data to XMLCh data.
// ---------------------------------------------------------------------------
class XStr
{
public:
  CLAY_DLL_EXPORT XStr(const char* const toTranscode);
  CLAY_DLL_EXPORT ~XStr();
  CLAY_DLL_EXPORT bool operator==(const XStr&  rhs);
  CLAY_DLL_EXPORT bool operator==(const XMLCh* pRhs);
  CLAY_DLL_EXPORT const XMLCh* unicodeForm() const;
private :
  XMLCh*   fUnicodeForm;
};

// ---------------------------------------------------------------------------
//  This is a simple class that lets us do easy (though not terribly efficient)
//  trancoding of XMLCh data to local code page for display.
// ---------------------------------------------------------------------------
class StrX
{
public :
  CLAY_DLL_EXPORT StrX(const XMLCh* const toTranscode);
  CLAY_DLL_EXPORT ~StrX();

  CLAY_DLL_EXPORT const char* localForm() const;
private :
  char*   fLocalForm;
};

#define S2X(str) CLAY::HELPER::XStr(str).unicodeForm()
#define X2S(str) CLAY::HELPER::StrX(str).localForm()

//---------------------------------------------------------------------------

class XercesXML
{
public:
  typedef std::vector<char> tBinaryBuffer;

  CLAY_DLL_EXPORT static void init();
  CLAY_DLL_EXPORT static void deInit();
      
  CLAY_DLL_EXPORT static XERCES::DOMDocument* createDOMDoc(const char* pNodeName);
  CLAY_DLL_EXPORT static XERCES::DOMDocument* createDOMDoc(const XMLCh* pNodeName);
  CLAY_DLL_EXPORT static XERCES::DOMDocument* createDOMDoc(const char* pBuffer, unsigned int uNumBytes);
  CLAY_DLL_EXPORT static XERCES::DOMElement*  appendNode  (XERCES::DOMElement* pParent, const char* pChildName);
          
  CLAY_DLL_EXPORT static void addAttribute(XERCES::DOMElement* pNode, const char* pKey, const char* pValue);
  CLAY_DLL_EXPORT static void addAttribute(XERCES::DOMElement* pNode, const char* pKey, const tString& sValue);
  CLAY_DLL_EXPORT static void addAttribute(XERCES::DOMElement* pNode, const char* pKey, float fValue);
  CLAY_DLL_EXPORT static void addAttribute(XERCES::DOMElement* pNode, const char* pKey, int iValue);
  CLAY_DLL_EXPORT static void addAttribute(XERCES::DOMElement* pNode, const char* pKey, unsigned int uValue);
  CLAY_DLL_EXPORT static void addBinaryAttribute(XERCES::DOMElement* pNode, const char* pKey, const char* pBuffer, unsigned int uSize);
  CLAY_DLL_EXPORT static bool nodeHasName (XERCES::DOMElement* pNode, const char* pCmpName);
  CLAY_DLL_EXPORT static bool getNodeValue(XERCES::DOMNode* pNode, tString& sDst);

  CLAY_DLL_EXPORT static bool getAttributeValue(XERCES::DOMElement* pNode, const char* pAttrName, tString& sDst);
  CLAY_DLL_EXPORT static bool getAttributeValue(XERCES::DOMElement* pNode, const char* pAttrName, float&   fDst);
  CLAY_DLL_EXPORT static bool getAttributeValue(XERCES::DOMElement* pNode, const char* pAttrName, unsigned int& uDst);
  CLAY_DLL_EXPORT static bool getBinaryAttributeValue(XERCES::DOMElement* pNode, const char* pAttrName, tBinaryBuffer& aDst);
      
  CLAY_DLL_EXPORT static XERCES::DOMElement* findChildNode(XERCES::DOMElement* pParent, const char* pChildName, bool bRecursive = false);
  CLAY_DLL_EXPORT static XERCES::MemBufFormatTarget* serializeToMemory(XERCES::DOMElement* pNode);
  CLAY_DLL_EXPORT static bool saveDOMElement(XERCES::DOMElement* pNode, const std::string& sFileName);
  CLAY_DLL_EXPORT static XERCES::DOMDocument* loadXML(const tString& sFilename);

  CLAY_DLL_EXPORT static void encodeBinaryToBase64(const char* pInputBuffer, unsigned int uInputSize, tString& sDst);
  CLAY_DLL_EXPORT static void decodeBase64ToBinary(const tString& sInputStr, tBinaryBuffer& collDstBinaryBuffer);

  template<class tVisitee>
  CLAY_DLL_EXPORT static void visitChildNodes(XERCES::DOMElement* pParent, tVisitee aCallback, XERCES::DOMNode::NodeType eType = XERCES::DOMNode::ELEMENT_NODE);
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------visitChildNodes
template<class tVisitee>
inline void XercesXML::visitChildNodes(XERCES::DOMElement* pParent, tVisitee aCallback, XERCES::DOMNode::NodeType eType)
{
  XERCES::DOMNodeList* pCollChildNodes = pParent->getChildNodes();
  for(unsigned int i=0, n=pCollChildNodes->getLength(); i<n; ++i)
  {
    XERCES::DOMElement* pElement = static_cast<XERCES::DOMElement*>(pCollChildNodes->item(i));
    if(pElement->getNodeType() == eType)
    {
      aCallback(pElement);
    }
  }
}

} }

#endif 
