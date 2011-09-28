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

#include "QtListDialog.h"

#include "clay-core/base/ClayDefines.h"

namespace CLAY
{
  namespace UI
  {

    //---------------------------------------------QtListDialog
    QtListDialog::QtListDialog(QWidget* pParent) : tBase(pParent)
    {
      
    }

    //---------------------------------------------init
    void QtListDialog::init()
    {
      m_aUI.setupUi(this);
      QObject::connect(m_aUI.m_pList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(accept()));
    }

    //---------------------------------------------addEntry
    void QtListDialog::addEntry(const QString& sEntry, const QVariant& aUserData)
    {
      m_aUI.m_pList->addItem(new QListWidgetItem(sEntry));
      if(aUserData.type() != QVariant::Invalid)
      {
        m_collUserData.push_back(aUserData);
      }
    }

    //---------------------------------------------getSelectedRow
    int QtListDialog::getSelectedRow() const
    {
      return m_aUI.m_pList->currentRow();
    }

    //---------------------------------------------getSelectedUserData
    QVariant QtListDialog::getSelectedUserData() const
    {
      CLAY_ASSERT((int)m_collUserData.size() > m_aUI.m_pList->currentRow());
      return m_collUserData[m_aUI.m_pList->currentRow()];
    }
  }
}
