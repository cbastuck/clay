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

#include "WAVReaderModuleUI.h"

#include <clay-modules/audio/WavReaderModule.h>

#include <QFileDialog>

NAMESPACE_CLAY_UI_BEGIN

//---------------------------------------------WAVReaderModuleUI
WAVReaderModuleUI::WAVReaderModuleUI(Module* pModule, Host* pHost, QWidget* pParent)
  : tBase(pModule, pHost, pParent)
{

}

//---------------------------------------------init
void WAVReaderModuleUI::init()
{
  tBase::init();
}

//---------------------------------------------deInit
void WAVReaderModuleUI::deInit()
{
  tBase::deInit();
}

//---------------------------------------------showModuleUI
void WAVReaderModuleUI::showModuleUI()
{
  QString sWavFn = QFileDialog::getOpenFileName(this, "Specify WAV Filename");
  if(!sWavFn.isEmpty())
  {
    MODULE::WavReaderModule* pModule = static_cast<MODULE::WavReaderModule*>(getModule());
    pModule->loadWAV(sWavFn.toAscii().constBegin());
  }
}

NAMESPACE_CLAY_UI_END
