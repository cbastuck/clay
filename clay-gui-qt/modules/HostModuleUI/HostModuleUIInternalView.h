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

#ifndef HOST_MODULE_UI_INTERNAL_VIEW_H_
#define HOST_MODULE_UI_INTERNAL_VIEW_H_

//clay-core
#include <clay-core/base/Module.h>
#include <clay-core/event/SignalReceiver.h>

//clay-ui
#include <clay-gui-qt/base/QtViewerWorkspace.h>

#include "ui_HostModuleUIInternalViewQt.h"

//QT
#include <QWidget>

namespace CLAY{ namespace NUGGET{ 
class IClayNugget;
class ClayNuggetLoader;
} }

namespace CLAY{ namespace MODULE { class HostModule; } }

namespace CLAY{ namespace UI{

class HostModule;
class HostModuleUI;
class ModuleWidget;

class HostModuleUIInternalView : public QWidget,
                                 public SignalReceiver
{
  Q_OBJECT

private:
  typedef std::deque<ModuleWidget*> tModuleWidgetCollection;

public:
  typedef tModuleWidgetCollection::iterator       tWidgetIterator;
  typedef tModuleWidgetCollection::const_iterator tConstWidgetIterator;
  typedef QtViewerWorkspace::tIterator            tConnectionIterator;
  typedef QtViewerWorkspace::tConstIterator       tConstConnectionIterator;

  HostModuleUIInternalView(HostModuleUI* pParent, MODULE::HostModule* pHostModule);
  virtual ~HostModuleUIInternalView();

  void init();
  void deInit();

  void setupUi(QWidget* pParent);

public slots:
  void autoArrangeModules();
  void onNewProject();
  void onAddModule();
  void onSaveProject();
  void onLoadProject();
  void onCloseProject();
  void onPlayButton();
  void onSettingsButton();

  void dragEnterEvent(QDragEnterEvent* pEvent);
  void dragMoveEvent(QDragMoveEvent* pEvent);
  void dropEvent(QDropEvent* pEvent);

  Module* onAddModule(const char* moduleURI,
                      const tString&                       sRuntimeModuleId,
                      XERCES::DOMNode*                     pConfigNode=NULL);

  void onRemoveModule(UI::ModuleWidget* pWidget);
  void onRemoveModuleConnections(UI::ModuleWidget* pWidget);

  void onLoadProject(const QString& sFilename);

  UI::ModuleWidget* addModuleUI(Module* pModule, unsigned int uX=100, unsigned int uY=100);
  void removeModuleUI(UI::ModuleWidget* pWidget);

  QtViewerWorkspace* createProject(const QString& aProjectName);

  void onConnectionEstablished(Module* pFrom, Module* pTo);
  void onClayNuggetLoaded(NUGGET::IClayNugget* pNugget);

  unsigned int getNumWidgets() const;
  tConstWidgetIterator beginWidgets() const;
  tConstWidgetIterator endWidgets() const;

  unsigned int getNumConnections() const;
  tConstConnectionIterator beginConnections() const;
  tConstConnectionIterator endConnections() const;
  void addWidgetConnection(ModuleWidget* pFrom, ModuleWidget* pTo); 

  ModuleWidget*       getModuleWidget(const tString& sModuleRuntimeID);
  const ModuleWidget* getModuleWidget(const tString& sModuleRuntimeID) const;

  void onLoadFinished();

public slots:
  void onWidgetSelected(UI::ModuleWidget* pWidget);
  void onRequestContextMenu(UI::ModuleWidget* pWidget, QMouseEvent* pEvent);

protected:
  UI::ModuleWidget* getModuleWidget(Module* pModule);
  void createModuleWidgets();
  void createConnectionWires();
  void activateProjectDependents(bool bEnable);

private:
  HostModuleUI*                  m_pParent;
  Ui::HostModuleUIInternalViewQt m_aUi;
  MODULE::HostModule*            m_pHostModule;
  QtViewerWorkspace*             m_pWorkspace;
  tModuleWidgetCollection        m_collModules;

  QAction*                       m_pRemoveModuleAction;
  QAction*                       m_pRemoveModuleConnectionsAction;
  QAction*                       m_pShowCustomWidgetAction;
  QAction*                       m_pResetModuleAction;
  QAction*                       m_pRunModuleAction;
  QAction*                       m_pMarkAsPriority;
};

} }

#endif
