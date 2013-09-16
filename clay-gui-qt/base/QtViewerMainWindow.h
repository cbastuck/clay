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

#ifndef QTVIEWERMAINWINDOW_H_
#define QTVIEWERMAINWINDOW_H_

#include <clay-core/host/Host.h>

#include <deque>

#include "ui_QtViewerMainWindow.h"

namespace CLAY
{
  namespace UI
  {
    class ModuleWidget;
    class QtViewerWorkspace;

    class QtViewerMainWindow : public QMainWindow, public Host
    {
      Q_OBJECT
    public:
      typedef Host tBase;

      QtViewerMainWindow();

      virtual bool init();

      virtual void deInit();

      virtual bool connect(Module* pFrom, Module* pTo);

      virtual bool saveProject(XERCES::DOMElement* pNode);

      virtual bool loadProject(const tString& sFilename);

      virtual Module* loadModule(XERCES::DOMElement* pNode, Module::tConnectionMap* pInputConnections, Module::tConnectionMap* pOutputConnections);  

      virtual void closeProject();

      void onAddModule(const char* moduleURI, const tString& sRuntimeModuleId);

      void autoArrangeModules();

    public slots:
        void onAddModule();
        void onSaveProject();
        void onLoadProject();
        void onRecentProjects();
        void onPlayButton();
        void onCloseProject();
        void onLoadRecentProject1();
        void onLoadRecentProject2();
        void onLoadRecentProject3();
        void onLoadRecentProject4();
        void onLoadRecentProject5();

    protected:
      virtual bool restoreInputConnections (const Module::tConnectionMap& collInputConnections);
      virtual bool restoreOutputConnections(const Module::tConnectionMap& collOutputConnections);

      virtual void dragEnterEvent(QDragEnterEvent* pEvent);
      virtual void dragMoveEvent(QDragMoveEvent* pEvent);
      virtual void dropEvent(QDropEvent* pEvent);

      UI::ModuleWidget* getModuleWidget(const Module* pModule) const;

      QtViewerWorkspace* createProject(const QString& aProjectName);

      void onLoadProject(const QString& sFilename);

      void addModuleUI(Module* pModule, unsigned int uX=100, unsigned int uY=100);

      void restoreRecentProjects();

      QString getRecentProject(unsigned int i);

      void addRecentProject(const QString& sProject);

      unsigned int getNumRecentProjects();

    private:
      Ui::qtViewerMainWindowUI m_aUi;

      typedef std::deque<UI::ModuleWidget*> tModuleWidgetCollection;
      tModuleWidgetCollection m_collModules;

      QtViewerWorkspace* m_pWorkspace;

      typedef std::deque<QString> tRecentProjectsStack;
      tRecentProjectsStack m_collRecentProjectsStack;
    };
  }
}

#endif 
