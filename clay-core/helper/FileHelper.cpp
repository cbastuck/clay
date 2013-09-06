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

#include "FileHelper.h"

namespace CLAY{ namespace HELPER{ 

//---------------------------------------------FileStream
FileStream::FileStream(const tString& sFilename)
  : m_sFilename(sFilename)
{

}

//---------------------------------------------~FileStream
FileStream::~FileStream()
{

}

//---------------------------------------------operator bool()
FileStream::operator bool()
{
  return m_aStream.good() && !m_aStream.eof();
}

//---------------------------------------------open
bool FileStream::open(unsigned int uFileOpenMode)
{
  
  std::ios::openmode aMode = static_cast<std::ios::openmode>(0);
  if(!(uFileOpenMode & WRITE_ONLY_ACCESS))
  {
    aMode |= std::ios::in;
  }
  if(uFileOpenMode & READ_WRITE_ACCESS ||
     uFileOpenMode & WRITE_ONLY_ACCESS)
  {
    aMode |= std::ios::out;
  }
  if(uFileOpenMode & OPEN_BINARY)
  {
    aMode |= std::ios::binary;
  }

  m_aStream.open(m_sFilename.c_str(), aMode);

  return m_aStream ? true : false;
}

//---------------------------------------------getLine
bool FileStream::getLine(tString& sDst)
{
  if(m_aStream)
  {
    std::getline(m_aStream, sDst);
    return true;
  }
  return false;
}

//---------------------------------------------append
bool FileStream::append(const tString& sSrc)
{
  if(m_aStream)
  {
    m_aStream << sSrc;
    return true;
  }
  return false;
}

//---------------------------------------------append
bool FileStream::append(std::istream& aSrc)
{
  if(m_aStream)
  {    
    while(aSrc.good())
    {
      std::string sLine;
      std::getline(aSrc, sLine);  
      m_aStream << sLine;
    }

    return true;
  }
  return false;
}

//---------------------------------------------getRawBuffer
bool FileStream::getRawBuffer(tString& sDst)
{
  if(m_aStream && m_aStream.good())
  {
    std::streampos uStartPos = m_aStream.tellg();
    m_aStream.seekg(0, std::ios::end);
    std::streampos uLength = m_aStream.tellg();
    m_aStream.seekg(uStartPos, std::ios::beg);

    uLength -= uStartPos;
    sDst.resize((size_t)uLength);

    m_aStream.read(&sDst[0], uLength);
    return true;
  }
  return false;
}

} }
