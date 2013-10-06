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

#include "XMLRPCClient.h"

#include <clay-core/xml/XMLDocument.h>

//xmlrpc
#include <xmlrpc-c/girerr.hpp>
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/client_simple.hpp>

//boost
#include <boost/algorithm/string.hpp>

//debugging
#include <fstream>

namespace CLAY { namespace MODULE {

//---------------------------------------------
const char* XMLRPCClient::staticModuleURI()
{
    return "http://claymodules.org/rpc/xmlrpclient";
}

//---------------------------------------------
XMLRPCClient::XMLRPCClient(const tString& sId)
  : Module(sId)
{

}

//---------------------------------------------
const char* XMLRPCClient::getModuleURI() const
{
    return staticModuleURI();
}

//---------------------------------------------
bool XMLRPCClient::init(XERCES::DOMNode* pNode)
{
  if(!Module::init(pNode))
  {
      return false;
  }

  registerModuleOutputs();
  registerModuleInputs();

  return true;
}

//---------------------------------------------
void XMLRPCClient::deInit()
{
    Module::deInit();
}

//---------------------------------------------
void XMLRPCClient::registerModuleOutputs()
{
  registerOutput(new XMLDocumentOutput, "xml");
}

//---------------------------------------------
void XMLRPCClient::registerModuleInputs()
{

}

//---------------------------------------------
Module::ProcessResultCode XMLRPCClient::process()
{
  XMLDocumentOutput* pOut = static_cast<XMLDocumentOutput* >(getOutput("xml"));
  if(!pOut)
  {
    return Module::Process_ERROR;
  }

  try
  {
    std::string const serverUrl("http://ws.audioscrobbler.com/2.0/");
    std::string const methodName("user.getLovedTracks");

    xmlrpc_c::clientSimple myClient;
    xmlrpc_c::value result;

    xmlrpc_c::paramList params;

    std::map<std::string, xmlrpc_c::value> aMap;
    aMap["user"] = xmlrpc_c::value_string("chris1379");
    aMap["api_key"] = xmlrpc_c::value_string("436c72298a38ed5b982ac0db43b5680f");

    xmlrpc_c::value_struct aStruct(aMap);

    params.add(aStruct);

    myClient.call(serverUrl, methodName, params, &result);

    std::string sResultXML = xmlrpc_c::value_string(result);
    //CLAY_LOG_INFO(sResultXML.c_str());

    //the quotes in the result string are escaped for whatever reason, remove!
    boost::erase_all(sResultXML, "\\");

    pOut->getData().readFromMemory(sResultXML.c_str(), sResultXML.size());
  }
  catch(std::exception const& e)
  {
    std::cerr << "Client threw error: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cerr << "Client threw unexpected error." << std::endl;
  }

  return Module::Process_OK;
}

} }
