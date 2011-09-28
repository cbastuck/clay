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

#include "QtViewerMainWindow.h"

#include "ModuleWidget.h"
#include "QtListDialog.h"
#include "QtViewerWorkspace.h"

#include <clay-core/base/ClayDefines.h>
#include <clay-modules/StandardRegistry.h>
#include <clay-gui-qt/base/QtModuleUIRegistry.h>
#include <clay-core/helper/LexicalConversion.h>

//Qt
#include <QDragEnterEvent>
#include <QDataStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

#define XML_QT_VIEWER_ROOT_TAG "qt-viewer-host"

namespace CLAY
{
  namespace UI
  {

    //---------------------------------------------QtViewerMainWindow
    QtViewerMainWindow::QtViewerMainWindow()
      : Host(MODULE::StandardRegistry::instance()),
        m_pWorkspace(NULL)
    {
      m_aUi.setupUi(this);
    }

    //---------------------------------------------init
    bool QtViewerMainWindow::init()
    {
      m_aUi.m_pTabWidget->clear();
      m_pWorkspace = createProject("<unnamed>"); //create initial project

      QObject::connect(m_aUi.actionAdd_Module,      SIGNAL(triggered()), this, SLOT(onAddModule()));
      QObject::connect(m_aUi.actionSave_Project,    SIGNAL(triggered()), this, SLOT(onSaveProject()));
      QObject::connect(m_aUi.actionLoad_Project,    SIGNAL(triggered()), this, SLOT(onLoadProject()));
      //QObject::connect(m_aUi.menuRecent_Projects,   SIGNAL(triggered()), this, SLOT(onRecentProjects()));
      QObject::connect(m_aUi.actionClose_Project,   SIGNAL(triggered()), this, SLOT(onCloseProject()));
      QObject::connect(m_aUi.m_pPlayButton,         SIGNAL(pressed()),   this, SLOT(onPlayButton()));

      setAcceptDrops(true);

      //TESTING
      //initTestEnvironment();

      QFile aSettignsFile("settings.mf");
      if(aSettignsFile.exists() && aSettignsFile.open(QIODevice::ReadOnly))
      {
        QDataStream aStream(&aSettignsFile);
        while(!aStream.atEnd())
        {
          QString sRecentFile;
          aStream >> sRecentFile;
          m_collRecentProjectsStack.push_back(sRecentFile);
        }
      }
      
      restoreRecentProjects();

      return tBase::init();
    }

    //---------------------------------------------deInit
    void QtViewerMainWindow::deInit()
    {
      tBase::deInit();

      QFile aSettignsFile("settings.mf");
      if(aSettignsFile.open(QIODevice::WriteOnly))
      {
        QDataStream aStream(&aSettignsFile);

        tRecentProjectsStack::const_iterator it  = m_collRecentProjectsStack.begin();
        tRecentProjectsStack::const_iterator end = m_collRecentProjectsStack.end();
        for(unsigned int i=0; it!=end && i<5; ++it, ++i)
        {
          aStream << *it;
        }
      }
    }

    //---------------------------------------------connect
    bool QtViewerMainWindow::connect(Module* pFrom, Module* pTo)
    {
      if(Host::connect(pFrom, pTo)) //base implementation
      {
        UI::ModuleWidget* pFromUI = getModuleWidget(pFrom);
        UI::ModuleWidget* pToUI   = getModuleWidget(pTo);
        CLAY_ASSERT(pFromUI);
        CLAY_ASSERT(pToUI);
        m_pWorkspace->addConnection(pFromUI, pToUI); //class is responsible for drawing connections
        m_pWorkspace->update();
        return true;
      }
      CLAY_FAIL();
      return false;    
    }

    //---------------------------------------------saveProject
    bool QtViewerMainWindow::saveProject(XERCES::DOMElement* pNode)
    {
      if(tBase::saveProject(pNode)) //call case implementation
      {
        return true;
      }
      return false;
    }

    //---------------------------------------------loadProject
    bool QtViewerMainWindow::loadProject(const tString& sFilename)
    {
      QString sFn = sFilename.c_str();
      if((getNumRecentProjects() == 0) ||
          (m_collRecentProjectsStack.front() != sFn))
      {
        m_collRecentProjectsStack.push_front(sFn); //no same multiples
      }
      return tBase::loadProject(sFilename);
    }

    //---------------------------------------------loadModule
    Module* QtViewerMainWindow::loadModule(XERCES::DOMElement* pNode, Module::tConnectionMap* pInputConnections, Module::tConnectionMap* pOutputConnections)
    {
      Module* pModule = tBase::loadModule(pNode, pInputConnections, pOutputConnections);
      if(pModule)
      {
        addModuleUI(pModule); //create the UIs to the given module
      }
      return pModule;
    }

    //---------------------------------------------closeProject
    void QtViewerMainWindow::closeProject()
    {
      tBase::closeProject();

      tModuleWidgetCollection::iterator it  = m_collModules.begin();
      tModuleWidgetCollection::iterator end = m_collModules.end();
      for(; it!=end; ++it)
      {
        UI::ModuleWidget* pCurrent = *it;
        pCurrent->deInit();
        delete pCurrent;
      }
      m_collModules.clear();
    }

    //---------------------------------------------onAddModule
    void QtViewerMainWindow::onAddModule()
    {
      QtListDialog aDialog(this);
      aDialog.init();
      
      ModuleRegistry::namespace_iterator it  = MODULE::StandardRegistry::instance()->beginNamespace();
      ModuleRegistry::namespace_iterator end = MODULE::StandardRegistry::instance()->endNamespace();
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
        unsigned int uNumModules = getNumHostedModules();
        tString sUniqueModuleId  = IntegerDecoding::toString(&aModuleId) + LexicalConversion::toString(uNumModules);

        onAddModule(aModuleId, sUniqueModuleId, aNamespaceId);
      }
    }

    //---------------------------------------------onSaveProject
    void QtViewerMainWindow::onSaveProject()
    {
      QString sFilename = QFileDialog::getSaveFileName(this);
      if(!sFilename.isEmpty())
      {
        XERCES::DOMDocument* pDoc = XercesXML::createDOMDoc(XML_QT_VIEWER_ROOT_TAG);
        XERCES::DOMElement* pRoot = pDoc->getDocumentElement();
        saveProject(pRoot);

        XercesXML::saveDOMElement(pRoot, sFilename.toAscii().constData());
      }
    }

    //---------------------------------------------onLoadProject
    void QtViewerMainWindow::onLoadProject()
    {
      QString sFilename = QFileDialog::getOpenFileName(this);
      if(!sFilename.isEmpty())
      {
        onLoadProject(sFilename);
      }
    }

    //---------------------------------------------onRecentProjects
    void QtViewerMainWindow::onRecentProjects()
    {
      //TODO: 
      onLoadProject("C:\\myproj.xml");
    }

    //---------------------------------------------onAddModule
    void QtViewerMainWindow::onAddModule(ModuleDescriptorTraits::tModuleID    aModuleId,
                                         const tString&                       sRuntimeModuleId,
                                         ModuleDescriptorTraits::tNamespaceID aNamespaceId)
    {
      Module* pModule = createModule(aModuleId, aNamespaceId, sRuntimeModuleId, NULL, true);
      addModuleUI(pModule);
    }

    //---------------------------------------------autoArrangeModules
    void QtViewerMainWindow::autoArrangeModules()
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
    }

    //---------------------------------------------onPlayButton
    void QtViewerMainWindow::onPlayButton()
    {
      CLAY::Module::ProcessResultCode eResult = play();
      CLAY_ASSERT(eResult == Module::Process_OK);
    }

    //---------------------------------------------onCloseProject
    void QtViewerMainWindow::onCloseProject()
    {
      closeProject();
    }

    //---------------------------------------------onLoadRecentProject1
    void QtViewerMainWindow::onLoadRecentProject1()
    {
      loadProject(getRecentProject(0).toAscii().constData());
    }

    //---------------------------------------------onLoadRecentProject2
    void QtViewerMainWindow::onLoadRecentProject2()
    {
      loadProject(getRecentProject(1).toAscii().constData());
    }

    //---------------------------------------------onLoadRecentProject3
    void QtViewerMainWindow::onLoadRecentProject3()
    {
      loadProject(getRecentProject(2).toAscii().constData());
    }
    
    //---------------------------------------------onLoadRecentProject4
    void QtViewerMainWindow::onLoadRecentProject4()
    {
      loadProject(getRecentProject(3).toAscii().constData());
    }
    
    //---------------------------------------------onLoadRecentProject5
    void QtViewerMainWindow::onLoadRecentProject5()
    {
      loadProject(getRecentProject(4).toAscii().constData());
    }

    //---------------------------------------------restoreInputConnections
    bool QtViewerMainWindow::restoreInputConnections(const Module::tConnectionMap& collInputConnections)
    {
      if(tBase::restoreInputConnections(collInputConnections))
      {
        Module::tConnectionMap::const_iterator it  = collInputConnections.begin();
        Module::tConnectionMap::const_iterator end = collInputConnections.end();
        for(; it!=end; ++it)
        {
          Module* pInputModule  = it->first;
          const Module::tConnectionColl& aCollection = it->second;

          Module::tConnectionColl::const_iterator ti = aCollection.begin();
          Module::tConnectionColl::const_iterator de = aCollection.end();
          for(; ti!=de; ++ti)
          {
            Module* pOutputModule = getModule(ti->outputModule);
            if(pOutputModule && pInputModule)
            {
              UI::ModuleWidget* pInputModuleUI  = getModuleWidget(pInputModule);
              UI::ModuleWidget* pOutputModuleUI = getModuleWidget(pOutputModule);
              m_pWorkspace->addConnection(pOutputModuleUI, pInputModuleUI);
            }
            else
            {
              return false;
            }
          }
        }
        return true;
      }
      return false;
    }

    //---------------------------------------------restoreOutputConnections
    bool QtViewerMainWindow::restoreOutputConnections(const Module::tConnectionMap& collOutputConnections)
    {
      if(tBase::restoreOutputConnections(collOutputConnections))
      {
        Module::tConnectionMap::const_iterator it  = collOutputConnections.begin();
        Module::tConnectionMap::const_iterator end = collOutputConnections.end();
        for(; it!=end; ++it)
        {
          Module* pOutputModule = it->first;
          const Module::tConnectionColl& aCollection = it->second;

          Module::tConnectionColl::const_iterator ti = aCollection.begin();
          Module::tConnectionColl::const_iterator de = aCollection.end();
          for(; ti!=de; ++ti)
          {
            Module* pInputModule  = getModule(ti->inputModule);
            if(pOutputModule && pInputModule)
            {
              UI::ModuleWidget* pInputModuleUI  = getModuleWidget(pInputModule);
              UI::ModuleWidget* pOutputModuleUI = getModuleWidget(pOutputModule);
              m_pWorkspace->addConnection(pInputModuleUI, pOutputModuleUI);
            }
            else
            {
              return false;
            }
          }
        }
        return true;
      }
      return false;
    }

    //---------------------------------------------dragEnterEvent
    void QtViewerMainWindow::dragEnterEvent(QDragEnterEvent* pEvent)
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
    void QtViewerMainWindow::dragMoveEvent(QDragMoveEvent* pEvent)
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
    void QtViewerMainWindow::dropEvent(QDropEvent* pEvent)
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

        CLAY_ASSERT(m_collModules.size() > uIdx);
        QPoint aTranslatedPos = m_pWorkspace->mapFrom(this, pEvent->pos());
        m_collModules[uIdx]->move(aTranslatedPos - aHotspot);
        m_pWorkspace->update(); //trigger redraw of connections
      } 
      else 
      {
        pEvent->ignore();
      }
    }

    //---------------------------------------------getModuleWidget
    UI::ModuleWidget* QtViewerMainWindow::getModuleWidget(const Module* pModule) const
    {
      tModuleWidgetCollection::const_iterator it  = m_collModules.begin();
      tModuleWidgetCollection::const_iterator end = m_collModules.end();
      for(; it!=end; ++it)
      {
        if((*it)->getModule() == pModule)
          return *it;
      }
      return NULL;
    }

    //---------------------------------------------createProject
    QtViewerWorkspace* QtViewerMainWindow::createProject(const QString& aProjectName)
    {
      QtViewerWorkspace*  pWorkspace = new QtViewerWorkspace(NULL);
      m_aUi.m_pTabWidget->addTab(pWorkspace, aProjectName);
      return pWorkspace;
    }

    //---------------------------------------------onLoadProject
    void QtViewerMainWindow::onLoadProject(const QString& sFilename)
    {
      if(loadProject(sFilename.toAscii().constData()))
      {
        autoArrangeModules(); //TODO: restore the module positions etc.
        QMainWindow::repaint();
      }
      else
      {
        QMessageBox::warning(this, "Error Opening Project", "The project could not be restored", QMessageBox::Ok);
      }
    }

    //---------------------------------------------addModuleUI
    void QtViewerMainWindow::addModuleUI(Module* pModule, unsigned int uX, unsigned int uY)
    {
      UI::ModuleWidget* pModuleWidget = QtModuleUIRegistry::instance()->createUI(pModule, pModule->getModuleDescriptor()->getModuleId(), this, m_pWorkspace);

      if(pModuleWidget)
      {
        pModuleWidget->move(uX, uY);
        pModuleWidget->show();

        m_collModules.push_back(pModuleWidget);
      }
      else
      {
        CLAY_FAIL();
      }
    }

    //---------------------------------------------restoreRecentProjects
    void QtViewerMainWindow::restoreRecentProjects()
    {
      unsigned int uNum = getNumRecentProjects();
      if(uNum > 0)
      {
        QAction* pAction = new QAction(m_aUi.menuRecent_Projects);
        QString sRecentEntry = getRecentProject(0);
        pAction->setObjectName(sRecentEntry);
        pAction->setText(sRecentEntry);
        m_aUi.menuRecent_Projects->addAction(pAction);

        QObject::connect(pAction, SIGNAL(triggered()), this, SLOT(onLoadRecentProject1()));
      }
      if(uNum > 1)
      {
        QAction* pAction = new QAction(m_aUi.menuRecent_Projects);
        QString sRecentEntry = getRecentProject(1);
        pAction->setObjectName(sRecentEntry);
        pAction->setText(sRecentEntry);
        m_aUi.menuRecent_Projects->addAction(pAction);

        QObject::connect(pAction, SIGNAL(triggered()), this, SLOT(onLoadRecentProject2()));
      }
      if(uNum > 2)
      {
        QAction* pAction = new QAction(m_aUi.menuRecent_Projects);
        QString sRecentEntry = getRecentProject(2);
        pAction->setObjectName(sRecentEntry);
        pAction->setText(sRecentEntry);
        m_aUi.menuRecent_Projects->addAction(pAction);

        QObject::connect(pAction, SIGNAL(triggered()), this, SLOT(onLoadRecentProject3()));
      }
      if(uNum > 3)
      {
        QAction* pAction = new QAction(m_aUi.menuRecent_Projects);
        QString sRecentEntry = getRecentProject(3);
        pAction->setObjectName(sRecentEntry);
        pAction->setText(sRecentEntry);
        m_aUi.menuRecent_Projects->addAction(pAction);

        QObject::connect(pAction, SIGNAL(triggered()), this, SLOT(onLoadRecentProject4()));
      }
      if(uNum > 4)
      {
        QAction* pAction = new QAction(m_aUi.menuRecent_Projects);
        QString sRecentEntry = getRecentProject(4);
        pAction->setObjectName(sRecentEntry);
        pAction->setText(sRecentEntry);
        m_aUi.menuRecent_Projects->addAction(pAction);

        QObject::connect(pAction, SIGNAL(triggered()), this, SLOT(onLoadRecentProject5()));
      }
    }

    //---------------------------------------------getNumRecentProjects
    unsigned int QtViewerMainWindow::getNumRecentProjects()
    {
      return m_collRecentProjectsStack.size();
    }

    //---------------------------------------------getRecentProject
    QString QtViewerMainWindow::getRecentProject(unsigned int i)
    {
      CLAY_ASSERT(i < getNumRecentProjects());
      return m_collRecentProjectsStack[i];
    }

    //---------------------------------------------addRecentProject
    void QtViewerMainWindow::addRecentProject(const QString& sProject)
    {
      m_collRecentProjectsStack.push_front(sProject);
    }
  }
}
