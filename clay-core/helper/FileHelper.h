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

#ifndef FileHelper_H_
#define FileHelper_H_

#include <clay-core/base/ClayDefines.h>

#include <fstream>

namespace CLAY{ namespace HELPER{

class FileStream
{
public:
  FileStream(const tString& sFilename);

  ~FileStream();

  operator bool();

  enum FileOpenMode
  {
    READ_WRITE_ACCESS = 1<<0,
    READ_ONLY_ACCESS  = 1<<1,
    WRITE_ONLY_ACCESS = 1<<2,
    OPEN_BINARY       = 1<<3,
  };
  bool open(unsigned int uFileOpenMode = READ_ONLY_ACCESS);

  bool getLine(tString& sDst);
  bool append(const tString& sSrc);
  bool append(std::istream& aSrc);

  bool getRawBuffer(tString& sDst);

private:
  tString      m_sFilename;
  std::fstream m_aStream;
};
    
template<class T>
inline T readBigEndian(std::istream& is)
{
    const unsigned int n=sizeof(T);
    unsigned char buf[n]; //=new unsigned char[n];
    is.read((char*)buf,n);
    T v=readBigEndian<T>(buf);
    //delete[] buf;
    return v;
}

template<class T>
inline T readLittleEndian(std::istream& is)
{
    const unsigned int n=sizeof(T);
    unsigned char buf[n]; //=new unsigned char[n];
    is.read((char*)buf,n);
    T v=readLittleEndian<T>(buf);
    //delete[] buf;
    return v;
}

} }

#endif 
