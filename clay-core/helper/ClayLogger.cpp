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

#include "ClayLogger.h"

#include <iostream>

namespace CLAY{

//---------------------------------------------
ClayLogger& ClayLogger::getInstance()
{
  static ClayLogger s_aInstance;
  return s_aInstance;
}

//---------------------------------------------
void ClayLogger::printError(const char* pError)
{
  //TODO: on unix add red color to console output
  std::cout << "ERROR: " << pError << std::endl;
}

//---------------------------------------------
void ClayLogger::printInfo(const char* pInfo)
{
  std::cout << pInfo <<std::endl;
}

//---------------------------------------------
ClayLogger::ClayLogger()
{

}

//---------------------------------------------
ClayLogger::~ClayLogger()
{

}

}

