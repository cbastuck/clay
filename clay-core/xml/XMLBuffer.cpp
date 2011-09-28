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

#include "XMLBuffer.h"

#include <xercesc/framework/MemBufFormatTarget.hpp>

namespace CLAY {

//---------------------------------------------
XMLBuffer::XMLBuffer()
: m_pBuffer(NULL),
  m_uBufferSize(0)
{

}

//---------------------------------------------
XMLBuffer::~XMLBuffer()
{
  if(m_pBuffer)
  {
    delete[] m_pBuffer;
  }
}

//---------------------------------------------
unsigned int XMLBuffer::getSize() const
{
  if(!m_pBuffer)
  {
    return 0;
  }

  return m_uBufferSize;
}

//---------------------------------------------
const void* XMLBuffer::getData() const
{
  if(!m_pBuffer)
  {
    return NULL;
  }

  return m_pBuffer;
}

//---------------------------------------------
void XMLBuffer::print(std::ostream& os)
{
  for(unsigned int i=0; i<m_uBufferSize; ++i)
  {
    os << m_pBuffer[i];
    if(i % 20 == 0)
    {
      os << std::endl;
    }
  }
}

//---------------------------------------------
XMLBuffer& XMLBuffer::operator=(XERCES::MemBufFormatTarget* pBuffer)
{
  if(m_pBuffer)
  {
    delete[] m_pBuffer;
    m_pBuffer = NULL;
    m_uBufferSize = 0;
  }

  m_uBufferSize = pBuffer->getLen();
  m_pBuffer = new char[m_uBufferSize];
  memcpy(m_pBuffer, pBuffer->getRawBuffer(), m_uBufferSize);

  return *this;
}

}
