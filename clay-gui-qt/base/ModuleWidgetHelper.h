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

#ifndef ModuleWidgetHelper_H_
#define ModuleWidgetHelper_H_

#include <clay-gui-qt/base/ModuleWidget.h>
#include <clay-core/base/ModuleUIDescriptor.h>

namespace CLAY{ namespace UI{
   
template<ModuleUIDescriptorTraits::tFactoryMethod aFactory,    
         ModuleUIDescriptorTraits::tDestroyMethod aDestroy,
         typename MODULE>
class ModuleUIDescriptorHelper : public ModuleUIDescriptor
{
public:
  ModuleUIDescriptorHelper() 
    : ModuleUIDescriptor(aFactory, aDestroy, MODULE::staticModuleURI())
  {

  }
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

class EmptyCustomWidget
{
public:
  void setupUi(QWidget*){}
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

template<class MODULE, class UI, class T=EmptyCustomWidget>
class ModuleWidgetHelper : public ModuleWidget
{
private:
  typedef ModuleWidgetHelper<MODULE, UI, T> tThis;
  typedef ModuleWidget                      tBase;

public:
  static ModuleWidget* create(Module* pModule, Host* pHost, void* pParent);
  static void destroy(ModuleWidget* pWidget);

  ModuleWidgetHelper(Module* pModule, Host* pHost, QWidget* pParent);

  virtual void init();
  virtual void deInit();

  virtual ModuleUIDescriptor* getUIDescriptor();

  virtual void showModuleUI();
  virtual void hideModuleUI();
  virtual bool isVisibleModuleUI();

protected:
  //-------- inline stuff
  T& getUI(){ return m_aCustomUI; }

  QWidget* m_pCustomWidget;
  T        m_aCustomUI;
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------create
template<class MODULE, class UI, class T>
inline ModuleWidget* ModuleWidgetHelper<MODULE, UI, T>::create(Module* pModule, Host* pHost, void* pParent)
{
  return new UI(pModule, pHost, static_cast<QWidget*>(pParent));
}

//---------------------------------------------destroy
template<class MODULE, class UI, class T>
inline void ModuleWidgetHelper<MODULE, UI, T>::destroy(ModuleWidget* pWidget)
{
  CLAY_ASSERT(pWidget);
  delete pWidget;
}

//---------------------------------------------ModuleWidgetHelper
template<class MODULE, class UI, class T>
inline ModuleWidgetHelper<MODULE, UI, T>::ModuleWidgetHelper(Module* pModule, Host* pHost, QWidget* pParent)
  : tBase(pModule, pHost, pParent),
    m_pCustomWidget(NULL)
{

}

//---------------------------------------------init
template<class MODULE, class UI, class T>
inline void ModuleWidgetHelper<MODULE, UI, T>::init()
{
  tBase::init();

  CLAY_ASSERT(!m_pCustomWidget);
  m_pCustomWidget = new QWidget(NULL);
  m_aCustomUI.setupUi(m_pCustomWidget);

  m_pCustomWidget->move(tBase::topLevelWidget()->rect().center()); //center the widget
}

//---------------------------------------------deInit
template<class MODULE, class UI, class T>
inline void ModuleWidgetHelper<MODULE, UI, T>::deInit()
{
  if(m_pCustomWidget)
  {
    m_pCustomWidget->hide();
    delete m_pCustomWidget;
    m_pCustomWidget = NULL;
  }

  tBase::deInit();
}

//---------------------------------------------getUIDescriptor
template<class MODULE, class UI, class T>
inline ModuleUIDescriptor* ModuleWidgetHelper<MODULE, UI, T>::getUIDescriptor()
{
  //create an instance of the UI descriptor (template parameter)
  static ModuleUIDescriptorHelper<tThis::create, tThis::destroy, MODULE> aModuleDescriptor; 
  return &aModuleDescriptor;
}

//---------------------------------------------showModuleUI
template<class MODULE, class UI, class T>
inline void ModuleWidgetHelper<MODULE, UI, T>::showModuleUI()
{
  if(!m_pCustomWidget->isVisible())
  {
    m_pCustomWidget->show();
  }
  m_pCustomWidget->raise();
}

//---------------------------------------------showModuleUI
template<class MODULE, class UI, class T>
inline void ModuleWidgetHelper<MODULE, UI, T>::hideModuleUI()
{
  if(m_pCustomWidget)
  {
    m_pCustomWidget->close();
  }
}

//---------------------------------------------isVisibleModuleUI
template<class MODULE, class UI, class T>
inline bool ModuleWidgetHelper<MODULE, UI, T>::isVisibleModuleUI()
{
  if(m_pCustomWidget)
  {
    return m_pCustomWidget->isVisible();
  }
  return false;
}

} }

#endif
