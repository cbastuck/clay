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

#ifndef HostModuleUICustomWidget_h_
#define HostModuleUICustomWidget_h_

#include <clay-core/base/ModuleDescriptor.h>

#include <deque>

#include "ui_QtViewerMainWindow.h"

namespace MF
{
  class Host;
  class ModuleWidget;
  class QtViewerWorkspace;

  class HostModuleUICustomWidget : public QMainWindow
  {
    Q_OBJECT
  public:
    HostModuleUICustomWidget(QWidget* pParent, Host* pHost);

    void init();
    void deInit();

    void autoArrangeModules();

  public slots:
    void onAddModule();
    void onSaveProject();
    void onLoadProject();
    void onCloseProject();
    void onPlayButton();

  protected:
    virtual void dragEnterEvent(QDragEnterEvent* pEvent);
    virtual void dragMoveEvent(QDragMoveEvent* pEvent);
    virtual void dropEvent(QDropEvent* pEvent);

    void onAddModule(ModuleDescriptorTraits::tModuleID    aModuleId,
                     const tString&                       sRuntimeModuleId,
                     ModuleDescriptorTraits::tNamespaceID aNamespaceId);

    void onLoadProject(const QString& sFilename);

    void addModuleUI(Module* pModule, unsigned int uX=100, unsigned int uY=100);

    QtViewerWorkspace* createProject(const QString& aProjectName);

  private:
    Host* m_pHost;

    Ui::qtViewerMainWindowUI m_aUi;

    typedef std::deque<ModuleWidget*> tModuleWidgetCollection;
    tModuleWidgetCollection m_collModules;

    QtViewerWorkspace* m_pWorkspace;
  };
}

#endif 
