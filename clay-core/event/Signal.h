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

#ifndef SIGNAL_H_
#define SIGNAL_H_

//clay
#include <clay-core/base/ClayDefines.h>
#include <clay-core/event/SignalBase.h>
#include <clay-core/event/SignalReceiver.h>

//STL
#include <deque>

//boost
#include <boost/bind.hpp>
#include <boost/function.hpp>
namespace CLAY{
  
class SignalReceiver;

template<class tCallback=boost::function<void()> >
class Signal : public SignalBase
{
public:
  typedef typename tCallback::result_type result_type;

  //---------------------------------------------Signal
  CLAY_DLL_EXPORT Signal()
  {
  }

  //---------------------------------------------~Signal
  CLAY_DLL_EXPORT ~Signal()
  {
    typename tSlotCollection::iterator it  = m_collRegisteredSlots.begin();
    typename tSlotCollection::iterator end = m_collRegisteredSlots.end();
    for(; it!=end; ++it)
    {
      SignalReceiver* pReceiver = it->second;
      pReceiver->removeConnections(this);
    }
  }

  //---------------------------------------------removeReceiver
  CLAY_DLL_EXPORT virtual void removeReceiver(unsigned int uIdx)
  {
    m_collRegisteredSlots.erase(m_collRegisteredSlots.begin() + uIdx);
  }

  //---------------------------------------------emit0
  CLAY_DLL_EXPORT void emit0()
  {
    typename tSlotCollection::iterator it  = m_collRegisteredSlots.begin();
    typename tSlotCollection::iterator end = m_collRegisteredSlots.end();
    for(; it!=end; ++it)
    {
      it->first.operator()();
    }
  }

  //---------------------------------------------emit1
  template<class T0>
  CLAY_DLL_EXPORT void emit1(T0 t0)
  {
    typename tSlotCollection::iterator it  = m_collRegisteredSlots.begin();
    typename tSlotCollection::iterator end = m_collRegisteredSlots.end();
    for(; it!=end; ++it)
    {
      it->first.operator()(t0);
    }
  }

  //---------------------------------------------emit2
  template<class T0, class T1>
  CLAY_DLL_EXPORT void emit2(T0 t0, T1 t1)
  {
    typename tSlotCollection::iterator it  = m_collRegisteredSlots.begin();
    typename tSlotCollection::iterator end = m_collRegisteredSlots.end();
    for(; it!=end; ++it)
    {
      it->first.operator()(t0, t1);
    }
  }

  //---------------------------------------------unregisterAllCallbacks
  CLAY_DLL_EXPORT void unregisterAllCallbacks()
  {
    m_collRegisteredSlots.clear();
  }

private:
  friend class SignalReceiver;

  typedef std::deque<std::pair<tCallback, SignalReceiver*> > tSlotCollection;
  tSlotCollection m_collRegisteredSlots;
};

}

#endif
