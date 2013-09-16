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

#include "HostModuleUIInternalView.h"

//clay-core
#include <clay-core/nugget/ClayNuggetLoader.h>
#include <clay-core/registry/ModuleRegistry.h>
#include <clay-core/helper/LexicalConversion.h>

//clay-gui
#include <clay-gui-qt/base/QtIOWidget.h>
#include <clay-gui-qt/base/QtListDialog.h>
#include <clay-gui-qt/base/QtModuleUIRegistry.h>
#include <clay-gui-qt/modules/HostModuleUI/HostModuleUI.h>

//clay-modules
#include <clay-modules/host/HostModule.h>

//qt
#include <QMenu>
#include <QFileDialog>
#include <QMessageBox>
#include <QDataStream>
#include <QFileDialog>
#include <QScrollArea>
#include <QDragEnterEvent>

//boost
#include <boost/bind.hpp>

namespace CLAY{ namespace UI{

//---------------------------------------------HostModuleUIInternalView
HostModuleUIInternalView::HostModuleUIInternalView(HostModuleUI* pParent, MODULE::HostModule* pHostModule)
  : QWidget(NULL),
    m_pParent(pParent),
    m_pHostModule(pHostModule),
    m_pWorkspace(NULL),
    m_pRemoveModuleAction(NULL),
    m_pRemoveModuleConnectionsAction(NULL),
    m_pShowCustomWidgetAction(NULL),
    m_pResetModuleAction(NULL),
    m_pRunModuleAction(NULL),
    m_pMarkAsPriority(NULL)
{

}

//---------------------------------------------~HostModuleUIInternalView
HostModuleUIInternalView::~HostModuleUIInternalView()
{

}

//---------------------------------------------init
void HostModuleUIInternalView::init()
{
  m_aUi.setupUi(this);

  activateProjectDependents(false);

  QObject::connect(m_aUi.m_pNewProjectButton,SIGNAL(pressed()), this, SLOT(onNewProject()));
  QObject::connect(m_aUi.m_pAddModuleButton, SIGNAL(pressed()), this, SLOT(onAddModule()));
  QObject::connect(m_aUi.m_pSaveButton,      SIGNAL(pressed()), this, SLOT(onSaveProject()));
  QObject::connect(m_aUi.m_pLoadButton,      SIGNAL(pressed()), this, SLOT(onLoadProject()));
  QObject::connect(m_aUi.m_pCloseButton,     SIGNAL(pressed()), this, SLOT(onCloseProject()));
  QObject::connect(m_aUi.m_pPlayButton,      SIGNAL(pressed()), this, SLOT(onPlayButton()));
  QObject::connect(m_aUi.m_pSettingsButton,  SIGNAL(pressed()), this, SLOT(onSettingsButton()));

  setAcceptDrops(true);

  m_aUi.m_pTabWidget->clear();
  m_pWorkspace = createProject("<unnamed>"); //create initial project

  addConnection(&m_pHostModule->signalConnectionEstablished, &HostModuleUIInternalView::onConnectionEstablished);
  addConnection(&m_pHostModule->signalClayNuggetLoaded, &HostModuleUIInternalView::onClayNuggetLoaded);

  //create actions
  CLAY_ASSERT(!m_pRemoveModuleAction);
  m_pRemoveModuleAction = new QAction("Remove Module", this);

  CLAY_ASSERT(!m_pRemoveModuleConnectionsAction);
  m_pRemoveModuleConnectionsAction = new QAction("Remove all connections", this);

  CLAY_ASSERT(!m_pMarkAsPriority);
  m_pMarkAsPriority = new QAction("Mark as priority module", this);

  CLAY_ASSERT(!m_pShowCustomWidgetAction);
  m_pShowCustomWidgetAction = new QAction("Show Custom Widget", this);

  CLAY_ASSERT(!m_pResetModuleAction);
  m_pResetModuleAction = new QAction("Reset Module", this);

  CLAY_ASSERT(!m_pRunModuleAction);
  m_pRunModuleAction = new QAction("Process Module", this);
}

//---------------------------------------------deInit
void HostModuleUIInternalView::deInit()
{
  onCloseProject();

  if(m_pRemoveModuleAction)
  {
    delete m_pRemoveModuleAction;
    m_pRemoveModuleAction = NULL;
  }

  if(m_pRemoveModuleConnectionsAction)
  {
    delete m_pRemoveModuleConnectionsAction;
    m_pRemoveModuleConnectionsAction = NULL;
  }

  if(m_pShowCustomWidgetAction)
  {
    delete m_pShowCustomWidgetAction;
    m_pShowCustomWidgetAction = NULL;
  }

  if(m_pResetModuleAction)
  {
    delete m_pResetModuleAction;
    m_pResetModuleAction = NULL;
  }

  if(m_pRunModuleAction)
  {
    delete m_pRunModuleAction;
    m_pRunModuleAction = NULL;
  }

  if(m_pMarkAsPriority)
  {
    delete m_pMarkAsPriority;
    m_pMarkAsPriority = NULL;
  }
}

//---------------------------------------------setupUi
void HostModuleUIInternalView::setupUi(QWidget* pParent)
{
  m_aUi.setupUi(pParent);
}

//---------------------------------------------autoArrangeModules
void HostModuleUIInternalView::autoArrangeModules()
{
  if(!m_collModules.size())
    return;

  int iModuleSpacing = 30;
  QPoint aCurrentPos((width() - (*m_collModules.begin())->width()) / 2, iModuleSpacing);
  tModuleWidgetCollection::iterator it  = m_collModules.begin();
  tModuleWidgetCollection::iterator end = m_collModules.end();
  for(; it!=end; ++it)
  {
    UI::ModuleWidget* pCurrent = *it;
    pCurrent->move(aCurrentPos);
    aCurrentPos.setY(aCurrentPos.y() + pCurrent->height() + iModuleSpacing);
  }

  if(m_pWorkspace)
  {
    m_pWorkspace->autoResize();
  }
}

//---------------------------------------------onNewProject
void HostModuleUIInternalView::onNewProject()
{
  if(m_pWorkspace)
  {
    onCloseProject();
  }
  CLAY_ASSERT(m_pWorkspace == NULL);
  m_pWorkspace = createProject("<unnamed>");
}

//---------------------------------------------onAddModule
void HostModuleUIInternalView::onAddModule()
{
  QtListDialog aDialog(this);
  aDialog.init();

  ModuleRegistry* pRegistry = m_pHostModule->getRegistry();
  ModuleRegistry::const_iterator it = pRegistry->begin();
  ModuleRegistry::const_iterator end = pRegistry->end();
  for(unsigned int i=0; it!=end; ++it, ++i)
  {
      QString sCompoundEntry = it->first.c_str();
      aDialog.addEntry(sCompoundEntry, i);
  }

  int iResult = aDialog.exec();
  if(iResult == QDialog::Accepted)
  {
    unsigned int idx = aDialog.getSelectedUserData().toUInt();

    const char* moduleURI = pRegistry->getModuleURI(idx);
    if(moduleURI)
    {
      //build a generic module name
      unsigned int uNumModules = m_pHostModule->getNumHostedModules();

      tString sUniqueModuleId = moduleURI;
      size_t offset = sUniqueModuleId.find_last_of('/');
      sUniqueModuleId = (offset != tString::npos) ? sUniqueModuleId.substr(offset+1) + LexicalConversion::toString(uNumModules) : LexicalConversion::toString(uNumModules);

      onAddModule(moduleURI, sUniqueModuleId);

      m_pHostModule->shapeProcess();

      if(m_pWorkspace)
      {
        m_pWorkspace->autoResize();
      }
    }
  }
}

//---------------------------------------------onSaveProject
void HostModuleUIInternalView::onSaveProject()
{
  QString sFilename = QFileDialog::getSaveFileName(this);
  if(!sFilename.isEmpty())
  {
    if(!m_pParent->saveProject(sFilename.toAscii().constData()))
    {
      QMessageBox::warning(this, "Error saving project", "An error occurred during saving the project");
    }
  }
}

//---------------------------------------------onLoadProject
void HostModuleUIInternalView::onLoadProject()
{
  QString sFilename = QFileDialog::getOpenFileName(this);
  if(!sFilename.isEmpty())
  {
    onLoadProject(sFilename);
  }
}

//---------------------------------------------onCloseProject
void HostModuleUIInternalView::onCloseProject()
{
  if(m_collModules.size())
  {
    tModuleWidgetCollection::iterator it  = m_collModules.begin();
    tModuleWidgetCollection::iterator end = m_collModules.end();
    for(; it!=end; ++it)
    {
      UI::ModuleWidget* pWidget = *it;
      CLAY_ASSERT(pWidget);
      pWidget->deInit(); //will be deleted later (s.b.)
      pWidget->setParent(NULL);
      QtModuleUIRegistry::instance()->destroyUI(pWidget);
    }
    m_collModules.clear();
  }

  if(m_pWorkspace)
  {
    m_pWorkspace->deInit();
    delete m_pWorkspace; //also deletes all ModuleWidgets (as they are child-widgets)
    m_pWorkspace = NULL;
  }

  m_pHostModule->closeProject();

  activateProjectDependents(false);
}

//---------------------------------------------onPlayButton
void HostModuleUIInternalView::onPlayButton()
{
  CLAY::Module::ProcessResultCode eResult = m_pHostModule->play();
  CLAY_ASSERT(eResult == Module::Process_OK);
  //m_pHostModule->compileAndRun();
}

//---------------------------------------------onSettingsButton
void HostModuleUIInternalView::onSettingsButton()
{
  QString sFn = QFileDialog::getOpenFileName(this, "Choose external module");
  if(!sFn.isEmpty())
  {
    tString sNuggetFn = sFn.toAscii().constBegin();
    NUGGET::IClayNugget* pNugget = m_pHostModule->loadNugget(sNuggetFn);
    CLAY_ASSERT(pNugget);
  }
}

//---------------------------------------------dragEnterEvent
void HostModuleUIInternalView::dragEnterEvent(QDragEnterEvent* pEvent)
{
  if (pEvent->mimeData()->hasFormat(MODULE_DRAG_DROP_FORMAT)) 
  {
    pEvent->setDropAction(Qt::MoveAction);
    pEvent->accept();
  } 
  else 
  {
    pEvent->ignore();
  }
}

//---------------------------------------------dragMoveEvent
void HostModuleUIInternalView::dragMoveEvent(QDragMoveEvent* pEvent)
{
  if (pEvent->mimeData()->hasFormat(MODULE_DRAG_DROP_FORMAT)) 
  {
    pEvent->setDropAction(Qt::MoveAction);
    pEvent->accept();
  } 
  else 
  {
    pEvent->ignore();
  }
}

//---------------------------------------------dropEvent
void HostModuleUIInternalView::dropEvent(QDropEvent* pEvent)
{
  if (pEvent->mimeData()->hasFormat(MODULE_DRAG_DROP_FORMAT)) 
  {
    QByteArray aItemData = pEvent->mimeData()->data(MODULE_DRAG_DROP_FORMAT);
    QDataStream aDataStream(&aItemData, QIODevice::ReadOnly);

    pEvent->setDropAction(Qt::MoveAction);
    pEvent->accept();

    QString sModuleRuntimeId;
    aDataStream >> sModuleRuntimeId;
    Module* pModule = m_pHostModule->getModule(sModuleRuntimeId.toAscii().constData());
    CLAY_ASSERT(pModule);
    if(pModule)
    {
      UI::ModuleWidget* pModuleWidget = getModuleWidget(pModule);
      CLAY_ASSERT(pModuleWidget);     

      QPoint aHotspot;
      aDataStream >> aHotspot;

      QPoint aTranslatedPos = m_pWorkspace->mapFrom(this, pEvent->pos());
      pModuleWidget->move(aTranslatedPos - aHotspot);
      m_pWorkspace->autoResize(); //trigger redraw of connections and auto resize the widget
    }
  } 
  else 
  {
    pEvent->ignore();
  }
}

//---------------------------------------------onAddModule
Module* HostModuleUIInternalView::onAddModule(const char* moduleURI,
                                              const tString& sRuntimeModuleId,
                                              XERCES::DOMNode* pConfigNode)
{
  Module* pModule = m_pHostModule->createModule(moduleURI,
                                                sRuntimeModuleId, 
                                                pConfigNode, 
                                                true);
  addModuleUI(pModule);
  return pModule;
}

//---------------------------------------------onRemoveModule
void HostModuleUIInternalView::onRemoveModule(UI::ModuleWidget* pWidget)
{
  Module* pModule = pWidget->getModule();
  CLAY_ASSERT(pModule);

  removeModuleUI(pWidget); // remove the UI 
  m_pHostModule->removeModule(pModule); // remove the module itself
}

//---------------------------------------------onRemoveModuleConnections
void HostModuleUIInternalView::onRemoveModuleConnections(UI::ModuleWidget* pWidget)
{
  m_pWorkspace->removeConnections(pWidget);
  Module* pModule = pWidget->getModule();
  pModule->disconnect();
}

//---------------------------------------------onLoadProject
void HostModuleUIInternalView::onLoadProject(const QString& sFilename)
{
  onCloseProject();
  CLAY_ASSERT(!m_pWorkspace);
  m_pWorkspace = createProject(sFilename);

  if(m_pHostModule->load(sFilename.toAscii().constData()))
  {
    createModuleWidgets();
    createConnectionWires();
    autoArrangeModules(); //TODO: restore the module positions etc.
    repaint();
  }
  else
  {
    QMessageBox::warning(this, "Error Opening Project", "The project could not be restored", QMessageBox::Ok);
  }
}

//---------------------------------------------addModuleUI
UI::ModuleWidget* HostModuleUIInternalView::addModuleUI(Module* pModule, unsigned int uX, unsigned int uY)
{
  UI::ModuleWidget* pModuleWidget = QtModuleUIRegistry::instance()->createUI(pModule, 
                                                                             pModule->getModuleURI(), 
                                                                             m_pHostModule,
                                                                             //m_pParent,
                                                                             m_pWorkspace);

  if(pModuleWidget)
  {
    pModuleWidget->move(uX, uY);
    pModuleWidget->show();

    m_collModules.push_back(pModuleWidget);

    QObject::connect(pModuleWidget, SIGNAL(selected(UI::ModuleWidget*)), this, SLOT(onWidgetSelected(UI::ModuleWidget*)));
    QObject::connect(pModuleWidget, SIGNAL(showContextMenu(UI::ModuleWidget*, QMouseEvent*)), this, SLOT(onRequestContextMenu(UI::ModuleWidget*, QMouseEvent*)));

    return pModuleWidget;
  }
  else
  {
    CLAY_FAIL();
  }
  return NULL;
}

//---------------------------------------------removeModuleUI
void HostModuleUIInternalView::removeModuleUI(ModuleWidget* pWidget)
{
  tModuleWidgetCollection::iterator pos = std::find(m_collModules.begin(), m_collModules.end(), pWidget);
  if(pos != m_collModules.end())
  {
    m_collModules.erase(pos);
  }
  else
  {
    CLAY_FAIL();
  }

  m_pWorkspace->removeConnections(pWidget);

  pWidget->hide();
  pWidget->deInit();
  pWidget->setParent(NULL); 
  delete pWidget;
}

//---------------------------------------------createProject
QtViewerWorkspace* HostModuleUIInternalView::createProject(const QString& aProjectName)
{
  QScrollArea* pScrollArea = new QScrollArea;
  pScrollArea->setBackgroundRole(QPalette::Base);
    
  QtViewerWorkspace* pWorkspace = new QtViewerWorkspace(pScrollArea);
  pWorkspace->init();
  pScrollArea->setWidget(pWorkspace);
  
  m_aUi.m_pTabWidget->addTab(pScrollArea, aProjectName);
  activateProjectDependents(true);
  return pWorkspace;
}

//---------------------------------------------onConnectionEstablished
void HostModuleUIInternalView::onConnectionEstablished(Module* pFrom, Module* pTo)
{
  m_pWorkspace->addConnection(getModuleWidget(pFrom), getModuleWidget(pTo));
  m_pHostModule->shapeProcess();
}
    
//---------------------------------------------onClayNuggetLoaded
void HostModuleUIInternalView::onClayNuggetLoaded(NUGGET::IClayNugget* pNugget)
{
  QtModuleUIRegistry::instance()->registerModuleUIs(pNugget); //register the UIs in this nugget
}

//---------------------------------------------getNumWidgets
unsigned int HostModuleUIInternalView::getNumWidgets() const
{
  return m_collModules.size();
}

//---------------------------------------------beginWidgets
HostModuleUIInternalView::tConstWidgetIterator HostModuleUIInternalView::beginWidgets() const
{
  return m_collModules.begin();
}

//---------------------------------------------endWidgets
HostModuleUIInternalView::tConstWidgetIterator HostModuleUIInternalView::endWidgets() const
{
  return m_collModules.end();
}

//---------------------------------------------getNumConnections
unsigned int HostModuleUIInternalView::getNumConnections() const
{
  return m_pWorkspace->getNumConnections();
}

//---------------------------------------------beginConnections
HostModuleUIInternalView::tConstConnectionIterator HostModuleUIInternalView::beginConnections() const
{
  return m_pWorkspace->beginConnections();
}

//---------------------------------------------endConnections
HostModuleUIInternalView::tConstConnectionIterator HostModuleUIInternalView::endConnections() const
{
  return m_pWorkspace->endConnections();
}

//---------------------------------------------addWidgetConnection
void HostModuleUIInternalView::addWidgetConnection(ModuleWidget* pFrom, ModuleWidget* pTo)
{
  m_pWorkspace->addConnection(pFrom, pTo);
}

//---------------------------------------------getModuleWidget
ModuleWidget* HostModuleUIInternalView::getModuleWidget(const tString& sModuleRuntimeID)
{
  const HostModuleUIInternalView* pConstThis = this;
  return const_cast<ModuleWidget*>(pConstThis->getModuleWidget(sModuleRuntimeID));
}

//---------------------------------------------getModuleWidget
const ModuleWidget* HostModuleUIInternalView::getModuleWidget(const tString& sModuleRuntimeID) const
{
  tConstWidgetIterator it = beginWidgets();
  tConstWidgetIterator ed = endWidgets();
  for(; it!=ed; ++it)
  {
    const ModuleWidget* pWidget = *it;
    if(pWidget->getModule()->getRuntimeModuleID() == sModuleRuntimeID)
    {
      return pWidget;
    }
  }
  return NULL;
}

//---------------------------------------------onLoadFinished
void HostModuleUIInternalView::onLoadFinished()
{
  if(m_pWorkspace)
  {
    m_pWorkspace->autoResize();
  }
}

//---------------------------------------------onWidgetSelected
void HostModuleUIInternalView::onWidgetSelected(UI::ModuleWidget* pWidget)
{
  //TODO: what should we do if a widget gets selected 
  //implement a hightlighted UI state
}

//---------------------------------------------onRequestContextMenu
void HostModuleUIInternalView::onRequestContextMenu(UI::ModuleWidget* pWidget, QMouseEvent* pEvent)
{
  CLAY_ASSERT(m_pRemoveModuleAction);
  CLAY_ASSERT(m_pRemoveModuleConnectionsAction);
  CLAY_ASSERT(m_pShowCustomWidgetAction);

  QMenu menu(this);
  menu.addAction(m_pShowCustomWidgetAction);
  menu.addSeparator();
  menu.addAction(m_pResetModuleAction);
  menu.addAction(m_pRunModuleAction);
  menu.addSeparator();
  menu.addAction(m_pRemoveModuleAction);
  menu.addAction(m_pRemoveModuleConnectionsAction);
  menu.addSeparator();
  menu.addAction(m_pMarkAsPriority);
  pWidget->onShowContextMenu(menu);
  QAction* pDecision = menu.exec(pEvent->globalPos());
  if(pDecision)
  {
    if(pDecision == m_pRemoveModuleAction)
    {
      onRemoveModule(pWidget);
    }
    else if(pDecision == m_pRemoveModuleConnectionsAction)
    {
      onRemoveModuleConnections(pWidget);
    }
    else if(pDecision == m_pShowCustomWidgetAction)
    {
      pWidget->showModuleUI();
    }
    else if(pDecision == m_pResetModuleAction)
    {
      pWidget->onResetButton();
    }
    else if(pDecision == m_pRunModuleAction)
    {
      pWidget->onRunButton();
    }
    else if(pDecision == m_pMarkAsPriority)
    {
      m_pHostModule->addPriorityModule(pWidget->getModule());
    }
    else //check if the action belongs to the widget
    {
      pWidget->onContextMenu(pDecision);
    }
  }
}

//---------------------------------------------getModuleWidget
UI::ModuleWidget* HostModuleUIInternalView::getModuleWidget(Module* pModule)
{
  tModuleWidgetCollection::iterator it  = m_collModules.begin();
  tModuleWidgetCollection::iterator end = m_collModules.end();
  for(; it!=end; ++it)
  {
    if((*it)->getModule() == pModule)
    {
      return *it;
    }
  }
  return NULL;
}

//---------------------------------------------createModuleWidgets
void HostModuleUIInternalView::createModuleWidgets()
{
  const Host* pHost = m_pHostModule;
  Host::tConstModuleIterator it  = pHost->beginModules();
  Host::tConstModuleIterator end = pHost->endModules();
  for(; it!=end; ++it)
  {
    Module* pModule = *it;
    addModuleUI(pModule);
  }
}

//---------------------------------------------createConnectionWires
void HostModuleUIInternalView::createConnectionWires()
{
  CLAY_ASSERT(m_pWorkspace);

  const Host* pHost = m_pHostModule;
  Host::tConstModuleIterator it  = pHost->beginModules();
  Host::tConstModuleIterator end = pHost->endModules();
  for(; it!=end; ++it)
  {
    Module* pFromModule = *it;
    Module::tOutputIterator ti  = pFromModule->beginOutput();
    Module::tOutputIterator dne = pFromModule->endOutput();
    for(; ti!=dne; ++ti)
    {
      ModuleOutputBase* pOutput = *ti;
      unsigned int uNumConnections = pOutput->getNumConnections();
      for(unsigned int i=0; i<uNumConnections; ++i)
      {
        Module* pToModule = pOutput->getTargetModule(i);
        CLAY_ASSERT(getModuleWidget(pFromModule));
        CLAY_ASSERT(getModuleWidget(pToModule));
        m_pWorkspace->addConnection(getModuleWidget(pFromModule), getModuleWidget(pToModule));
      }
    }
  }
}

//---------------------------------------------activateProjectDependents
void HostModuleUIInternalView::activateProjectDependents(bool bEnable)
{
  m_aUi.m_pAddModuleButton->setEnabled(bEnable);
  m_aUi.m_pSaveButton->setEnabled(bEnable);
  m_aUi.m_pCloseButton->setEnabled(bEnable);
  m_aUi.m_pPlayButton->setEnabled(bEnable);
}
  
} }
