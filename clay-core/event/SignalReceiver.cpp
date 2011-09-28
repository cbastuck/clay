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

#include "SignalReceiver.h"

namespace CLAY
{

  //---------------------------------------------SignalReceiver
  SignalReceiver::SignalReceiver()
  {
  }

  //---------------------------------------------~SignalReceiver
  SignalReceiver::~SignalReceiver()
  {
    tSignalCollection::iterator it  = m_collSignals.begin();
    tSignalCollection::iterator end = m_collSignals.end();
    for(; it!=end; ++it)
    {
      it->first->removeReceiver(it->second);
    }
  }

  //---------------------------------------------addConnection
  void SignalReceiver::addConnection(SignalBase* pSignal, unsigned int uIdx)
  {
    m_collSignals.push_back(std::make_pair(pSignal, uIdx));
  }

  //---------------------------------------------removeConnections
  void SignalReceiver::removeConnections(SignalBase* pSignal)
  {
    tSignalCollection::iterator it  = m_collSignals.begin();
    tSignalCollection::iterator end = m_collSignals.end();
    for(; it!=end; )
    {
      if(it->first == pSignal)
      {
        pSignal->removeReceiver(it->second);
        it  = m_collSignals.erase(it);
        end = m_collSignals.end();
      }
      else
      {
        ++it;
      }
    }
  }

}