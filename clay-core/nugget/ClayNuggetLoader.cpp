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

#include "ClayNuggetLoader.h"

#include <clay-core/helper/SharedLibrary.h>

NAMESPACE_CLAY_NUGGET_BEGIN

//---------------------------------------------ClayNuggetLoader
ClayNuggetLoader::ClayNuggetLoader()
{

}

//---------------------------------------------~ClayNuggetLoader
ClayNuggetLoader::~ClayNuggetLoader()
{

}

//---------------------------------------------init
void ClayNuggetLoader::init()
{
  CLAY_ASSERT(m_collLoadedNuggets.size() == 0);
}

//---------------------------------------------deInit
void ClayNuggetLoader::deInit()
{
  tNuggetLibsCollection::iterator it  = m_collLoadedNuggets.begin();
  tNuggetLibsCollection::iterator end = m_collLoadedNuggets.end();
  for(; it!=end; ++it)
  {
    tCollectionEntry& aCurrent = *it;
    HELPER::SharedLibrary* pCurrentLib    = aCurrent.first;
    NUGGET::IClayNugget*   pCurrentNugget = aCurrent.second;
    pCurrentNugget->deInit();
    pCurrentLib->deInit();
    delete pCurrentLib;
  }
  m_collLoadedNuggets.clear();
}

//---------------------------------------------loadNugget
IClayNugget* ClayNuggetLoader::loadNugget(const tString& sNuggetFn)
{
  bool bSuccess = true;
  HELPER::SharedLibrary* pShared = new HELPER::SharedLibrary;
  if(pShared->loadLibrary(sNuggetFn))
  {
    NUGGET::IClayNugget* pNugget = NULL;
    if(pShared->invoke("getNugget", pNugget))
    {
      CLAY_ASSERT(pNugget);
      m_collLoadedNuggets.push_back(std::make_pair(pShared, pNugget));
      return pNugget;
    }
    else
    {
      bSuccess = false;
    }

    if(!bSuccess)
    {
      delete pShared; //an error occurred during function call
    }
  }
  return NULL;
}

//---------------------------------------------getNumLoadedNuggets
unsigned int ClayNuggetLoader::getNumLoadedNuggets() const
{
  return m_collLoadedNuggets.size();
}

//---------------------------------------------getNuggetFilename
const tString& ClayNuggetLoader::getNuggetFilename(unsigned int uIdx) const
{
  CLAY_ASSERT(getNumLoadedNuggets() > uIdx);
  return m_collLoadedNuggets[uIdx].first->getFilename();
}

NAMESPACE_CLAY_NUGGET_END
