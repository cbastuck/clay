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

#include "QtViewerWorkspace.h"
#include "ModuleWidget.h"

#include <clay-core/base/Module.h>

#include <QVariant>
#include <QPainter>
#include <QColor>

namespace CLAY{ namespace UI{

//---------------------------------------------QtViewerWorkspace
QtViewerWorkspace::QtViewerWorkspace(QWidget* pParent) 
  : tBase(pParent)
{
  setBackgroundRole(QPalette::Base);
}
    
//---------------------------------------------~QtViewerWorkspace
QtViewerWorkspace::~QtViewerWorkspace()
{

}

//---------------------------------------------init
void QtViewerWorkspace::init()
{

}

//---------------------------------------------deInit
void QtViewerWorkspace::deInit()
{

}

//---------------------------------------------addConnection
void QtViewerWorkspace::addConnection(UI::ModuleWidget* pFromUI, UI::ModuleWidget* pToUI)
{
  CLAY_ASSERT(pToUI);
  CLAY_ASSERT(pFromUI);
  m_collConnections.push_back(std::make_pair(pFromUI, pToUI));
  update();
}

//---------------------------------------------removeConnections
void QtViewerWorkspace::removeConnections(UI::ModuleWidget* pWidget)
{
  tConnectionCollection::iterator it  = m_collConnections.begin();
  tConnectionCollection::iterator end = m_collConnections.end();
  for(; it!=end; )
  {
    tConnection& aConnection = *it;
    if(aConnection.first  == pWidget ||
        aConnection.second == pWidget)
    {
      it  = m_collConnections.erase(it);
      end = m_collConnections.end();
    }
    else
    {
      ++it;
    }
  }
  update();
}


//---------------------------------------------autoResize
void QtViewerWorkspace::autoResize()
{  
  int iBorderX = 0;
  int iBorderY = 0;

  const QObjectList& collWidgets  = children();
  QObjectList::const_iterator it  = collWidgets.begin();
  QObjectList::const_iterator end = collWidgets.end();
  for(; it!=end; ++it)
  {
    ModuleWidget* pModuleWidget = static_cast<ModuleWidget*>(*it);

    //border detection
    if(pModuleWidget->geometry().right() > iBorderX)
    {
      iBorderX = pModuleWidget->geometry().right();
    }

    if(pModuleWidget->geometry().bottom() > iBorderY)
    {
      iBorderY = pModuleWidget->geometry().bottom();
    }
  }

  if(iBorderX != width() || iBorderY != height())
  {
    resize(iBorderX + 30, iBorderY + 30); //shrink and grow if needed
  }

  update(); //trigger an update in any case (even if no resize was done)
}

//---------------------------------------------paintEvent
void QtViewerWorkspace::paintEvent(QPaintEvent* pEvent)
{
  //draw connections
  QPainter aPainter(this);
  aPainter.setRenderHint(QPainter::Antialiasing);
      
  QBrush aBrush;
  aBrush.setColor(Qt::white);
  aBrush.setStyle(Qt::SolidPattern);
  aPainter.fillRect(rect(), aBrush);

  QPen aPen;
  aPen.setColor(QColor::fromRgb(100, 100, 100));
  aPen.setWidth(1);
  aPainter.setPen(aPen);
  tConnectionCollection::const_iterator it  = m_collConnections.begin();
  tConnectionCollection::const_iterator end = m_collConnections.end();
  for(; it!=end; ++it)
  {
    UI::ModuleWidget* pFrom = it->first;
    UI::ModuleWidget* pTo   = it->second;

    Module* pFromModule = pFrom->getModule();
    Module* pToModule   = pTo->getModule();

    std::deque<ModuleOutputBase*> collConnections;
    pFromModule->getOutputConnectionsTo(pToModule, collConnections);

    std::deque<ModuleOutputBase*>::const_iterator ti  = collConnections.begin();
    std::deque<ModuleOutputBase*>::const_iterator dne = collConnections.end();
    for(; ti!=dne; ++ti)
    {
      ModuleOutputBase* pCurrent = *ti;
      for(unsigned int i=0, n=pCurrent->getNumConnections(); i<n; ++i)
      {
        ModuleInputBase* pOpponent = pCurrent->getOpponent(i);
        if(pOpponent->getParentModule() == pToModule)
        {
          QWidget* pFromWidget = pFrom->getOutputWidget(pCurrent);
          QWidget* pToWidget   = pTo->getInputWidget(pOpponent);
          CLAY_ASSERT(pFromWidget);
          CLAY_ASSERT(pToWidget);
          
          QPoint aCenterOffset(-6, 0);
          QPoint aFromPos = pFrom->pos() + pFromWidget->mapTo(pFrom, pFromWidget->rect().center()) + aCenterOffset;
          QPoint aToPos   = pTo->pos() + pToWidget->mapTo(pTo, pToWidget->rect().center()) + aCenterOffset;

          int iExtLength = 25; // extension length in px
          QPoint aFromExt = aFromPos + QPoint(0, iExtLength);
          QPoint aToExt = aToPos + QPoint(0, -iExtLength);

          aPainter.drawLine(aFromPos, aFromExt);

          //if the input(aToExt) is about the output (aFromExt) then render an additional line segment
          if(aToExt.y() < aFromExt.y())
          {
            int iHelpLineLen = (aToExt.x() > aFromExt.x()) ? -25 : 25;

            QPoint aToHelpLine(aToExt.x() + iHelpLineLen, aToExt.y());
            QPoint aFromHelpLine(aFromExt.x() - iHelpLineLen, aFromExt.y());                  
            aPainter.drawLine(aToHelpLine, aToExt);
            aPainter.drawLine(aFromHelpLine, aFromExt);
            aPainter.drawLine(aFromHelpLine, aToHelpLine);
          }
          else
          {
            aPainter.drawLine(aFromExt, aToExt);
          }

          aPainter.drawLine(aToExt, aToPos);
        }
      }
    }
  }
}

} }
