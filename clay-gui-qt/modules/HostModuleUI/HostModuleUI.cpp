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

#include "HostModuleUI.h"
#include "HostModuleUIInternalView.h"

//CLAY
#include <clay-gui-qt/base/QtIOWidget.h>
#include <clay-modules/host/HostModule.h>
#include <clay-core/helper/LexicalConversion.h>

//QT
#include <QFileDialog>
#include <QMessageBox>
#include <QDragEnterEvent>

#define XML_HOST_MODULES_NODE    "modules"
#define XML_HOST_WIDGETS_NODE    "widgets"

#define XML_HOST_UI_ROOT_NODE    "host-module-ui"
#define XML_HOST_UI_CONFIG_NODE  "host-ui-config"
#define XML_HOST_UI_SIZE_X_ATTR  "host-ui-size-x"
#define XML_HOST_UI_SIZE_Y_ATTR  "host-ui-size-y"
#define XML_WIDGETS_NODE         "module-widgets"
#define XML_NUM_WIDGETS_ATTR     "num-widgets"
#define XML_MODULE_ID_ATTR       "module-id"
#define XML_WIDGET_NODE          "module-widget"
#define XML_CONNECTIONS_NODE     "widget-connections"
#define XML_NUM_CONNECTIONS_ATTR "widget-connections"
#define XML_CONNECTION_NODE      "widget-connection"
#define XML_CONNECTION_FROM_ATTR "connection-from"
#define XML_CONNECTION_TO_ATTR   "connection-to"

namespace CLAY{ namespace UI{

//---------------------------------------------create
ModuleWidget* HostModuleUI::create(Module* pModule, Host* pHost, void* pParent)
{
  return new HostModuleUI(pModule, pHost, static_cast<QWidget*>(pParent));
}

//---------------------------------------------destroy
void HostModuleUI::destroy(ModuleWidget* pWidget)
{
  delete pWidget;
}

//---------------------------------------------HostModuleUI
HostModuleUI::HostModuleUI(Module* pModule, Host* pHost, QWidget* pParent)
  : tBase(pModule, pHost, pParent),
    m_pCustomWidget(NULL)
{
  
}

//---------------------------------------------~HostModuleUI
HostModuleUI::~HostModuleUI()
{

}

//---------------------------------------------init
void HostModuleUI::init()
{
  tBase::init();

  MODULE::HostModule* pHostModule = static_cast<MODULE::HostModule*>(getModule());
  if(!m_pCustomWidget)
  {
    m_pCustomWidget = new HostModuleUIInternalView(this, pHostModule);
    m_pCustomWidget->init();
  }
}

//---------------------------------------------deInit
void HostModuleUI::deInit()
{
  if(m_pCustomWidget)
  {
    m_pCustomWidget->deInit();
    delete m_pCustomWidget;
    m_pCustomWidget = NULL;
  }

  tBase::deInit();
}

//---------------------------------------------save
bool HostModuleUI::save(XERCES::DOMElement* pNode) const
{
  XERCES::DOMElement* pWidgetsNode = XercesXML::appendNode(pNode, "widgets");
  if(!saveWidgets(pWidgetsNode))
  {
    return false;
  }

  XERCES::DOMElement* pBaseNode = XercesXML::appendNode(pNode, "base");
  return ModuleWidget::save(pBaseNode);
}

//---------------------------------------------load
bool HostModuleUI::load(XERCES::DOMElement* pNode)
{
  XERCES::DOMElement* pWidgetsNode = XercesXML::findChildNode(pNode, "widgets");
  if(!pWidgetsNode)
  {
    return false;
  }

  if(!loadWidgets(pWidgetsNode))
  {
    return false;
  }

  XERCES::DOMElement* pBaseNode = XercesXML::findChildNode(pNode, "base");
  if(!pBaseNode)
  {
    return false;
  }

  if(!ModuleWidget::load(pBaseNode))
  {
    return false;
  }

  m_pCustomWidget->onLoadFinished();

  return true;
}

//---------------------------------------------onShowContextMenu
void HostModuleUI::onShowContextMenu(QMenu& aMenu)
{

}

//---------------------------------------------onContextMenu
void HostModuleUI::onContextMenu(QAction* pDecision)
{

}

//---------------------------------------------connectModuleSignals
void HostModuleUI::connectModuleSignals()
{
  tBase::connectModuleSignals();
}

//---------------------------------------------showModuleUI
void HostModuleUI::showModuleUI()
{
  if(m_pCustomWidget && !m_pCustomWidget->isVisible())
  {
    m_pCustomWidget->move(tBase::topLevelWidget()->rect().center()); //center the widget
    m_pCustomWidget->show();
  }

  m_pCustomWidget->raise(); //bring to front
}

//---------------------------------------------loadProject
bool HostModuleUI::loadProject(const tString& sFn)
{
  XERCES::DOMDocument* pDoc = XercesXML::loadXML(sFn);
  if(!pDoc)
  {
    return false;
  }

  XERCES::DOMElement* pRoot = pDoc->getDocumentElement();
  if(!pRoot)
  {
    return false;
  }

  XERCES::DOMElement* pModulesNode = XercesXML::findChildNode(pRoot, XML_HOST_MODULES_NODE);
  if(!pModulesNode)
  {
    return false;
  }

  if(!getModule()->load(pModulesNode))
  {
    return false;
  }

  XERCES::DOMElement* pWidgetsNode = XercesXML::findChildNode(pRoot, XML_HOST_WIDGETS_NODE);
  if(!pWidgetsNode)
  {
    return false;
  }

  return load(pWidgetsNode);
}

//---------------------------------------------loadWidgets
bool HostModuleUI::loadWidgets(XERCES::DOMElement* pParent)
{
  XERCES::DOMElement* pRoot = XercesXML::findChildNode(pParent, XML_HOST_UI_ROOT_NODE);

  //restore global settings
  XERCES::DOMElement* pConfigNode = XercesXML::findChildNode(pRoot, XML_HOST_UI_CONFIG_NODE, false);
  if(!pConfigNode)
  {
    return false;
  }
  unsigned int uCustomWidgetWidth = 0;
  if(!XercesXML::getAttributeValue(pConfigNode, XML_HOST_UI_SIZE_X_ATTR, uCustomWidgetWidth))
  {
    return false;
  }
  unsigned int uCustomWidgetHeight = 0;
  if(!XercesXML::getAttributeValue(pConfigNode, XML_HOST_UI_SIZE_Y_ATTR, uCustomWidgetHeight))
  {
    return false;
  }
  m_pCustomWidget->resize(uCustomWidgetWidth, uCustomWidgetHeight);

  //create widgets
  MODULE::HostModule* pHostModule = static_cast<MODULE::HostModule*>(getModule());
  Host::tConstModuleIterator it = pHostModule->beginModules();
  Host::tConstModuleIterator ed = pHostModule->endModules();
  for(; it!=ed; ++it)
  {
    m_pCustomWidget->addModuleUI(*it);
  }

  //restore widget-states
  XERCES::DOMElement* pWidgetsNode = XercesXML::findChildNode(pRoot, XML_WIDGETS_NODE);
  if(!pWidgetsNode) 
  {
    return false;
  }
  XercesXML::visitChildNodes(pWidgetsNode, boost::bind(&HostModuleUI::restoreWidget, this, _1));

  //restore connections
  XERCES::DOMElement* pConnectionsNode = XercesXML::findChildNode(pRoot, XML_CONNECTIONS_NODE);
  if(!pConnectionsNode)
  {
    return false;
  }
  XercesXML::visitChildNodes(pConnectionsNode, boost::bind(&HostModuleUI::restoreConnection, this, _1));

  return true;
}

//---------------------------------------------saveProject
bool HostModuleUI::saveProject(const tString& sFn)
{
  XERCES::DOMDocument* pDoc  = XercesXML::createDOMDoc(XML_HOST_UI_ROOT_NODE);
  XERCES::DOMElement*  pRoot = pDoc->getDocumentElement();

  XERCES::DOMElement* pModulesNode = XercesXML::appendNode(pRoot, XML_HOST_MODULES_NODE);
  if(!getModule()->save(pModulesNode))
  {
    return false;
  }

  XERCES::DOMElement* pWidgetsNode = XercesXML::appendNode(pRoot, XML_HOST_WIDGETS_NODE);
  if(!save(pWidgetsNode))
  {
    return false;
  }

  return XercesXML::saveDOMElement(pRoot, sFn.c_str());
}

//---------------------------------------------saveWidgets
bool HostModuleUI::saveWidgets(XERCES::DOMElement* pParent) const
{
  XERCES::DOMElement* pRoot = XercesXML::appendNode(pParent, XML_HOST_UI_ROOT_NODE);
  
  //write host window configuration
  XERCES::DOMElement* pConfigNode = XercesXML::appendNode(pRoot, XML_HOST_UI_CONFIG_NODE);
  QSize aSize = m_pCustomWidget->size();
  XercesXML::addAttribute(pConfigNode, XML_HOST_UI_SIZE_X_ATTR, aSize.width());
  XercesXML::addAttribute(pConfigNode, XML_HOST_UI_SIZE_Y_ATTR, aSize.height());

  //write widgets configuration
  unsigned int uNumWidgets = m_pCustomWidget->getNumWidgets();
  XERCES::DOMElement* pWidgetsNode = XercesXML::appendNode(pRoot, XML_WIDGETS_NODE);
  XercesXML::addAttribute(pWidgetsNode, XML_NUM_WIDGETS_ATTR, uNumWidgets);

  //write widget-states
  HostModuleUIInternalView::tConstWidgetIterator it = m_pCustomWidget->beginWidgets();
  HostModuleUIInternalView::tConstWidgetIterator ed = m_pCustomWidget->endWidgets();
  for(; it!=ed; ++it)
  {
    const ModuleWidget* pWidget = *it; 
    XERCES::DOMElement* pWidgetNode = XercesXML::appendNode(pWidgetsNode, XML_WIDGET_NODE);
    XercesXML::addAttribute(pWidgetNode, XML_MODULE_ID_ATTR, pWidget->getModule()->getRuntimeModuleID());
    pWidget->save(pWidgetNode);
  }

  //write connections lines
  unsigned int uNumConnections = m_pCustomWidget->getNumConnections();
  XERCES::DOMElement* pConnectionsNode = XercesXML::appendNode(pRoot, XML_CONNECTIONS_NODE);  
  XercesXML::addAttribute(pConnectionsNode, XML_NUM_CONNECTIONS_ATTR, uNumConnections);

  HostModuleUIInternalView::tConstConnectionIterator ti = m_pCustomWidget->beginConnections();
  HostModuleUIInternalView::tConstConnectionIterator de = m_pCustomWidget->endConnections();
  for(; ti!=de; ++ti)
  {
    const ModuleWidget* pFrom = ti->first;
    const ModuleWidget* pTo   = ti->second;

    XERCES::DOMElement* pConnectionNode = XercesXML::appendNode(pConnectionsNode, XML_CONNECTION_NODE);

    XercesXML::addAttribute(pConnectionNode, XML_CONNECTION_FROM_ATTR, pFrom->getModule()->getRuntimeModuleID());
    XercesXML::addAttribute(pConnectionNode, XML_CONNECTION_TO_ATTR, pTo->getModule()->getRuntimeModuleID());
  }
  
  return true;
}

//---------------------------------------------dragEnterEvent
void HostModuleUI::dragEnterEvent(QDragEnterEvent* pEvent)
{
  if(pEvent->mimeData()->hasFormat(QtIOWidget::getModuleOutputDnDFormat()))
  {
    QByteArray aItemData = pEvent->mimeData()->data(QtIOWidget::getModuleOutputDnDFormat());
    QDataStream aDataStream(&aItemData, QIODevice::ReadOnly);

    QtIOWidget* pIOWidget = NULL;
    aDataStream.readRawData(reinterpret_cast<char*>(&pIOWidget), sizeof(pIOWidget));

    //check if the dragged module belongs to this host
    ModuleWidget* pWidget = pIOWidget->getModuleWidget();    
    Module* pModule = pWidget->getModule();

    MODULE::HostModule* pHostModule = static_cast<MODULE::HostModule*>(getModule());
    if(pHostModule->getModule(pModule->getRuntimeModuleID()) == pModule)
    {
      pEvent->setDropAction(Qt::MoveAction);
      pEvent->accept();
    }
    else
    {
      pEvent->ignore();
    }
  }
  else if(pEvent->mimeData()->hasFormat(QtIOWidget::getModuleInputDnDFormat()))
  {
    QByteArray aItemData = pEvent->mimeData()->data(QtIOWidget::getModuleInputDnDFormat());
    QDataStream aDataStream(&aItemData, QIODevice::ReadOnly);

    QtIOWidget* pIOWidget = NULL;
    aDataStream.readRawData(reinterpret_cast<char*>(&pIOWidget), sizeof(pIOWidget));

    //check if the dragged module belongs to this host
    ModuleWidget* pWidget = pIOWidget->getModuleWidget();    
    Module* pModule = pWidget->getModule();

    MODULE::HostModule* pHostModule = static_cast<MODULE::HostModule*>(getModule());
    if(pHostModule->getModule(pModule->getRuntimeModuleID()) == pModule)
    {
      pEvent->setDropAction(Qt::MoveAction);
      pEvent->accept();
    }
    else
    {
      pEvent->ignore();
    }
  }
  else
  {
    tBase::dragEnterEvent(pEvent);
  }
}

//---------------------------------------------dragMoveEvent
void HostModuleUI::dragMoveEvent(QDragMoveEvent* pEvent)
{
  if(!pEvent->mimeData()->hasFormat(QtIOWidget::getModuleOutputDnDFormat()) &&
     !pEvent->mimeData()->hasFormat(QtIOWidget::getModuleInputDnDFormat()))
  {
    tBase::dragMoveEvent(pEvent);
  }
}

//---------------------------------------------dropEvent
void HostModuleUI::dropEvent(QDropEvent* pEvent)
{
  if(pEvent->mimeData()->hasFormat(QtIOWidget::getModuleOutputDnDFormat()))
  {
    QByteArray aItemData = pEvent->mimeData()->data(QtIOWidget::getModuleOutputDnDFormat());
    QDataStream aDataStream(&aItemData, QIODevice::ReadOnly);

    QtIOWidget* pIOWidget = NULL;
    aDataStream.readRawData(reinterpret_cast<char*>(&pIOWidget), sizeof(pIOWidget));
    QtModuleOutputWidget* pOutputWidget = static_cast<QtModuleOutputWidget*>(pIOWidget);

    //export the dragged output to the host module
    MODULE::HostModule* pHostModule = static_cast<MODULE::HostModule*>(getModule());
    ModuleOutputBase* pOutput = pOutputWidget->getModuleOutput();
    pHostModule->exportModuleOutput(pOutput);
  }
  else if(pEvent->mimeData()->hasFormat(QtIOWidget::getModuleInputDnDFormat()))
  {
    QByteArray aItemData = pEvent->mimeData()->data(QtIOWidget::getModuleInputDnDFormat());
    QDataStream aDataStream(&aItemData, QIODevice::ReadOnly);

    QtIOWidget* pIOWidget = NULL;
    aDataStream.readRawData(reinterpret_cast<char*>(&pIOWidget), sizeof(pIOWidget));
    QtModuleInputWidget* pInputWidget = static_cast<QtModuleInputWidget*>(pIOWidget);

    //export the dragged output to the host module
    MODULE::HostModule* pHostModule = static_cast<MODULE::HostModule*>(getModule());
    ModuleInputBase* pInput = pInputWidget->getModuleInput();
    pHostModule->exportModuleInput(pInput);
  }
  else
  {
    tBase::dropEvent(pEvent);
  }
}

//---------------------------------------------restoreWidget
bool HostModuleUI::restoreWidget(XERCES::DOMElement* pNode)
{ 
  tString sModuleRuntimeID;
  if(!XercesXML::getAttributeValue(pNode, XML_MODULE_ID_ATTR, sModuleRuntimeID))
  {
    return false;
  }

  ModuleWidget* pWidget = m_pCustomWidget->getModuleWidget(sModuleRuntimeID);
  if(!pWidget)
  {
    return false;
  }
  return pWidget->load(pNode);
}

//---------------------------------------------restoreConnection
bool HostModuleUI::restoreConnection(XERCES::DOMElement* pNode)
{  
  tString sFromModuleID;
  if(!XercesXML::getAttributeValue(pNode, XML_CONNECTION_FROM_ATTR, sFromModuleID))
  {
    return false;
  }

  tString sToModuleID;
  if(!XercesXML::getAttributeValue(pNode, XML_CONNECTION_TO_ATTR, sToModuleID))
  {
    return false;
  }

  ModuleWidget* pFrom = m_pCustomWidget->getModuleWidget(sFromModuleID);
  if(!pFrom)
  {
    return false;
  }

  ModuleWidget* pTo = m_pCustomWidget->getModuleWidget(sToModuleID);
  if(!pTo)
  {
    return false;
  }

  m_pCustomWidget->addWidgetConnection(pFrom, pTo);

  return true;
}

} }
