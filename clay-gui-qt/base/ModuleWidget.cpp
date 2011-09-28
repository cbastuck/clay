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

#include "ModuleWidget.h"

#include "ui_ModuleWidgetUI.h"

//CLAY
#include <clay-core/host/Host.h>
#include <clay-core/base/Module.h>
#include <clay-gui-qt/base/QtIOWidget.h>
#include <clay-core/helper/LexicalConversion.h>

//QT
#include <QTimer>
#include <QBitmap>
#include <QPainter>
#include <QHBoxLayout>
#include <QDragMoveEvent>

//STL
#include <iostream>

//boost
#include <boost/bind.hpp>

#define XML_WIDGET_NODE       "widget"
#define XML_WIDGET_POS_X_ATTR "pos-x"
#define XML_WIDGET_POS_Y_ATTR "pos-y"

#define CONNECTOR_IMAGE_WIDTH  22

//define the time unit - timer interval in milliseconds
#define TIMER_UNIT 100

namespace CLAY{ namespace UI{

//---------------------------------------------statics
ModuleWidget::tMapping ModuleWidget::m_aInputMapping;
ModuleWidget::tMapping ModuleWidget::m_aOutputMapping;

//---------------------------------------------ModuleWidget
ModuleWidget::ModuleWidget(Module* pModule, Host* pHost, QWidget* pParent)
  : tBase(pParent),
    m_pHost(pHost),
    m_pModule(pModule),
    m_bLeftButtonPressed(false),
    m_iStateActiveCounter(-1),
    m_aTimer(this),
    m_pUi(NULL)
{
  if(m_aInputMapping.size() == 0)
  {
    m_aInputMapping[0]  = 0;
    m_aInputMapping[10] = 2;
    m_aInputMapping[20] = 4;
    m_aInputMapping[30] = 10;
    m_aInputMapping[40] = 21;
    m_aInputMapping[48] = 36;
  }

  if(m_aOutputMapping.size() == 0)
  {
    m_aOutputMapping[0]  = 68;
    m_aOutputMapping[10] = 66;
    m_aOutputMapping[20] = 64;
    m_aOutputMapping[26] = 61;
    m_aOutputMapping[31] = 54;
    m_aOutputMapping[48] = 50;
  }
}

//---------------------------------------------~ModuleWidget
ModuleWidget::~ModuleWidget()
{
  CLAY_ASSERT(!m_pUi);
}

//---------------------------------------------init
void ModuleWidget::init()
{
  m_pUi = new Ui::ModuleWidgetUI();
  m_pUi->setupUi(this);
  m_pUi->m_pModuleName->setText(QString(m_pModule->getRuntimeModuleID().c_str()));

  setAcceptDrops(true);

  m_aTimer.start(TIMER_UNIT); //interval the timer is invoked

  //visualize the state of the module
  if(m_pModule->isActive())
  {
    onModuleInitStateChanged(true);
  }
  else if(m_pModule->isInitialized())
  {
    onModuleInitStateChanged(true);
  }

  connectModuleSignals(); //call virtual method

  //QObject::connect(m_pUi->m_pRunButton, SIGNAL(pressed()), this, SLOT(onRunButton()));
  //QObject::connect(m_pUi->m_pResetButton, SIGNAL(pressed()), this, SLOT(onResetButton()));
  QObject::connect(&m_aTimer, SIGNAL(timeout()), this, SLOT(onTimer()));

  initInputs();
  initOutputs();

  setContextMenuPolicy(Qt::CustomContextMenu);

  static QBitmap aMask;
  if(aMask.width() == 0) //create the mask only once
  {
    aMask = QBitmap(size());
    aMask.clear();
    QPainter aPainter(&aMask);
    aPainter.setBrush(QBrush(Qt::SolidPattern));
    aPainter.drawEllipse(QPoint(width()/2, height()/2), width()/2, width()/2);
  }
  setMask(aMask);
}

//---------------------------------------------deInit
void ModuleWidget::deInit()
{
  disconnectModuleSignals();

  deInitInputs();
  deInitOutputs();

  if(m_pUi)
  {
    delete m_pUi;
    m_pUi = NULL;
  }
}

//---------------------------------------------initInputs
void ModuleWidget::initInputs()
{
  Module* pModule = getModule();
  Module::tInputIterator it  = pModule->beginInput();
  Module::tInputIterator end = pModule->endInput();
  for(; it!=end; ++it)
  {
    ModuleInputBase* pInput = *it;
    createModuleInputWidget(pInput);
  }
}

//---------------------------------------------initOutputs
void ModuleWidget::initOutputs()
{
  Module* pModule = getModule();
  Module::tOutputIterator it  = pModule->beginOutput();
  Module::tOutputIterator end = pModule->endOutput();
  for(; it!=end; ++it)
  {
    ModuleOutputBase* pOutput = *it;
    createModuleOutputWidget(pOutput);
  }
}

//---------------------------------------------deInitInputs
void ModuleWidget::deInitInputs()
{
  tInputsCollection::iterator it  = m_collInputWidgets.begin();
  tInputsCollection::iterator end = m_collInputWidgets.end();
  for(; it!=end; ++it)
  {
    QWidget* pWidget = it->second;
    pWidget->close();
    //m_pUi->m_pInputsLayout->removeWidget(pWidget);
    //delete pWidget;
  }
  m_collInputWidgets.clear();
}

//---------------------------------------------deInitOutputs
void ModuleWidget::deInitOutputs()
{
  tOutputsCollection::iterator it  = m_collOutputWidgets.begin();
  tOutputsCollection::iterator end = m_collOutputWidgets.end();
  for(; it!=end; ++it)
  {
    QWidget* pWidget = it->second;
    pWidget->close();
    //m_pUi->m_pOutputsLayout->removeWidget(pWidget);
    //delete pWidget;
  }
  m_collOutputWidgets.clear();
}

//---------------------------------------------createModuleInputWidget
void ModuleWidget::createModuleInputWidget(ModuleInputBase* pInput)
{
  CLAY_ASSERT(pInput);
  CLAY_ASSERT(m_collInputWidgets.find(pInput) == m_collInputWidgets.end()); //do not register it twice

  QtModuleInputWidget* pWidget = new QtModuleInputWidget(this, pInput, this);
  pWidget->init(pInput->getName());

  m_collInputWidgets[pInput] = pWidget;

  alignInputWidgets();

  pWidget->show();
}

//---------------------------------------------createModuleOutputWidget
void ModuleWidget::createModuleOutputWidget(ModuleOutputBase* pOutput)
{
  CLAY_ASSERT(pOutput);
  CLAY_ASSERT(m_collOutputWidgets.find(pOutput) == m_collOutputWidgets.end()); //do not register it twice

  QtModuleOutputWidget* pWidget = new QtModuleOutputWidget(this, pOutput, this);
  pWidget->init(pOutput->getName());

  m_collOutputWidgets[pOutput] = pWidget;

  alignOutputWidgets();
  
  pWidget->show();
}

//---------------------------------------------getUIDescriptor
ModuleUIDescriptor* ModuleWidget::getUIDescriptor()
{
  return NULL;
}

//---------------------------------------------connectModuleSignals
void ModuleWidget::connectModuleSignals()
{
  /*
  m_pModule->signalModuleActive.addSlot(&ModuleWidget::onModuleActiveStateChanged, this, true);
  m_pModule->signalModuleInactive.addSlot(&ModuleWidget::onModuleActiveStateChanged, this, false);
  m_pModule->signalModuleInitialized.addSlot(&ModuleWidget::onModuleInitStateChanged, this, true);
  m_pModule->signalModuleUnitialized.addSlot(&ModuleWidget::onModuleInitStateChanged, this, false);

  m_pModule->signalModuleInputRegistered.addSlot(&ModuleWidget::onModuleInputRegistered, this);
  m_pModule->signalModuleInputUnregistered.addSlot(&ModuleWidget::onModuleInputUnregistered, this);
  m_pModule->signalModuleInputConnected.addSlot(&ModuleWidget::onModuleInputConnected, this);
  m_pModule->signalModuleInputDisconnected.addSlot(&ModuleWidget::onModuleInputDisconnected, this);

  m_pModule->signalModuleOutputRegistered.addSlot(&ModuleWidget::onModuleOutputRegistered, this);
  m_pModule->signalModuleOutputUnregistered.addSlot(&ModuleWidget::onModuleOutputUnregistered, this);
  m_pModule->signalModuleOutputConnected.addSlot(&ModuleWidget::onModuleOutputConnected, this);
  m_pModule->signalModuleOutputDisconnected.addSlot(&ModuleWidget::onModuleOutputDisconnected, this);
  */

  addConnection(&m_pModule->signalModuleActive, &ModuleWidget::onModuleActiveStateChanged, true);
  addConnection(&m_pModule->signalModuleInactive, &ModuleWidget::onModuleActiveStateChanged, false);
  addConnection(&m_pModule->signalModuleInitialized, &ModuleWidget::onModuleInitStateChanged, true);
  addConnection(&m_pModule->signalModuleUnitialized, &ModuleWidget::onModuleInitStateChanged, false);

  addConnection(&m_pModule->signalModuleInputRegistered, &ModuleWidget::onModuleInputRegistered);
  addConnection(&m_pModule->signalModuleInputUnregistered, &ModuleWidget::onModuleInputUnregistered);
  addConnection(&m_pModule->signalModuleInputConnected, &ModuleWidget::onModuleInputConnected);
  addConnection(&m_pModule->signalModuleInputDisconnected, &ModuleWidget::onModuleInputDisconnected);

  addConnection(&m_pModule->signalModuleOutputRegistered, &ModuleWidget::onModuleOutputRegistered);
  addConnection(&m_pModule->signalModuleOutputUnregistered, &ModuleWidget::onModuleOutputUnregistered);
  addConnection(&m_pModule->signalModuleOutputConnected, &ModuleWidget::onModuleOutputConnected);
  addConnection(&m_pModule->signalModuleOutputDisconnected, &ModuleWidget::onModuleOutputDisconnected);
}

//---------------------------------------------disconnectModuleSignals
void ModuleWidget::disconnectModuleSignals()
{
  removeConnections(&m_pModule->signalModuleActive);
  removeConnections(&m_pModule->signalModuleInactive);
  removeConnections(&m_pModule->signalModuleInitialized);
  removeConnections(&m_pModule->signalModuleUnitialized);

  removeConnections(&m_pModule->signalModuleInputRegistered);
  removeConnections(&m_pModule->signalModuleInputUnregistered);
  removeConnections(&m_pModule->signalModuleInputConnected);
  removeConnections(&m_pModule->signalModuleInputDisconnected);

  removeConnections(&m_pModule->signalModuleOutputRegistered);
  removeConnections(&m_pModule->signalModuleOutputUnregistered);
  removeConnections(&m_pModule->signalModuleOutputConnected);
  removeConnections(&m_pModule->signalModuleOutputDisconnected);
}

//---------------------------------------------showModuleUI
void ModuleWidget::showModuleUI()
{
  //do this in subclass
}

//---------------------------------------------save
bool ModuleWidget::save(XERCES::DOMElement* pParent) const
{
  XERCES::DOMElement* pRoot = XercesXML::appendNode(pParent, XML_WIDGET_NODE);

  XercesXML::addAttribute(pRoot, XML_WIDGET_POS_X_ATTR, this->x());
  XercesXML::addAttribute(pRoot, XML_WIDGET_POS_Y_ATTR, this->y());

  return true;
}

//---------------------------------------------load
bool ModuleWidget::load(XERCES::DOMElement* pParent)
{
  XERCES::DOMElement* pRoot = XercesXML::findChildNode(pParent, XML_WIDGET_NODE);
  if(!pRoot)
  {
    return false;
  }

  unsigned int uPosX = 0;
  if(!XercesXML::getAttributeValue(pRoot, XML_WIDGET_POS_X_ATTR, uPosX))
  {
    return false;
  }

  unsigned int uPosY = 0;
  if(!XercesXML::getAttributeValue(pRoot, XML_WIDGET_POS_Y_ATTR, uPosY))
  {
    return false;
  }

  this->move(uPosX, uPosY);

  return true;
}

//---------------------------------------------onShowContextMenu
void ModuleWidget::onShowContextMenu(QMenu&)
{
  //override for custom menu entries
}

//---------------------------------------------onContextMenu
void ModuleWidget::onContextMenu(QAction*)
{
  //override for handling custom actions
}

//---------------------------------------------onTimer
void ModuleWidget::onTimer()
{
  //do timer stuff
  if(m_iStateActiveCounter >= 0)
  {
    if(m_iStateActiveCounter == 0)
    {
      onModuleActiveStateChanged(true);
      m_iStateActiveCounter = -1; //reset
      onModuleInitStateChanged(true); //not active anymore, but initialized
    }
    else
    {
      --m_iStateActiveCounter;
    }
  }
  signalOnTimerEvent.emit0();
}

//---------------------------------------------onModuleActiveStateChanged
void ModuleWidget::onModuleActiveStateChanged(bool bActive)
{
  if(bActive && !widgetInActiveState()) //otherwise active state is still 'on'
  {
    //m_pUi->m_pStateIndicatorLabel->setText("A");
    m_iStateActiveCounter = 1; //keep the signal for given time units
  }
}

//---------------------------------------------onModuleInitStateChanged
void ModuleWidget::onModuleInitStateChanged(bool bInitialized)
{
  if(bInitialized)
  {
    //m_pUi->m_pStateIndicatorLabel->setText("I");
  }
  else
  {
    //m_pUi->m_pStateIndicatorLabel->setText("X");
  }
}

//---------------------------------------------onModuleInputRegistered
void ModuleWidget::onModuleInputRegistered(ModuleInputBase* pInput)
{
  //deInitInputs();
  //initInputs();
  createModuleInputWidget(pInput);
}

//---------------------------------------------onModuleInputUnregistered
void ModuleWidget::onModuleInputUnregistered(ModuleInputBase* pInput)
{
  deInitInputs();
  initInputs();
}

//---------------------------------------------onModuleInputConnected
void ModuleWidget::onModuleInputConnected(ModuleInputBase* pInput)
{

}

//---------------------------------------------onModuleInputDisconnected
void ModuleWidget::onModuleInputDisconnected(ModuleInputBase* pInput)
{

}

//---------------------------------------------onModuleOutputRegistered
void ModuleWidget::onModuleOutputRegistered(ModuleOutputBase* pOutput)
{
  //deInitOutputs();
  //initOutputs();
  createModuleOutputWidget(pOutput);
}

//---------------------------------------------onModuleOutputUnregistered
void ModuleWidget::onModuleOutputUnregistered(ModuleOutputBase* pOutput)
{
  deInitOutputs();
  initOutputs();
}

//---------------------------------------------onModuleOutputConnected
void ModuleWidget::onModuleOutputConnected(ModuleOutputBase* pOutput)
{

}

//---------------------------------------------onModuleOutputDisconnected
void ModuleWidget::onModuleOutputDisconnected(ModuleOutputBase* pOutput)
{

}

//---------------------------------------------getOutputWidget
QWidget* ModuleWidget::getOutputWidget(ModuleOutputBase* pOutput)
{
  tOutputsCollection::iterator pos = m_collOutputWidgets.find(pOutput);
  if(pos != m_collOutputWidgets.end())
  {
    return pos->second;
  }
  return NULL;
}

//---------------------------------------------getInputWidget
QWidget* ModuleWidget::getInputWidget(ModuleInputBase* pInput)
{
  tInputsCollection::iterator pos = m_collInputWidgets.find(pInput);
  if(pos != m_collInputWidgets.end())
  {
    return pos->second;
  }
  return NULL;
}

//---------------------------------------------connect
bool ModuleWidget::connect(ModuleInputBase*  pInput,  ModuleOutputBase* pOutput)
{
  Module* pOther = pOutput->getParentModule();
  return m_pHost->connect(pOther, pOutput, m_pModule, pInput);
}

//---------------------------------------------connect
bool ModuleWidget::connect(ModuleOutputBase* pOutput, ModuleInputBase*  pInput)
{
  Module* pOther = pInput->getParentModule();
  return m_pHost->connect(m_pModule, pOutput, pOther, pInput);
}

//---------------------------------------------onRunButton
void ModuleWidget::onRunButton()
{
  CLAY_ASSERT(m_pModule);
  Module::ProcessResultCode eResult = m_pModule->process();
  CLAY_ASSERT(eResult == Module::Process_OK);
}

//---------------------------------------------onResetButton
void ModuleWidget::onResetButton()
{
  CLAY_ASSERT(m_pModule);
  m_pModule->deInit();
  m_pModule->init(NULL);
}

//---------------------------------------------paintEvent
void ModuleWidget::paintEvent(QPaintEvent* pEvent)
{
  tBase::paintEvent(pEvent);
}

//---------------------------------------------dragEnterEvent
void ModuleWidget::dragEnterEvent(QDragEnterEvent* pEvent)
{
  /*
  if(pEvent->mimeData()->hasFormat(MODULE_INPUT_DRAG_DROP_FORMAT))
  {
    processModuleInputRequest(pEvent);
  }
  else if(pEvent->mimeData()->hasFormat(MODULE_OUTPUT_DRAG_DROP_FORMAT))
  {
    processModuleOutputRequest(pEvent);
  }
  else
  {
    pEvent->ignore();
  }
  */
}

//---------------------------------------------dragMoveEvent
void ModuleWidget::dragMoveEvent(QDragMoveEvent* pEvent)
{
  if (pEvent->mimeData()->hasFormat(MODULE_INPUT_DRAG_DROP_FORMAT))
  {
    processModuleInputRequest(pEvent);
  }
  else if(pEvent->mimeData()->hasFormat(MODULE_OUTPUT_DRAG_DROP_FORMAT))
  {
    processModuleOutputRequest(pEvent);
  }
  else
  {
    pEvent->ignore();
  }
}

//---------------------------------------------mousePressEvent
void ModuleWidget::mousePressEvent(QMouseEvent* pEvent)
{
  if(pEvent->button() == Qt::LeftButton)
  {
    emit selected(this);
  }
  else if(pEvent->button() == Qt::RightButton)
  {
    emit showContextMenu(this, pEvent);
  }
}

//---------------------------------------------mouseMoveEvent
void ModuleWidget::mouseMoveEvent(QMouseEvent* pEvent)
{
/*
  QWidget* pPressedWidget = childAt(pEvent->pos());
  if(pPressedWidget == m_pUi->m_pInputFrame)
  {
    startModuleInputDrag(pEvent);
  }
  else if (pPressedWidget == m_pUi->m_pOutputFrame)
  {
    startModuleOutputDrag(pEvent);
  }
  else
*/
  {
    startModuleDrag(pEvent);
  }
  
}

//---------------------------------------------dropEvent
void ModuleWidget::dropEvent(QDropEvent* pEvent)
{
  if (pEvent->mimeData()->hasFormat(MODULE_INPUT_DRAG_DROP_FORMAT))
  {
    processModuleInputReceive(pEvent);
  }
  else if(pEvent->mimeData()->hasFormat(MODULE_OUTPUT_DRAG_DROP_FORMAT))
  {
    processModuleOutputReceive(pEvent);
  }
  else
  {
    pEvent->ignore();
  }
}

//---------------------------------------------mouseDoubleClickEvent
void ModuleWidget::mouseDoubleClickEvent(QMouseEvent* pEvent)
{
  showModuleUI();
}

//---------------------------------------------startModuleDrag
void ModuleWidget::startModuleDrag(QMouseEvent* pEvent)
{
  QByteArray aItemData;
  QDataStream aDataStream(&aItemData, QIODevice::WriteOnly);
  aDataStream << QString(getModule()->getRuntimeModuleID().c_str());
  aDataStream << pEvent->pos();

  QMimeData* pMimeData = new QMimeData;
  pMimeData->setData(MODULE_DRAG_DROP_FORMAT, aItemData);

  unsigned int uWidgetHeight = height();
  unsigned int uWidgetWidth  = width();
  QPixmap aDragImage(uWidgetWidth, uWidgetHeight);

  QPainter painter;
  painter.begin(&aDragImage);
  render(&painter);
  painter.end();   

  QBitmap aAlphaMap(uWidgetWidth, uWidgetHeight);
  aAlphaMap.clear();
  painter.begin(&aAlphaMap);

  painter.setBrush(QBrush(QColor(100, 100, 100), Qt::Dense4Pattern));
  painter.drawEllipse(aAlphaMap.rect().center(), uWidgetWidth/2, uWidgetHeight/2);
  painter.end();

  aDragImage.setMask(aAlphaMap);

  QDrag* pDrag = new QDrag(this);
  pDrag->setMimeData(pMimeData);
  pDrag->setPixmap(aDragImage);
  pDrag->setHotSpot(pEvent->pos());

  if (pDrag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction)
  {
    //TODO: maybe you want to do something here
  }

}

//---------------------------------------------startInputDrag
void ModuleWidget::startModuleInputDrag(QMouseEvent* pEvent)
{
  QByteArray aItemData;
  QDataStream aDataStream(&aItemData, QIODevice::WriteOnly);
  aDataStream.writeRawData(reinterpret_cast<char*>(&m_pModule), sizeof(m_pModule));

  QMimeData* pMimeData = new QMimeData;
  pMimeData->setData(MODULE_INPUT_DRAG_DROP_FORMAT, aItemData);

  QDrag aDrag(this);
  aDrag.setMimeData(pMimeData);
  aDrag.setHotSpot(pEvent->pos());

  if (aDrag.exec(Qt::MoveAction) == Qt::MoveAction)
  {
    return;
  }
}

//---------------------------------------------startOutputDrag
void ModuleWidget::startModuleOutputDrag(QMouseEvent* pEvent)
{
  QByteArray aItemData;
  QDataStream aDataStream(&aItemData, QIODevice::WriteOnly);
  aDataStream.writeRawData(reinterpret_cast<char*>(&m_pModule), sizeof(m_pModule));

  QMimeData* pMimeData = new QMimeData;
  pMimeData->setData(MODULE_OUTPUT_DRAG_DROP_FORMAT, aItemData);

  QDrag aDrag(this);
  aDrag.setMimeData(pMimeData);
  aDrag.setHotSpot(pEvent->pos());

  if (aDrag.exec(Qt::MoveAction) == Qt::MoveAction)
  {
    return;
  }
}

//---------------------------------------------processModuleInputRequest
void ModuleWidget::processModuleInputRequest(QDragMoveEvent* pEvent)
{
  QByteArray aItemData = pEvent->mimeData()->data(MODULE_INPUT_DRAG_DROP_FORMAT);
  QDataStream aDataStream(&aItemData, QIODevice::ReadOnly);

  Module* pModuleAtOutput = NULL;
  aDataStream.readRawData(reinterpret_cast<char*>(&pModuleAtOutput), sizeof(pModuleAtOutput));

  if(pModuleAtOutput == m_pModule)
    return;

  if(pModuleAtOutput->canConnectFrom(m_pModule))
  {
    pEvent->setDropAction(Qt::MoveAction);
    pEvent->accept();
  }
  else
  {
    pEvent->ignore();
  }
}

//---------------------------------------------processModuleOutputRequest
void ModuleWidget::processModuleOutputRequest(QDragMoveEvent* pEvent)
{
  CLAY_FAIL(); //not implemented yet
}


//---------------------------------------------processModuleInputReceive
void ModuleWidget::processModuleInputReceive(QDropEvent* pEvent)
{
  QByteArray aItemData = pEvent->mimeData()->data(MODULE_INPUT_DRAG_DROP_FORMAT);
  QDataStream aDataStream(&aItemData, QIODevice::ReadOnly);

  Module* pModuleAtOutput = NULL;
  aDataStream.readRawData(reinterpret_cast<char*>(&pModuleAtOutput), sizeof(pModuleAtOutput));

  if(pModuleAtOutput != m_pModule)
  {
    CLAY_ASSERT(m_pHost);
    m_pHost->connect(m_pModule, pModuleAtOutput);
  }
}

//---------------------------------------------processModuleOutputReceive
void ModuleWidget::processModuleOutputReceive(QDropEvent* pEvent)
{
  CLAY_FAIL(); //not implemented yet
}

//---------------------------------------------widgetInActiveState
bool ModuleWidget::widgetInActiveState()
{
  return m_iStateActiveCounter > 0;
}

//---------------------------------------------alignInputWidgets
void ModuleWidget::alignInputWidgets()
{
  unsigned int uWidth = width();

  unsigned int uNumItems = m_collInputWidgets.size();
  float fItemSpace = (float)uWidth / uNumItems;
  float fAlignmentOffset = fItemSpace / 2.f + CONNECTOR_IMAGE_WIDTH / 2.f;

  tInputsCollection::iterator it = m_collInputWidgets.begin();
  tInputsCollection::iterator ed = m_collInputWidgets.end();
  for(unsigned int i=1; it!=ed; ++it, ++i)
  {
    QWidget* pWidget = it->second;
    unsigned int uX = static_cast<unsigned int>(i * fItemSpace - fAlignmentOffset);
    unsigned int uY = getY(m_aInputMapping, abs((int)uX - (int)(uWidth/2) + CONNECTOR_IMAGE_WIDTH / 2.f));

    pWidget->move(uX, uY);
  }
}

//---------------------------------------------alignOutputWidgets
void ModuleWidget::alignOutputWidgets()
{
  unsigned int uWidth = width();

  unsigned int uNumItems = m_collOutputWidgets.size();
  float fItemSpace = (float)uWidth / uNumItems;
  float fAlignmentOffset = fItemSpace / 2.f + CONNECTOR_IMAGE_WIDTH / 2.f;

  tOutputsCollection::iterator it = m_collOutputWidgets.begin();
  tOutputsCollection::iterator ed = m_collOutputWidgets.end();
  for(unsigned int i=1; it!=ed; ++it, ++i)
  {
    QWidget* pWidget = it->second;
    unsigned int uX = static_cast<unsigned int>(i * fItemSpace - fAlignmentOffset);
    unsigned int uY = getY(m_aOutputMapping, abs((int)uX - (int)(uWidth/2) + CONNECTOR_IMAGE_WIDTH / 2.f));

    pWidget->move(uX, uY);
  }
}

//---------------------------------------------getY
unsigned int ModuleWidget::getY(const tMapping& aMapping, unsigned int uX) const
{
  tMapping::const_iterator pos = aMapping.lower_bound(uX);
  CLAY_ASSERT(pos != aMapping.end());
  if(pos != aMapping.begin())
  {
    tMapping::const_iterator prev = pos;
    --prev;
    float fAlpha = (uX - prev->first) / (float)(pos->first - prev->first);
    return (1.f-fAlpha) * prev->second + fAlpha * pos->second;
  }
  return pos->second;
}
 
} }
