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

#ifndef MP3READER_MODULE_UI_H_
#define MP3READER_MODULE_UI_H_

#include <clay-gui-qt/base/ModuleWidgetHelper.h>
#include <clay-modules/mp3/MP3ReaderModule.h>

namespace CLAY{ namespace UI{

class LoaderThread;

class MP3ReaderModuleUI : public ModuleWidgetHelper<CLAY::MODULE::MP3ReaderModule, MP3ReaderModuleUI>
{
private:
  typedef ModuleWidgetHelper<CLAY::MODULE::MP3ReaderModule, MP3ReaderModuleUI> tBase;

public:
  MP3ReaderModuleUI(Module* pModule, Host* pHost, QWidget* pParent);
  virtual ~MP3ReaderModuleUI();

  virtual void init();
  virtual void deInit();
  virtual void showModuleUI();
 
protected:
  void resetLoaderThread();

private:
  LoaderThread* m_pLoaderThread;
};

} }

#endif
