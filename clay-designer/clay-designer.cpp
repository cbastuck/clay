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

#include <clay-core/base/ClayDefines.h>

#include <clay-modules/host/HostModule.h>
#include <clay-core/registry/ModuleRegistry.h>
#include <clay-core/helper/LexicalConversion.h>
#include <clay-gui-qt/base/QtModuleUIRegistry.h>
#include <clay-gui-qt/modules/HostModuleUI/HostModuleUI.h>

#include <qrc_gui.cxx> //include the resources

#include <QApplication>

#if CLAY_PLATFORM_UNIX
  int main(int argc, char** argv)
#elif CLAY_PLATFORM_WIN32
  #include <iostream>
  #include <tchar.h>
  int _tmain(int argc, _TCHAR* argv[])
#endif
{
  //_CrtSetDbgFlag(_CrtSetDbgFlag(0)|_CRTDBG_CHECK_ALWAYS_DF);

  CLAY::ClayXML::init();

  CLAY::MODEL::ClayTypeChecking::init();

  int uPseudoArgc = 0;
  char** ppPseudeArgv = NULL;
  QApplication app(uPseudoArgc, ppPseudeArgv);

  CLAY::ModuleRegistry aFactory;
  aFactory.registerModule<CLAY::MODULE::HostModule>();
  
  CLAY::UI::QtModuleUIRegistry::instance()->registerModuleUI<CLAY::MODULE::HostModule, CLAY::UI::HostModuleUI>();

  CLAY::MODULE::HostModule* pHost = new CLAY::MODULE::HostModule("ClayExeHostModule", &aFactory);
  pHost->init(NULL);

  CLAY::tString sFn;
  if(argc == 2)
  {
    #if CLAY_PLATFORM_WIN32
      sFn = CLAY::HELPER::LexicalConversion::wstringToUtf8(argv[1]);
    #elif CLAY_PLATFORM_UNIX
      sFn = argv[1];
    #endif
  }

  CLAY::UI::HostModuleUI* pHostUI = new CLAY::UI::HostModuleUI(pHost, pHost, NULL);
  pHostUI->init();
  pHostUI->showModuleUI();

  if(!sFn.empty())
  {
    pHostUI->loadProject(sFn);
  }

  app.exec();

  pHostUI->deInit();
  pHost->deInit();

  delete pHostUI;
  delete pHost;

  CLAY::ClayXML::deInit();

  CLAY::MODEL::ClayTypeChecking::deInit();

  return 0;
}
