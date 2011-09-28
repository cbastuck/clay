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

#ifndef HostModuleUI_h_
#define HostModuleUI_h_

//clay-gui
#include <clay-gui-qt/base/ModuleWidget.h>

//clay-module
#include <clay-modules/host/HostModule.h>

//clay-core
#include <clay-core/helper/XMLHelper.h>

//QT forwards
class QDragEnterEvent;

namespace CLAY{ namespace UI{

class HostModuleUIInternalView;

class HostModuleUI : public ModuleWidget 
{
  Q_OBJECT
private:
  typedef ModuleWidget tBase;

public:
  static ModuleWidget* create(Module*, Host*, void*);
  static void destroy(ModuleWidget*);

  HostModuleUI(Module* pModule, Host* pHost, QWidget* pParent);
  virtual ~HostModuleUI();
  
  virtual void init();
  virtual void deInit();

  virtual bool save(XERCES::DOMElement* pNode) const;
  virtual bool load(XERCES::DOMElement* pNode);

  virtual void onShowContextMenu(QMenu& aMenu);
  virtual void onContextMenu(QAction* pDecision);

  virtual void connectModuleSignals();
  virtual void showModuleUI();
  
  //persistence
  bool loadProject(const tString& sFn);
  bool loadWidgets(XERCES::DOMElement* pElem);

  bool saveProject(const tString& sFn);
  bool saveWidgets(XERCES::DOMElement* pElem) const;

protected:
  virtual void dragEnterEvent(QDragEnterEvent* pEvent);
  virtual void dragMoveEvent(QDragMoveEvent* pEvent);
  virtual void dropEvent(QDropEvent* pEvent);

  bool restoreWidget(XERCES::DOMElement* pNode);
  bool restoreConnection(XERCES::DOMElement* pNode);

private:
  HostModuleUIInternalView* m_pCustomWidget;
};

} }

#endif
