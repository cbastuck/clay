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

#ifndef MODULEHELPER_H_
#define MODULEHELPER_H_

#include <clay-core/base/Module.h>

namespace CLAY{

class ModuleHelper : public Module
{
public:
  ModuleHelper(const tString& sId);
  virtual bool init(XERCES::DOMNode* pNode);

  virtual void registerModuleOutputs();
  virtual void registerModuleInputs();
  
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------
ModuleHelper::ModuleHelper(const tString& sId)
  : Module(sId)
{
}

//---------------------------------------------init
inline bool ModuleHelper::init(XERCES::DOMNode* pNode)
{
  bool bRes = Module::init(pNode);
  registerModuleOutputs();
  registerModuleInputs();
  return bRes;
}


//---------------------------------------------registerModuleOutputs
inline void ModuleHelper::registerModuleOutputs()
{
  //override
}

//---------------------------------------------registerModuleInputs
inline void ModuleHelper::registerModuleInputs()
{
  //override
}

}

#endif
