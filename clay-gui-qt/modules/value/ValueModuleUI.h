#ifndef CLAY_VALUE_MODULE_UI_H_
#define CLAY_VALUE_MODULE_UI_H_

#include <clay-gui-qt/base/ModuleWidgetHelper.h>
#include <clay-modules/value/ValueModule.h>

//Qt
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

namespace CLAY{ namespace UI{

class StringValueCustomWidget : public QObject
{
  Q_OBJECT

public:
  StringValueCustomWidget()
    : m_pLayout(NULL),
      m_pTextEdit(NULL),
      m_pValueModule(NULL)
  {}

  void setValueModule(MODULE::ValueModuleBase* pValueModule)
  {
    m_pValueModule = pValueModule;
  }

  void setupUi(QWidget* pParent)
  {
    CLAY_ASSERT(!m_pLayout);
    m_pLayout = new QVBoxLayout(pParent);
    m_pLayout->setSpacing(4);
    m_pLayout->setContentsMargins(4, 4, 4, 4);
    m_pLayout->setObjectName(QString::fromUtf8("m_pLayout"));

    CLAY_ASSERT(!m_pTextEdit);
    m_pTextEdit = new QTextEdit(pParent);
    m_pLayout->addWidget(m_pTextEdit);

    if(m_pValueModule)
    {
      tString sValue;
      if(m_pValueModule->getString(sValue))
      {
        m_pTextEdit->setText(sValue.c_str());
      }
    }

    m_pButton = new QPushButton;
    m_pButton->setText("apply value");
    m_pLayout->addWidget(m_pButton);

    QObject::connect(m_pButton, SIGNAL(released()), this, SLOT(onApplyValue()));
  }
 
protected slots:
  void onApplyValue()
  {
    CLAY_ASSERT(m_pValueModule);
    QString sText = m_pTextEdit->toPlainText();
    tString sStr = sText.toAscii().constData();
    m_pValueModule->setString(sStr);
  }

private:
  QVBoxLayout*             m_pLayout;
  QPushButton*             m_pButton;
  QTextEdit*               m_pTextEdit;
  MODULE::ValueModuleBase* m_pValueModule;
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

template<class MODULE, class CUSTOM=StringValueCustomWidget>
class ValueModuleUI : public ModuleWidgetHelper<MODULE, ValueModuleUI<MODULE>, CUSTOM>
{
private:
  typedef ModuleWidgetHelper<MODULE, ValueModuleUI<MODULE>, CUSTOM> tBase;

public:
  ValueModuleUI(Module* pModule, Host* pHost, QWidget* pParent);
  
};


//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------ValueModuleUI
template<class MODULE, class CUSTOM>
inline ValueModuleUI<MODULE, CUSTOM>::ValueModuleUI(Module* pModule, Host* pHost, QWidget* pParent)
  : tBase(pModule, pHost, pParent)
{
  tBase::m_aCustomUI.setValueModule(static_cast<MODULE*>(pModule));
}

} }

#endif
