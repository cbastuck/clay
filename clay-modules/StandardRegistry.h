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

#ifndef STANDARD_REGISTRY_H_
#define STANDARD_REGISTRY_H_

#include <clay-core/registry/ModuleRegistry.h>

namespace CLAY{ namespace MODULE{

class StandardRegistry : public ModuleRegistry
{
public:
  static StandardRegistry* instance();

private:
  StandardRegistry();
      
  void setupRegistry();
};

} }

#endif
