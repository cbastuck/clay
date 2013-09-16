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

#include "XMLHelper.h"

#include <clay-core/helper/LexicalConversion.h>

//XERCES
#include <xercesc/util/Base64.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOMLSSerializer.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/MemBufFormatTarget.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

namespace CLAY{ namespace HELPER{

//---------------------------------------------XStr
XStr::XStr(const char* const toTranscode)
{
  fUnicodeForm = XERCES::XMLString::transcode(toTranscode); //call the private transcoding method
}

//---------------------------------------------~XStr
XStr::~XStr()
{
  XERCES::XMLString::release(&fUnicodeForm);
}

//---------------------------------------------operator==
bool XStr::operator==(const XStr& rhs)
{
  return operator==(rhs.fUnicodeForm);
}

//---------------------------------------------operator==
bool XStr::operator==(const XMLCh* pRhs)
{
  XMLCh* pLhs = fUnicodeForm;
  for(; *pLhs && *pRhs; ++pLhs, ++pRhs)
  {
    if(*pLhs != *pRhs)
    {
      return false;
    }
  }
  return !*pLhs && !*pRhs; //both should be at the end
}

//---------------------------------------------unicodeForm
const XMLCh* XStr::unicodeForm() const
{
  return fUnicodeForm;
}

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------StrX
StrX::StrX(const XMLCh* const toTranscode)
{
  // Call the private transcoding method
  fLocalForm = XERCES::XMLString::transcode(toTranscode);
}

//---------------------------------------------~StrX
StrX::~StrX()
{
  XERCES::XMLString::release(&fLocalForm);
}

//---------------------------------------------localForm
const char* StrX::localForm() const
{
  return fLocalForm;
}


//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
    
//---------------------------------------------init
void XercesXML::init()
{
  XERCES::XMLPlatformUtils::Initialize();
}

//---------------------------------------------deInit
void XercesXML::deInit()
{
  XERCES::XMLPlatformUtils::Terminate();
}

//---------------------------------------------createDOMDoc
XERCES::DOMDocument* XercesXML::createDOMDoc(const char* pNodeName)
{
  return createDOMDoc(S2X(pNodeName));
}

//---------------------------------------------createDOMDoc
XERCES::DOMDocument* XercesXML::createDOMDoc(const XMLCh* pNodeName)
{
  XERCES::DOMImplementation* pImpl = XERCES::DOMImplementationRegistry::getDOMImplementation(S2X("LS"));
  XERCES::DOMDocument* doc = pImpl->createDocument(0,                   // root element namespace URI.
                                                   pNodeName,           // root element name
                                                   0);                  // document type object (DTD).
  return doc;
}

//---------------------------------------------createDOMDoc
XERCES::DOMDocument* XercesXML::createDOMDoc(const char* pBuffer, unsigned int uNumBytes)
{
  XERCES::MemBufInputSource* pMemBufIS = new XERCES::MemBufInputSource((const XMLByte*)pBuffer, 
                                                                        static_cast<const XMLSize_t>(uNumBytes), 
                                                                        "test", 
                                                                        false);
  XERCES::XercesDOMParser* pParser = new XERCES::XercesDOMParser;

  pParser->parse(*pMemBufIS);

  XERCES::DOMDocument* pDoc  = pParser->getDocument();
  XERCES::DOMDocument* pCopy = static_cast<XERCES::DOMDocument*>(pDoc->cloneNode(true));

  delete pParser;
  delete pMemBufIS;

  return pCopy;
}

//---------------------------------------------serializeToMemory
XERCES::MemBufFormatTarget* XercesXML::serializeToMemory(XERCES::DOMElement* pNode)
{
  XERCES::DOMImplementation* pImpl = XERCES::DOMImplementationRegistry::getDOMImplementation(S2X("LS"));

  XERCES::DOMLSSerializer* pSerializer = ((XERCES::DOMImplementationLS*)pImpl)->createLSSerializer(); // construct the DOMWriter
  if(pSerializer->getDomConfig()->canSetParameter(XERCES::XMLUni::fgDOMWRTFormatPrettyPrint, true))
  {
    pSerializer->getDomConfig()->setParameter(XERCES::XMLUni::fgDOMWRTFormatPrettyPrint, true);
  }

  XERCES::DOMLSOutput* pOutput = ((XERCES::DOMImplementationLS*)pImpl)->createLSOutput();

  XERCES::MemBufFormatTarget* pFormatTarget = new XERCES::MemBufFormatTarget(1024);
  pOutput->setByteStream(pFormatTarget);

  pSerializer->write(pNode, pOutput);

  pOutput->release();
  pSerializer->release(); // release the serializer

  return pFormatTarget;
}

//---------------------------------------------saveDOMElement
bool XercesXML::saveDOMElement(XERCES::DOMElement* pNode, const std::string& sFileName)
{
  XERCES::DOMImplementation* pImpl = XERCES::DOMImplementationRegistry::getDOMImplementation(S2X("LS"));

  XERCES::DOMLSSerializer* pSerializer = ((XERCES::DOMImplementationLS*)pImpl)->createLSSerializer(); // construct the DOMWriter
  if(pSerializer->getDomConfig()->canSetParameter(XERCES::XMLUni::fgDOMWRTFormatPrettyPrint, true))
  {
    pSerializer->getDomConfig()->setParameter(XERCES::XMLUni::fgDOMWRTFormatPrettyPrint, true);
  }

  XERCES::DOMLSOutput* pOutput = ((XERCES::DOMImplementationLS*)pImpl)->createLSOutput();

  XERCES::XMLFormatTarget* pFormatTarget = new XERCES::LocalFileFormatTarget(sFileName.c_str());
  pOutput->setByteStream(pFormatTarget);

  pSerializer->write(pNode, pOutput);
  
  pOutput->release();
  pSerializer->release(); // release the serializer

  delete pFormatTarget;

  return true;
}

//---------------------------------------------loadXML
XERCES::DOMDocument* XercesXML::loadXML(const tString& sFilename)
{
  XERCES::DOMImplementation* pImpl = XERCES::DOMImplementationRegistry::getDOMImplementation(S2X("LS"));
      
  static XERCES::DOMLSParser* pParser = NULL; //one parser for every all load operations
  if(!pParser)
  {
    pParser = ((XERCES::DOMImplementationLS*)pImpl)->createLSParser(XERCES::DOMImplementationLS::MODE_SYNCHRONOUS, 0);
  }

  XERCES::DOMDocument* pDoc  = pParser->parseURI(sFilename.c_str());

  return pDoc;
}

//---------------------------------------------appendNode
XERCES::DOMElement* XercesXML::appendNode(XERCES::DOMElement* pParent, const char* pChildName)
{
  XERCES::DOMDocument* pDoc       = pParent->getOwnerDocument();
  XERCES::DOMElement* pModuleRoot = pDoc->createElement(S2X(pChildName));
  pParent->appendChild(pModuleRoot);
  return pModuleRoot;
}

//---------------------------------------------appendBinaryNode
void XercesXML::addBinaryAttribute(XERCES::DOMElement* pNode, const char* pKey, const char* pBuffer, unsigned int uSize)
{
  tString sEncoded;
  encodeBinaryToBase64(pBuffer, uSize, sEncoded);
  addAttribute(pNode, pKey, sEncoded.c_str());
}

//---------------------------------------------addAttribute
void XercesXML::addAttribute(XERCES::DOMElement* pNode, const char* pKey, const char* pValue)
{
  pNode->setAttribute(S2X(pKey), S2X(pValue));
}

//---------------------------------------------addAttribute
void XercesXML::addAttribute(XERCES::DOMElement* pNode, const char* pKey, float fValue)
{
  pNode->setAttribute(S2X(pKey), S2X(LexicalConversion::toString(fValue).c_str()));
}

//---------------------------------------------addAttribute
void XercesXML::addAttribute(XERCES::DOMElement* pNode, const char* pKey, int iValue)
{
  pNode->setAttribute(S2X(pKey), S2X(LexicalConversion::toString(iValue).c_str()));
}

//---------------------------------------------addAttribute
void XercesXML::addAttribute(XERCES::DOMElement* pNode, const char* pKey, unsigned int uValue)
{
  pNode->setAttribute(S2X(pKey), S2X(LexicalConversion::toString(uValue).c_str()));
}

//---------------------------------------------addAttribute
void XercesXML::addAttribute(XERCES::DOMElement* pNode, const char* pKey, const tString& sValue)
{
  addAttribute(pNode, pKey, sValue.c_str());
}

//---------------------------------------------nodeHasName
bool XercesXML::nodeHasName(XERCES::DOMElement* pNode, const char* pCmpName)
{
  CLAY::HELPER::XStr aHelper(pCmpName);
  return aHelper == pNode->getNodeName();
}

//---------------------------------------------
bool XercesXML::getNodeValue(XERCES::DOMNode* pNode, tString& sDst)
{
  const XMLCh* pValue = pNode->getNodeValue();
  if(!pValue)
  {
    return false;
  }

  sDst = X2S(pValue);
  return true;
}

//---------------------------------------------getAttributeValue
bool XercesXML::getAttributeValue(XERCES::DOMElement* pNode, const char* pAttrName, tString& sDst)
{
  XERCES::DOMAttr* pAttr = pNode->getAttributeNode(S2X(pAttrName));
  if(pAttr)
  {
    sDst = X2S(pAttr->getNodeValue());
    return true;
  }
  return false;
}

//---------------------------------------------getAttributeValue
bool XercesXML::getAttributeValue(XERCES::DOMElement* pNode, const char* pAttrName, float& fDst)
{
  tString sDst; 
  if(getAttributeValue(pNode, pAttrName, sDst))
  {
    fDst = LexicalConversion::toNumber<float>(sDst);
    return true;
  }
  return false;
}

//---------------------------------------------getAttributeValue
bool XercesXML::getAttributeValue(XERCES::DOMElement* pNode, const char* pAttrName, unsigned int& uDst)
{
  tString sDst; 
  if(getAttributeValue(pNode, pAttrName, sDst))
  {
    uDst = LexicalConversion::toNumber<unsigned int>(sDst);
    return true;
  }
  return false;
}

//---------------------------------------------getBinaryAttributeValue
bool XercesXML::getAttributeValue(XERCES::DOMElement* pNode, const char* pAttrName, bool& bDst)
{
  tString sDst; 
  if(getAttributeValue(pNode, pAttrName, sDst))
  {
    bDst = (sDst == "1") || (sDst == "true") || (sDst == "True") || (sDst == "TRUE");
    return true;
  }
  return false;
}

//---------------------------------------------getBinaryAttributeValue
bool XercesXML::getBinaryAttributeValue(XERCES::DOMElement* pNode, const char* pAttrName, tBinaryBuffer& aDst)
{
  tString sBase64Encoded;
  if(getAttributeValue(pNode, pAttrName, sBase64Encoded))
  {
    decodeBase64ToBinary(sBase64Encoded, aDst);
    return true;
  }
  return false;
}

//---------------------------------------------findChildNode
XERCES::DOMElement* XercesXML::findChildNode(XERCES::DOMElement* pParent, const char* pChildName, bool bRecursive)
{
  if(bRecursive)
  {
    XERCES::DOMNodeList* pCollChilds = pParent->getElementsByTagName(S2X(pChildName));
    if(pCollChilds->getLength())
    {
      return static_cast<XERCES::DOMElement*>(pCollChilds->item(0));
    }
  }
  else //just search the direct child-nodes
  {
    XERCES::DOMNodeList* pCollChilds = pParent->getChildNodes();
    for(unsigned int i=0, n=pCollChilds->getLength(); i<n; ++i)
    {
      XERCES::DOMElement* pElement = static_cast<XERCES::DOMElement*>(pCollChilds->item(i));
      if(nodeHasName(pElement, pChildName))
        return pElement;
    }
  }
  return NULL;      
}

//---------------------------------------------encodeBinaryToBase64
void XercesXML::encodeBinaryToBase64(const char* pInputBuffer,
                                      unsigned int uInputSize,
                                      tString& sDst)
{
  XMLSize_t iOutputLength = 0;
  XMLByte* pBase64 = XERCES::Base64::encode((const unsigned char*)pInputBuffer, uInputSize, &iOutputLength);

  sDst.resize(iOutputLength);
  char* pOutputData = &sDst[0];

  int iDest = 0;
  for(XMLSize_t i = 0; i < iOutputLength; ++i)
  {
    if(pBase64[i] != XERCES::chLF)
    {
      pOutputData[iDest++] = pBase64[i];
    }
  }

  XERCES::XMLString::release((char **)&pBase64);
}

//---------------------------------------------decodeBase64ToBinary
void XercesXML::decodeBase64ToBinary(const tString& sInputStr, std::vector<char>& collDstBinaryBuffer)
{
  size_t uStrLen = sInputStr.size();
  std::vector<XMLByte> aTerminatedBuf(uStrLen + 1, 0);

  for(size_t uIdx = 0; uIdx < uStrLen; uIdx++)
  {
    aTerminatedBuf[uIdx] = (XMLByte)sInputStr[uIdx];
  }

  aTerminatedBuf[uStrLen] = 0;
  XMLSize_t iOutputLength = 0;
  XMLByte* pResult = XERCES::Base64::decode(&aTerminatedBuf[0], &iOutputLength);
  if (pResult == NULL)
  {
    collDstBinaryBuffer.clear();
    return;
  }
  collDstBinaryBuffer.assign(pResult, pResult + iOutputLength);
  XERCES::XMLString::release((char **)&pResult);
}

} }
