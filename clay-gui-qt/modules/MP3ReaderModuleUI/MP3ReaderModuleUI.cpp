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

#include "MP3ReaderModuleUI.h"

#include <clay-modules/mp3/MP3ReaderModule.h>

#include <QThread>
#include <QFileDialog>

namespace CLAY{ namespace UI{

class LoaderThread : public QThread
{
public:
  LoaderThread(MODULE::MP3ReaderModule* pModule, const tString& sFn) 
    : m_sFn(sFn), m_pModule(pModule)
  {
  }

protected:
  virtual void run()
  {
    m_pModule->loadMP3(m_sFn);
  }

private:
  MODULE::MP3ReaderModule* m_pModule;
  tString                  m_sFn;
};

//---------------------------------------------MP3ReaderModuleUI
MP3ReaderModuleUI::MP3ReaderModuleUI(Module* pModule, Host* pHost, QWidget* pParent)
  : tBase(pModule, pHost, pParent),
    m_pLoaderThread(NULL)
{

}

//---------------------------------------------~MP3ReaderModuleUI
MP3ReaderModuleUI::~MP3ReaderModuleUI()
{
  CLAY_ASSERT(!m_pLoaderThread);
}

//---------------------------------------------init
void MP3ReaderModuleUI::init()
{
  tBase::init();
}

//---------------------------------------------deInit
void MP3ReaderModuleUI::deInit()
{
  resetLoaderThread();
  tBase::deInit();
}

//---------------------------------------------showModuleUI
void MP3ReaderModuleUI::showModuleUI()
{
  QString sWavFn = QFileDialog::getOpenFileName(this, "Specify MP3 Filename", "", "*.mp3");
  if(!sWavFn.isEmpty())
  {
    MODULE::MP3ReaderModule* pModule = static_cast<MODULE::MP3ReaderModule*>(getModule());
    pModule->reset();
    
    resetLoaderThread();
    m_pLoaderThread = new LoaderThread(pModule, sWavFn.toAscii().constBegin());
    m_pLoaderThread->start();
  }
}

//---------------------------------------------resetLoaderThread
void MP3ReaderModuleUI::resetLoaderThread()
{
  if(m_pLoaderThread)
  {
    if(m_pLoaderThread->isRunning())
    {
      m_pLoaderThread->terminate();
    }

    delete m_pLoaderThread;
    m_pLoaderThread = NULL;
  }
}

} }
