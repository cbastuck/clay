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

#include "ClayXML.h"

#include <clay-core/helper/XMLHelper.h>

#include <xercesc/util/Base64.hpp>

namespace CLAY{

//---------------------------------------------
ClayXML::XMLImplementation ClayXML::getImplementation()
{
  return CLAY_XML_XERCES_IMPL;
}

//---------------------------------------------init
void ClayXML::init()
{
  if(setInitialized(true)) //only init if not done before
  {
    CLAY::HELPER::XercesXML::init();
  }
}

//---------------------------------------------deInit
void ClayXML::deInit()
{
  if(setInitialized(false)) //deInit if init was called before
  {
    CLAY::HELPER::XercesXML::deInit();
  }
}

//---------------------------------------------setInitialized
bool ClayXML::setInitialized(bool bStatus)
{
  static bool bInitialized = false;
  if(bInitialized != bStatus)
  {
    bInitialized = bStatus;
    return true; //means the status was updated
  }
  return false; //indicates that the status did not change, i.e. was equal to the request
}

//---------------------------------------------encodeBinaryToBase64
void ClayXML::encodeBinaryToBase64(const char* pInputBuffer,
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
void ClayXML::decodeBase64ToBinary(const tString& sInputStr, std::vector<char>& collDstBinaryBuffer)
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

}
