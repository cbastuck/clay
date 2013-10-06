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

#include "AdderModule.h"

namespace CLAY{ namespace MODULE{

//---------------------------------------------
const char* AdderModule::staticModuleURI()
{
  return "http://claymodules.org/math/adder";
}

//---------------------------------------------
AdderModule::AdderModule(const tString& sId)
  : Module(sId)
{
}

//---------------------------------------------
AdderModule::~AdderModule()
{
}

//---------------------------------------------
const char* AdderModule::getModuleURI() const
{
    return staticModuleURI();
}

//---------------------------------------------
bool AdderModule::init(XERCES::DOMNode* pNode)
{
    if(!Module::init(pNode))
    {
        return false;
    }

    registerModuleInputs();
    registerModuleOutputs();

    return true;
}

//---------------------------------------------
void AdderModule::registerModuleInputs()
{
  registerInput(new ModuleInput<unsigned int>(), "in1");
  registerInput(new ModuleInput<unsigned int>(), "in2");
}

//---------------------------------------------
void AdderModule::registerModuleOutputs()
{
  registerOutput(new ModuleOutput<unsigned int>(), "out");
}

//---------------------------------------------
bool AdderModule::save(XERCES::DOMElement* pNode)
{
  return save(pNode);
}

//---------------------------------------------
bool AdderModule::load(XERCES::DOMElement* pNode, Module::tConnectionMap* pInputConnections, Module::tConnectionMap* pOutputConnections)
{
  return load(pNode, pInputConnections, pOutputConnections);
}

//---------------------------------------------
Module::ProcessResultCode AdderModule::process()
{
  ModuleInput<unsigned int>* pIn1 = static_cast<ModuleInput<unsigned int>*>(getInput("in1"));
  ModuleInput<unsigned int>* pIn2 = static_cast<ModuleInput<unsigned int>*>(getInput("in2"));

  unsigned int uIn1 = (pIn1->isConnected()) ? *pIn1->getData() : 0;
  unsigned int uIn2 = (pIn2->isConnected()) ? *pIn2->getData() : 0;

  ModuleOutput<unsigned int>* pOut = static_cast<ModuleOutput<unsigned int>*>(getOutput("out"));
  pOut->setData(uIn1 + uIn2);

  return Module::Process_OK;
}

} }
