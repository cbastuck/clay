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

#ifndef CLAYSHAPER_H_
#define CLAYSHAPER_H_

#include "Module.h"

#include <deque>

namespace CLAY{

class ClayExecutable;

class ClayShaper
{
public:
  typedef std::deque<Module*> tModuleCollection;

  CLAY_DLL_EXPORT ClayShaper();
  CLAY_DLL_EXPORT ClayShaper(const tModuleCollection* pPriorityList);
  CLAY_DLL_EXPORT ~ClayShaper();

  CLAY_DLL_EXPORT bool addExternalModule(Module* pModule);
  CLAY_DLL_EXPORT void clearExternals(); 

  CLAY_DLL_EXPORT Module::CompileResultCode shapeProcess(Module* pModule, ClayExecutable& aTarget);
  CLAY_DLL_EXPORT Module::CompileResultCode shapeProcess(const tModuleCollection& collModules, ClayExecutable& aTarget);
  
protected:
  CLAY_DLL_EXPORT Module::CompileResultCode shapeProcess(ModuleDependencies& aDirectDependencies, ModuleDependencies& aTransitiveDependencies, ClayExecutable& aTarget);

  CLAY_DLL_EXPORT Module::CompileResultCode shapeExecutable(ModuleDependencies& aDeps, ClayExecutable& aTarget);
  CLAY_DLL_EXPORT Module::CompileResultCode shapeRecursiveExecutable(ModuleDependencies& aDirectDependencies, 
                                                                     const ModuleDependencies& aTransitiveDependencies, 
                                                                     ClayExecutable& aTarget);

  CLAY_DLL_EXPORT Module* getPriorityModule(Module* pModule, const ModuleDependencies& aDeps);

  CLAY_DLL_EXPORT bool checkExternalDependencies(Module* pModule, const ModuleDependencies& aDeps);

  CLAY_DLL_EXPORT void collectDependenciesHelper(Module* pModule, ModuleDependencies& aDst);

private:
  const tModuleCollection* m_pPriorityList;
  tModuleCollection        m_aExternalModules;
};

}

#endif
