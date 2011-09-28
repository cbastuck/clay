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

#ifndef CLAY_NUGGET_LOADER_H_
#define CLAY_NUGGET_LOADER_H_

#include <clay-core/base/ClayDefines.h>

#include "IClayNugget.h"

#include <deque>

NAMESPACE_CLAY_HELPER_BEGIN
class SharedLibrary;
NAMESPACE_CLAY_HELPER_END

NAMESPACE_CLAY_NUGGET_BEGIN

class ClayNuggetLoader
{
public:
  ClayNuggetLoader();
  
  ~ClayNuggetLoader();

  void init();

  void deInit();

  IClayNugget* loadNugget(const tString& sNuggetFn);

  unsigned int getNumLoadedNuggets() const;

  const tString& getNuggetFilename(unsigned int uIdx) const;

private:
  typedef std::pair<HELPER::SharedLibrary*, NUGGET::IClayNugget*> tCollectionEntry;
  typedef std::deque<tCollectionEntry> tNuggetLibsCollection;
  tNuggetLibsCollection m_collLoadedNuggets;
};

NAMESPACE_CLAY_NUGGET_END

#endif
