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

#include <clay-modules/rpc/xmlrpc-c/XMLRPCClient.h>
#include <clay-modules/rpc/xmlrpc-c/XMLRPCServer.h>

#include <clay-modules/xml/XMLProcessor.h>
#include <clay-gui-qt/modules/xml/XMLProcessorUI.h>

#include <clay-core/nugget/IClayNugget.h>

#define CLAY_MODULE_BUNDLE_NUGGET boost::mpl::list<CLAY::NUGGET::NuggetItem<CLAY::MODULE::XMLRPCClient>, \
                                                   CLAY::NUGGET::NuggetItem<CLAY::MODULE::XMLRPCServer>, \
                                                   CLAY::NUGGET::NuggetItem<CLAY::MODULE::XMLProcessor, CLAY::UI::XMLProcessorUI> >

#include <clay-core/nugget/ClayNugget.h>
