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

#ifndef XMLRPCXML_RPC_CLIENT_H_
#define XMLRPCXML_RPC_CLIENT_H_

#include <clay-core/base/ModuleHelper.h>

namespace CLAY{ namespace MODULE{

class XMLRPCClient : public ModuleHelper<XMLRPCClient,
                                         HELPER::IntegerEncoding<'X','R','P','C'>::value,
                                         HELPER::IntegerEncoding<'C','O','M','M'>::value>
{
private:
  typedef ModuleHelper<XMLRPCClient,
                       HELPER::IntegerEncoding<'X','R','P','C'>::value,
                       HELPER::IntegerEncoding<'C','O','M','M'>::value> tBase;

public:
  XMLRPCClient(const tString& sId);

  virtual bool init(XERCES::DOMNode* pNode);
  virtual void deInit();

  virtual void registerModuleOutputs();
  virtual void registerModuleInputs();

  virtual ProcessResultCode process();

private:

};

} }

#endif
