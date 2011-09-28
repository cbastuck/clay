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

#ifndef QtIOWidget_H_
#define QtIOWidget_H_

#include <clay-core/base/ClayDefines.h>

#include "ui_ModuleWidgetUIConnector.h"

//#include <QLabel>
#include <QWidget>

namespace CLAY{
class ModuleInputBase;
class ModuleOutputBase;
}

namespace CLAY{ namespace UI{
  
class ModuleWidget;

//---------------------------------------------QtIOWidget
class QtIOWidget : public QWidget
{
private:
  typedef QWidget tBase;

public:
  static const char* getModuleInputDnDFormat();
  static const char* getModuleOutputDnDFormat();

  QtIOWidget(QWidget* pParent, UI::ModuleWidget* pModuleWidget);

  virtual const char* getDragDropFormat()             = 0;
  virtual const char* getAcceptDropFormat()           = 0;
  virtual bool        processDropRequest(QtIOWidget*) = 0;
  virtual void        processReceive(QtIOWidget*)     = 0;

  virtual void init(const tString& sId);

  ModuleWidget* getModuleWidget(){ return m_pModuleWidget; }

protected:
  virtual void mouseMoveEvent(QMouseEvent* pEvent);
  virtual void dragEnterEvent(QDragEnterEvent* pEvent);
  virtual void dropEvent(QDropEvent* pEvent);

  ModuleWidget* m_pModuleWidget;

  Ui::ModuleConnector m_aUi;
};

//---------------------------------------------QtModuleInputWidget
class QtModuleInputWidget : public QtIOWidget
{
private:
  typedef QtIOWidget tBase;

public:
  QtModuleInputWidget(QWidget* pParent, ModuleInputBase* pModuleInput, ModuleWidget* pModuleWidget);

  virtual const char* getDragDropFormat();
  virtual const char* getAcceptDropFormat();
  virtual bool        processDropRequest(QtIOWidget*);
  virtual void        processReceive(QtIOWidget*);

  virtual void init(const tString& sId);

  //------ inline stuff
  ModuleInputBase* getModuleInput(){ return m_pModuleInput; }

protected:
  virtual void mousePressEvent(QMouseEvent* pEvent);

protected:
  ModuleInputBase* m_pModuleInput;
};

//---------------------------------------------QtModuleOutputWidget
class QtModuleOutputWidget : public QtIOWidget
{
private:
  typedef QtIOWidget tBase;

public:
  QtModuleOutputWidget(QWidget* pParent, ModuleOutputBase* pModuleOutput, ModuleWidget* pModuleWidget);

  virtual const char* getDragDropFormat();
  virtual const char* getAcceptDropFormat();
  virtual bool        processDropRequest(QtIOWidget*);
  virtual void        processReceive(QtIOWidget*);

  //------- inline stuff
  ModuleOutputBase* getModuleOutput(){ return m_pModuleOutput; }

protected:
  virtual void mousePressEvent(QMouseEvent* pEvent);

protected:
  ModuleOutputBase* m_pModuleOutput;
};

} }

#endif
