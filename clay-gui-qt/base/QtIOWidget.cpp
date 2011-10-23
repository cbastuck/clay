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

#include "QtIOWidget.h"

#include <clay-core/base/ModuleIO.h>
#include <clay-gui-qt/base/ModuleWidget.h>

//Qt
#include <QPainter>
#include <QDragMoveEvent>

#define QT_MODULE_INPUT_DRAG_DROP_FORMAT  "mfgui-dragdrop-format/module-input-format"
#define QT_MODULE_OUTPUT_DRAG_DROP_FORMAT "mfgui-dragdrop-format/module-output-format"

namespace CLAY{ namespace UI{

//---------------------------------------------getModuleInputDnDFormat
const char* QtIOWidget::getModuleInputDnDFormat()
{
  return QT_MODULE_INPUT_DRAG_DROP_FORMAT;
}

//---------------------------------------------getModuleOutputDnDFormat
const char* QtIOWidget::getModuleOutputDnDFormat()
{
  return QT_MODULE_OUTPUT_DRAG_DROP_FORMAT;
}

//---------------------------------------------QtIOWidget
QtIOWidget::QtIOWidget(QWidget* pParent, UI::ModuleWidget* pModuleWidget)
  : tBase(pParent),
    m_pModuleWidget(pModuleWidget)
{
  setAttribute(Qt::WA_DeleteOnClose);
  setAcceptDrops(true);
}

//---------------------------------------------init
void QtIOWidget::init(const tString& sId)
{
  m_aUi.setupUi(this);
  //m_aUi.m_pLabel->setText(sId.c_str());
  //m_aUi.m_pLabel->setAlignment(Qt::AlignHCenter);

  setFixedWidth(40);
}

//---------------------------------------------mouseMoveEvent
void QtIOWidget::mouseMoveEvent(QMouseEvent* pEvent)
{
  QByteArray aItemData;
  QDataStream aDataStream(&aItemData, QIODevice::WriteOnly);

  QtIOWidget* pThis = this;
  aDataStream.writeRawData(reinterpret_cast<char*>(&pThis), sizeof(pThis));

  QMimeData* pMimeData = new QMimeData;
  pMimeData->setData(getDragDropFormat(), aItemData);

  QPixmap tempPixmap(width(), height());
  QPainter painter;
  painter.begin(&tempPixmap);
  painter.fillRect(rect(), QColor(127, 127, 127, 127));
  painter.end();

  QDrag* pDrag = new QDrag(this);
  pDrag->setMimeData(pMimeData);
  pDrag->setPixmap(tempPixmap);
  pDrag->setHotSpot(pEvent->pos());

  if (pDrag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction)
  {
    return; 
  }
}

//---------------------------------------------dragEnterEvent
void QtIOWidget::dragEnterEvent(QDragEnterEvent* pEvent)
{
  const char* pAcceptDropFormat = getAcceptDropFormat();
  if(pEvent->mimeData()->hasFormat(pAcceptDropFormat)) 
  {
    QByteArray aItemData = pEvent->mimeData()->data(pAcceptDropFormat);
    QDataStream aDataStream(&aItemData, QIODevice::ReadOnly);

    QtIOWidget* pIOWidget = NULL;
    aDataStream.readRawData(reinterpret_cast<char*>(&pIOWidget), sizeof(pIOWidget));

    if(processDropRequest(pIOWidget))
    {
      pEvent->setDropAction(Qt::MoveAction);
      pEvent->accept();
    }
    else
    {
      pEvent->ignore();
    }
  }
}

//---------------------------------------------dropEvent
void QtIOWidget::dropEvent(QDropEvent* pEvent)
{
  const char* pAcceptDropFormat = getAcceptDropFormat();
  QByteArray aItemData = pEvent->mimeData()->data(pAcceptDropFormat);
  QDataStream aDataStream(&aItemData, QIODevice::ReadOnly);

  QtIOWidget* pIOWidget = NULL;
  aDataStream.readRawData(reinterpret_cast<char*>(&pIOWidget), sizeof(pIOWidget));

  processReceive(pIOWidget);
}

//---------------------------------------------
//---------------------------------------------QtModuleInputWidget
//---------------------------------------------
QtModuleInputWidget::QtModuleInputWidget(QWidget* pParent, ModuleInputBase* pModuleInput, ModuleWidget* pModuleWidget)
  : QtIOWidget(pParent, pModuleWidget), 
    m_pModuleInput(pModuleInput)
{

}

//---------------------------------------------init
void QtModuleInputWidget::init(const tString& sId)
{
  tBase::init(sId);

  unsigned int uTmp = 0; //m_aUi.m_pLabel->y();
  //m_aUi.m_pLabel->move(m_aUi.m_pLabel->x(), m_aUi.m_pConnector->y() + 2);
  m_aUi.m_pConnector->move(m_aUi.m_pConnector->x(), uTmp);
}

//---------------------------------------------mousePressEvent
void QtModuleInputWidget::mousePressEvent(QMouseEvent* )
{

}

//---------------------------------------------getDragDropFormat
const char* QtModuleInputWidget::getDragDropFormat()
{
  return QtIOWidget::getModuleInputDnDFormat();
}

//---------------------------------------------getAcceptDropFormat
const char* QtModuleInputWidget::getAcceptDropFormat()
{
  return QtIOWidget::getModuleOutputDnDFormat();
}

//---------------------------------------------processDropRequest
bool QtModuleInputWidget::processDropRequest(QtIOWidget* pOther)
{
  QtModuleOutputWidget* pOutputWidget = static_cast<QtModuleOutputWidget*>(pOther);
  ModuleOutputBase* pOutput = pOutputWidget->getModuleOutput();
  if(/*!pOutput->isConnected() && */ !m_pModuleInput->isConnected())
  {
    return m_pModuleInput->canConnect(pOutput);
  }
  return false;    
}

//---------------------------------------------processReceive
void QtModuleInputWidget::processReceive(QtIOWidget* pOther)
{
  QtModuleOutputWidget* pOutput = static_cast<QtModuleOutputWidget*>(pOther);
  m_pModuleWidget->connect(m_pModuleInput, pOutput->getModuleOutput());
  //pOutput->getModuleOutput()->connect(m_pModuleInput, false);
}

//---------------------------------------------
//---------------------------------------------QtModuleOutputWidget
//---------------------------------------------
QtModuleOutputWidget::QtModuleOutputWidget(QWidget* pParent, ModuleOutputBase* pModuleOutput, ModuleWidget* pModuleWidget)
  : QtIOWidget(pParent, pModuleWidget),
    m_pModuleOutput(pModuleOutput)
{

}

//---------------------------------------------getDragDropFormat
const char* QtModuleOutputWidget::getDragDropFormat()
{
  return QtIOWidget::getModuleOutputDnDFormat();
}

//---------------------------------------------getAcceptDropFormat
const char* QtModuleOutputWidget::getAcceptDropFormat()
{
  return QtIOWidget::getModuleInputDnDFormat();
}

//---------------------------------------------processDropRequest
bool QtModuleOutputWidget::processDropRequest(QtIOWidget* pOther)
{
  QtModuleInputWidget* pInputWidget = static_cast<QtModuleInputWidget*>(pOther);
  ModuleInputBase* pInput = pInputWidget->getModuleInput();
  if(!pInput->isConnected())
  {
    return m_pModuleOutput->canConnect(pInput);
  }
  return false;
}

//---------------------------------------------processReceive
void QtModuleOutputWidget::processReceive(QtIOWidget* pOther)
{
  QtModuleInputWidget* pInput = static_cast<QtModuleInputWidget*>(pOther);
  m_pModuleWidget->connect(m_pModuleOutput, pInput->getModuleInput());
  //pInput->getModuleInput()->connect(m_pModuleOutput, false);
}

//---------------------------------------------mousePressEvent
void QtModuleOutputWidget::mousePressEvent(QMouseEvent* )
{

}

} }
