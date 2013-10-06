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

#include "XMLRPCServer.h"

//xmlrpc
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/registry.hpp>
#include <xmlrpc-c/server_abyss.hpp>

#if CLAY_PLATFORM_WIN32
#  include <windows.h>
#elif CLAY_PLATFORM_UNIX
#  include <unistd.h>
#endif

class sampleAddMethod: public xmlrpc_c::method
{
public:
  //---------------------------------------------
  sampleAddMethod()
  {
    // signature and help strings are documentation -- the client
    // can query this information with a system.methodSignature and
    // system.methodHelp RPC.
    this->_signature = "i:ii";
    // method's result and two arguments are integers
    this->_help = "This method adds two integers together";
  }

  //---------------------------------------------
  void execute(xmlrpc_c::paramList const& paramList, xmlrpc_c::value * const retvalP)
  {

    int const addend(paramList.getInt(0));
    int const adder(paramList.getInt(1));

    paramList.verifyEnd(2);

    *retvalP = xmlrpc_c::value_int(addend + adder);

    // Sometimes, make it look hard (so client can see what it's like
    // to do an RPC that takes a while).
    if(adder == 1)
    {
      sleep(2);
    }
  }
};

namespace CLAY{ namespace MODULE{

//---------------------------------------------
const char* XMLRPCServer::staticModuleURI()
{
    return "http://claymodules.org/rpc/xmlrpcserver";
}

//---------------------------------------------
XMLRPCServer::XMLRPCServer(const tString& sId) :
    Module(sId)
{
  runServer();
}

//---------------------------------------------
const char* XMLRPCServer::getModuleURI() const
{
    return staticModuleURI();
}

//---------------------------------------------
void XMLRPCServer::runServer()
{
  try
  {
    xmlrpc_c::registry myRegistry;

    xmlrpc_c::methodPtr const sampleAddMethodP(new sampleAddMethod);

    myRegistry.addMethod("sample.add", sampleAddMethodP);

    xmlrpc_c::serverAbyss myAbyssServer(
        xmlrpc_c::serverAbyss::constrOpt().registryP(&myRegistry).portNumber(
            8080));

    myAbyssServer.run();
    // xmlrpc_c::serverAbyss.run() never returns
    CLAY_ASSERT(false);
  }
  catch(std::exception const& e)
  {
    std::cerr << "Something failed.  " << e.what() << std::endl;
  }
}

} }
