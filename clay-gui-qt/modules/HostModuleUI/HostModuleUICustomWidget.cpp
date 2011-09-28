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

#include "HostModuleUICustomWidget.h"

#include <clay-core/host/Host.h>
#include <clay-gui-qt/ModuleWidget.h>
#include <clay-gui-qt/QtListDialog.h>
#include <clay-gui-qt/ModuleUIRegistry.h>
#include <clay-gui-qt/QtViewerWorkspace.h>
#include <clay-core/registry/ModuleRegistry.h>
#include <clay-core/helper/LexicalConversion.h>

#include <QDataStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QDragEnterEvent>

namespace MF
{
  //---------------------------------------------HostModuleUICustomWidget
  HostModuleUICustomWidget::HostModuleUICustomWidget(QWidget* pParent, Host* pHost)
    : QMainWindow(pParent),
      m_pHost(pHost),
      m_pWorkspace(NULL)
  {

  }

  //---------------------------------------------init
  void HostModuleUICustomWidget::init()
  {
    m_aUi.setupUi(this);

    QObject::connect(m_aUi.actionAdd_Module,    SIGNAL(triggered()), this, SLOT(onAddModule()));
    QObject::connect(m_aUi.actionSave_Project,  SIGNAL(triggered()), this, SLOT(onSaveProject()));
    QObject::connect(m_aUi.actionLoad_Project,  SIGNAL(triggered()), this, SLOT(onLoadProject()));
    QObject::connect(m_aUi.actionClose_Project, SIGNAL(triggered()), this, SLOT(onCloseProject()));
    QObject::connect(m_aUi.m_pPlayButton,       SIGNAL(pressed()),   this, SLOT(onPlayButton()));

    setAcceptDrops(true);

    m_aUi.m_pTabWidget->clear();
    m_pWorkspace = createProject("<unnamed>"); //create initial project
  }

  //---------------------------------------------deInit
  void HostModuleUICustomWidget::deInit()
  {

  }

  //---------------------------------------------autoArrangeModules
  void HostModuleUICustomWidget::autoArrangeModules()
  {
    if(!m_collModules.size())
      return;

    int iModuleSpacing = 30;
    QPoint aCurrentPos((width() - (*m_collModules.begin())->width()) / 2, iModuleSpacing);
    tModuleWidgetCollection::iterator it  = m_collModules.begin();
    tModuleWidgetCollection::iterator end = m_collModules.end();
    for(; it!=end; ++it)
    {
      ModuleWidget* pCurrent = *it;
      pCurrent->move(aCurrentPos);
      aCurrentPos.setY(aCurrentPos.y() + pCurrent->height() + iModuleSpacing);
    }
  }


  //---------------------------------------------onAddModule
  void HostModuleUICustomWidget::onAddModule()
  {
    QtListDialog aDialog(this);
    aDialog.init();

    ModuleRegistry* pRegistry = m_pHost->getRegistry();
    ModuleRegistry::namespace_iterator it  = pRegistry->beginNamespace();
    ModuleRegistry::namespace_iterator end = pRegistry->endNamespace();
    for(; it!=end; ++it)
    {
      ModuleDescriptorTraits::tNamespaceID uNamespaceId      = it->first;
      ModuleRegistry::tDescriptorCollection& collDescriptors = it->second;
      ModuleRegistry::descr_iterator ti  = collDescriptors.begin();
      ModuleRegistry::descr_iterator dne = collDescriptors.end();
      for(; ti!=dne; ++ti)
      {
        ModuleDescriptorTraits::tModuleID uModuleId = ti->first;
        QString sModuleId    = QString::fromAscii(reinterpret_cast<char*>(&uModuleId),    sizeof(uModuleId));
        QString sNamespaceId = QString::fromAscii(reinterpret_cast<char*>(&uNamespaceId), sizeof(uNamespaceId));

        unsigned long long aCompoundId = ModuleDescriptorTraits::encode(uModuleId, uNamespaceId);

        QString sCompoundEntry = sNamespaceId + ":"+ sModuleId;
        aDialog.addEntry(sCompoundEntry, aCompoundId);
      }
    }

    int iResult = aDialog.exec();
    if(iResult == QDialog::Accepted)
    {
      unsigned long long aCompound = aDialog.getSelectedUserData().toLongLong();

      ModuleDescriptorTraits::tModuleID    aModuleId;
      ModuleDescriptorTraits::tNamespaceID aNamespaceId;
      ModuleDescriptorTraits::decode(aCompound, aModuleId, aNamespaceId);

      //build a generic module name
      unsigned int uNumModules = m_pHost->getNumHostedModules();
      tString sUniqueModuleId  = IntegerDecoding::toString(&aModuleId) + LexicalConversion::toString(uNumModules);

      onAddModule(aModuleId, sUniqueModuleId, aNamespaceId);
    }
  }

  //---------------------------------------------onSaveProject
  void HostModuleUICustomWidget::onSaveProject()
  {
    QString sFilename = QFileDialog::getSaveFileName(this);
    if(!sFilename.isEmpty())
    {
      if(!m_pHost->saveProject(sFilename.toAscii().constData()))
      {
        QMessageBox::warning(this, "Error saving project", "An error occurred during saving the project");
      }
    }
  }

  //---------------------------------------------onLoadProject
  void HostModuleUICustomWidget::onLoadProject()
  {
    QString sFilename = QFileDialog::getOpenFileName(this);
    if(!sFilename.isEmpty())
    {
      onLoadProject(sFilename);
    }
  }

  //---------------------------------------------onCloseProject
  void HostModuleUICustomWidget::onCloseProject()
  {
    m_pHost->closeProject();
  }

  //---------------------------------------------onPlayButton
  void HostModuleUICustomWidget::onPlayButton()
  {
    MF::Module::ProcessResultCode eResult = m_pHost->play();
    MF_ASSERT(eResult == Module::Process_OK);
  }


  //---------------------------------------------dragEnterEvent
  void HostModuleUICustomWidget::dragEnterEvent(QDragEnterEvent* pEvent)
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
  void HostModuleUICustomWidget::dragMoveEvent(QDragMoveEvent* pEvent)
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
  void HostModuleUICustomWidget::dropEvent(QDropEvent* pEvent)
  {
    if (pEvent->mimeData()->hasFormat(MODULE_DRAG_DROP_FORMAT)) 
    {
      QByteArray aItemData = pEvent->mimeData()->data(MODULE_DRAG_DROP_FORMAT);
      QDataStream aDataStream(&aItemData, QIODevice::ReadOnly);

      pEvent->setDropAction(Qt::MoveAction);
      pEvent->accept();

      unsigned int uIdx;
      aDataStream >> uIdx;

      QPoint aHotspot;
      aDataStream >> aHotspot;

      MF_ASSERT(m_collModules.size() > uIdx);
      QPoint aTranslatedPos = m_pWorkspace->mapFrom(this, pEvent->pos());
      m_collModules[uIdx]->move(aTranslatedPos - aHotspot);
      m_pWorkspace->update(); //trigger redraw of connections
    } 
    else 
    {
      pEvent->ignore();
    }
  }

  //---------------------------------------------onAddModule
  void HostModuleUICustomWidget::onAddModule(ModuleDescriptorTraits::tModuleID    aModuleId,
                                             const tString&                       sRuntimeModuleId,
                                             ModuleDescriptorTraits::tNamespaceID aNamespaceId)
  {
    Module* pModule = m_pHost->createModule(aModuleId, aNamespaceId, sRuntimeModuleId, NULL);
    addModuleUI(pModule);
  }

  //---------------------------------------------onLoadProject
  void HostModuleUICustomWidget::onLoadProject(const QString& sFilename)
  {
    if(m_pHost->loadProject(sFilename.toAscii().constData()))
    {
      autoArrangeModules(); //TODO: restore the module positions etc.
      repaint();
    }
    else
    {
      QMessageBox::warning(this, "Error Opening Project", "The project could not be restored", QMessageBox::Ok);
    }
  }

  //---------------------------------------------addModuleUI
  void HostModuleUICustomWidget::addModuleUI(Module* pModule, unsigned int uX, unsigned int uY)
  {
    unsigned int uIdx = m_collModules.size();
    ModuleWidget* pModuleWidget = ModuleUIRegistry::instance()->createUI(pModule, pModule->getModuleDescriptor()->retrieveModuleId(), LexicalConversion::toString(uIdx), m_pHost, m_pWorkspace);

    if(pModuleWidget)
    {
      pModuleWidget->move(uX, uY);
      pModuleWidget->show();

      m_collModules.push_back(pModuleWidget);
    }
    else
    {
      MF_FAIL();
    }
  }

  //---------------------------------------------createProject
  QtViewerWorkspace* HostModuleUICustomWidget::createProject(const QString& aProjectName)
  {
    QtViewerWorkspace* pWorkspace = new QtViewerWorkspace(NULL);
    m_aUi.m_pTabWidget->addTab(pWorkspace, aProjectName);
    return pWorkspace;
  }
}
