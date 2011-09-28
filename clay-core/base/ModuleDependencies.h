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

#ifndef MODULEDEPENDENCIES_H_
#define MODULEDEPENDENCIES_H_

#include "ClayDefines.h"

//STL
#include <map>
#include <set>

namespace CLAY{

class Module;

class ModuleDependencies
{
public:
  typedef std::set<Module*> tModuleDep;

private:
  typedef std::map<Module*, tModuleDep> tDepCollection;

public:
  ModuleDependencies();

  void addModule(Module* pModule);
  void addDependency(Module* pModule, Module* pDependency);
  void removeDependencies(Module* pModule, bool bOnlyOutgoingDependencies=false);

  const tModuleDep* getDependencies(Module* pModule) const;
  bool isUnknown(Module* pModule) const;
  tDepCollection::size_type getNumDependencies(Module* pModule) const;

  tModuleDep::const_iterator beginDep(Module* pModule) const;
  tModuleDep::const_iterator endDep(Module* pModule) const;

  void computeTransitiveHull(Module* pModule=NULL, tModuleDep* pDst=NULL, std::set<Module*>* pVisitedModules=NULL);

  bool hasRecursiveModule() const;
  Module* getRecursiveModule() const;

  Module* getMostIndependentModule();
  
  void debugPrint();

private:
  tDepCollection m_collDependencies;
};

}

#endif
