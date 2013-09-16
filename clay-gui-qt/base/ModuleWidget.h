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

#ifndef MODULEWIDGET_H_
#define MODULEWIDGET_H_

//CLAY includes
#include <clay-core/base/ClayDefines.h>

#include <clay-core/event/Signal.h>
#include <clay-core/helper/XMLHelper.h>
#include <clay-core/event/SignalReceiver.h>

//Qt includes
#include <QWidget>
#include <QTimer>

//boost includes
#include <boost/function.hpp>

//STL includes
#include <map>

#define MODULE_DRAG_DROP_FORMAT        "mfgui-dragdrop-format/module-format"
#define MODULE_INPUT_DRAG_DROP_FORMAT  "mfgui-dragdrop-format/moduleinput-format"
#define MODULE_OUTPUT_DRAG_DROP_FORMAT "mfgui-dragdrop-format/moduleoutput-format"

namespace Ui{ class ModuleWidgetUI; }

namespace CLAY{
class Host;
class Module;
class ModuleInputBase;
class ModuleOutputBase;
}

//QT forwards
class QMenu;
class QAction;

namespace CLAY{ namespace UI{

class ModuleUIDescriptor;

class ModuleWidget : public QWidget,
                     public SignalReceiver
{
  Q_OBJECT
public:
  typedef QWidget tBase;

  ModuleWidget(Module* pModule, Host* pHost, QWidget* pParent);
  virtual ~ModuleWidget();

  virtual void init();
  virtual void deInit();

  virtual void initInputs();
  virtual void deInitInputs();

  virtual void initOutputs();
  virtual void deInitOutputs();

  virtual void createModuleInputWidget(ModuleInputBase* pInput);
  virtual void createModuleOutputWidget(ModuleOutputBase* pOutput);

  virtual ModuleUIDescriptor* getUIDescriptor();

  virtual void connectModuleSignals();
  virtual void disconnectModuleSignals();

  virtual void showModuleUI();

  virtual bool save(XERCES::DOMElement* pNode) const;
  virtual bool load(XERCES::DOMElement* pNode);

  virtual void onShowContextMenu(QMenu& aMenu);
  virtual void onContextMenu(QAction* pDecision);

  //module access
  Module*       getModule();       //inline 
  const Module* getModule() const; //inline 

  //callbacks
  void onModuleInputRegistered  (ModuleInputBase* pInput);
  void onModuleInputUnregistered(ModuleInputBase* pInput);
  void onModuleInputConnected   (ModuleInputBase* pInput);
  void onModuleInputDisconnected(ModuleInputBase* pInput);

  void onModuleOutputRegistered  (ModuleOutputBase* pOutput);
  void onModuleOutputUnregistered(ModuleOutputBase* pOutput);
  void onModuleOutputConnected   (ModuleOutputBase* pOutput);
  void onModuleOutputDisconnected(ModuleOutputBase* pOutput);

  QWidget* getOutputWidget(ModuleOutputBase* pOutput);
  QWidget* getInputWidget (ModuleInputBase* pInput);

  bool connect(ModuleInputBase*  pInput,  ModuleOutputBase* pOutput);
  bool connect(ModuleOutputBase* pOutput, ModuleInputBase*  pInput);

  //----- signals that are emitted by this class
  Signal<boost::function0<void> > signalOnTimerEvent;

public slots:
  void onRunButton();
  void onResetButton();
  virtual void onTimer();

signals:
  void selected(UI::ModuleWidget*);
  void showContextMenu(UI::ModuleWidget*, QMouseEvent*);

protected:
  typedef std::map<unsigned int, unsigned int> tMapping;

  virtual void paintEvent(QPaintEvent* pEvent);
  virtual void dragEnterEvent(QDragEnterEvent* pEvent);
  virtual void dragMoveEvent(QDragMoveEvent* pEvent);
  virtual void mousePressEvent(QMouseEvent* pEvent);
  virtual void mouseMoveEvent(QMouseEvent* pEvent);
  virtual void dropEvent(QDropEvent* pEvent);
  virtual void mouseDoubleClickEvent(QMouseEvent* pEvent);

  void startModuleDrag(QMouseEvent* pEvent);
  void startModuleInputDrag(QMouseEvent* pEvent);
  void startModuleOutputDrag(QMouseEvent* pEvent);

  void processModuleInputRequest(QDragMoveEvent* pEvent);
  void processModuleOutputRequest(QDragMoveEvent* pEvent);

  void processModuleInputReceive(QDropEvent* pEvent);
  void processModuleOutputReceive(QDropEvent* pEvent);
  
  void alignInputWidgets();
  void alignOutputWidgets();

  unsigned int getY(const tMapping& aMapping, unsigned int uX) const;

private:
  Module* m_pModule;
  bool m_bLeftButtonPressed;
  Ui::ModuleWidgetUI* m_pUi;
  Host* m_pHost;
  QTimer m_aTimer;

  typedef std::map<ModuleInputBase*, QWidget*> tInputsCollection;
  tInputsCollection m_collInputWidgets;

  typedef std::map<ModuleOutputBase*, QWidget*> tOutputsCollection;
  tOutputsCollection m_collOutputWidgets;

  static tMapping m_aInputMapping;
  static tMapping m_aOutputMapping;
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------getModule
inline Module* ModuleWidget::getModule()
{ 
  return m_pModule; 
}

//---------------------------------------------getModule
inline const Module* ModuleWidget::getModule() const
{ 
  return m_pModule; 
}

} } 

#endif
