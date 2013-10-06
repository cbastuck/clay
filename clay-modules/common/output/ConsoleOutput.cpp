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

#include "ConsoleOutput.h"

namespace CLAY{ namespace MODULE{

//---------------------------------------------
const char* ConsoleOutput::staticModuleURI()
{
  return "http://claymodules.org/debug/cout";
}

//---------------------------------------------
ConsoleOutput::ConsoleOutput(const tString& sId)
  : Module(sId)
{
}

//---------------------------------------------
ConsoleOutput::~ConsoleOutput()
{
}

//---------------------------------------------
const char* ConsoleOutput::getModuleURI() const
{
    return staticModuleURI();
}

//---------------------------------------------
bool ConsoleOutput::init(XERCES::DOMNode *pNode)
{
    if(!Module::init(pNode))
    {
        return false;
    }

    registerModuleInputs();

    return true;
}

//---------------------------------------------
void ConsoleOutput::registerModuleInputs()
{
  registerInput(new ModuleInput<unsigned int>(), "in");
}

//---------------------------------------------
Module::ProcessResultCode ConsoleOutput::process()
{
  ModuleInput<unsigned int>* pIn = static_cast<ModuleInput<unsigned int>*>(getInput("in"));
  if(pIn->isConnected())
  {
    std::cout << *pIn->getData() << std::endl;
  }
  return Module::Process_OK;
}

} }
