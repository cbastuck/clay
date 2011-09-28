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

#ifndef QtViewerWorkspace_H_
#define QtViewerWorkspace_H_

//QT
#include <QWidget>

//STL
#include <deque>

namespace CLAY{ namespace UI{

class ModuleWidget;

class QtViewerWorkspace : public QWidget
{
private:    
  typedef QWidget                                 tBase;
  typedef std::pair<ModuleWidget*, ModuleWidget*> tConnection;
  typedef std::deque<tConnection>                 tConnectionCollection;

public:
  typedef tConnectionCollection::iterator         tIterator;
  typedef tConnectionCollection::const_iterator   tConstIterator;

  QtViewerWorkspace(QWidget* pParent);
  virtual ~QtViewerWorkspace();
      
  void init();
  void deInit();

  void addConnection(ModuleWidget* pFrom, ModuleWidget* pTo);
  void removeConnections(ModuleWidget* pWidget);

  void autoResize();

  unsigned int getNumConnections() const; //inline 
  tIterator beginConnections(); //inline 
  tIterator endConnections();   //inline 

  tConstIterator beginConnections() const; //inline 
  tConstIterator endConnections()   const; //inline 

protected:
  virtual void paintEvent(QPaintEvent* pEvent);

private:
  tConnectionCollection m_collConnections;
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------getNumConnections
inline unsigned int QtViewerWorkspace::getNumConnections() const
{
  return m_collConnections.size();
}

//---------------------------------------------beginConnections
inline QtViewerWorkspace::tIterator QtViewerWorkspace::beginConnections()
{ 
  return m_collConnections.begin(); 
}

//---------------------------------------------endConnections
inline QtViewerWorkspace::tIterator QtViewerWorkspace::endConnections()  
{ 
  return m_collConnections.end(); 
}

//---------------------------------------------beginConnections
inline QtViewerWorkspace::tConstIterator QtViewerWorkspace::beginConnections() const
{ 
  return m_collConnections.begin(); 
}

//---------------------------------------------endConnections
inline QtViewerWorkspace::tConstIterator QtViewerWorkspace::endConnections() const
{ 
  return m_collConnections.end(); 
}

} }

#endif 
