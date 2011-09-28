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

#include "QtViewer.h"
#include "QtViewerMainWindow.h"

#include <QApplication>
#include <QMainWindow>

namespace CLAY
{
  namespace UI
  {

    //---------------------------------------------
    bool QtViewer::init(XERCES::DOMNode* pNode)
    {
      return true;
    }

    //---------------------------------------------
    void QtViewer::deInit()
    {

    }

    //---------------------------------------------
    Module::ProcessResultCode QtViewer::process()
    {
      int argc = 0;
      char** argv = NULL;
      QApplication app(argc, argv);
      QtViewerMainWindow aMainWindow;
      aMainWindow.init();
      aMainWindow.show();    
      app.exec();
      aMainWindow.deInit();
      return Module::Process_OK;
    }
  }
}