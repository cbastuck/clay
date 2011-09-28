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

#ifndef XMLBUFFER_H_
#define XMLBUFFER_H_

#include <clay-core/xml/ClayXML.h>

namespace CLAY {

class XMLBuffer
{
public:
  XMLBuffer();
  ~XMLBuffer();

  unsigned int getSize() const;
  const void* getData() const;

  void print(std::ostream& os);

private:
  friend class XMLDocument;

  XMLBuffer& operator=(XERCES::MemBufFormatTarget* pBuffer);
  char*        m_pBuffer;
  unsigned int m_uBufferSize;
};

}

#endif /* XMLBUFFER_H_ */
