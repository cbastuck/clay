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

#ifndef SIGNALRECEIVER_H_
#define SIGNALRECEIVER_H_

//clay
#include <clay-core/base/ClayDefines.h>
#include <clay-core/event/SignalBase.h>

//STL
#include <deque>

//boost
#include <boost/bind.hpp>

namespace CLAY{

class SignalReceiver
{
public:
  CLAY_DLL_EXPORT SignalReceiver();
  CLAY_DLL_EXPORT virtual ~SignalReceiver();

  //--- 0
  // binding with 0 arguments
  template<class S, class R, class T>
  CLAY_DLL_EXPORT void addConnection(S* pSignal, R (T::*f)())
  {
    unsigned int uIdx = pSignal->m_collRegisteredSlots.size();
    pSignal->m_collRegisteredSlots.push_back(std::make_pair(boost::bind(f, static_cast<T*>(this)), this)); 
    this->addConnection(pSignal, uIdx);
  }

  //--- 1
  // binding with 1 specified argument
  template<class S, class R, class T, class A0>
  CLAY_DLL_EXPORT void addConnection(S* pSignal, R (T::*f)(A0), A0 a0)
  {
    unsigned int uIdx = pSignal->m_collRegisteredSlots.size();
    pSignal->m_collRegisteredSlots.push_back(std::make_pair(boost::bind(f, static_cast<T*>(this), a0), this)); 
    this->addConnection(pSignal, uIdx);
  }

  // binding with 1 unspecified argument
  template<class S, class R, class T, class A0>
  CLAY_DLL_EXPORT void addConnection(S* pSignal, R (T::*f)(A0))
  {
    unsigned int uIdx = pSignal->m_collRegisteredSlots.size();
    pSignal->m_collRegisteredSlots.push_back(std::make_pair(boost::bind(f, static_cast<T*>(this), _1), this)); 
    this->addConnection(pSignal, uIdx);
  }

  //--- 2
  // binding with 2 specified argument
  template<class S, class R, class T, class A0, class A1>
  CLAY_DLL_EXPORT void addConnection(S* pSignal, R (T::*f)(A0, A1), A0 a0, A1 a1)
  {
    unsigned int uIdx = pSignal->m_collRegisteredSlots.size();
    pSignal->m_collRegisteredSlots.push_back(std::make_pair(boost::bind(f, static_cast<T*>(this), a0, a1), this)); 
    this->addConnection(pSignal, uIdx);
  }

  // binding with 1 specified and 1 unspecified argument
  template<class S, class R, class T, class A0, class A1>
  CLAY_DLL_EXPORT void addConnection(S* pSignal, R (T::*f)(A0, A1), A0 a0)
  {
    unsigned int uIdx = pSignal->m_collRegisteredSlots.size();
    pSignal->m_collRegisteredSlots.push_back(std::make_pair(boost::bind(f, static_cast<T*>(this), a0, _1), this)); 
    this->addConnection(pSignal, uIdx);
  }

  // binding with 1 unspecified and 1 specified argument
  template<class S, class R, class T, class A0, class A1>
  CLAY_DLL_EXPORT void addConnection(S* pSignal, R (T::*f)(A0, A1), A1 a1)
  {
    unsigned int uIdx = pSignal->m_collRegisteredSlots.size();
    pSignal->m_collRegisteredSlots.push_back(std::make_pair(boost::bind(f, static_cast<T*>(this), _1, a1), this)); 
    this->addConnection(pSignal, uIdx);
  }

  // binding with 2 unspecified argument
  template<class S, class R, class T, class A0, class A1>
  CLAY_DLL_EXPORT void addConnection(S* pSignal, R (T::*f)(A0, A1))
  {
    unsigned int uIdx = pSignal->m_collRegisteredSlots.size();
    pSignal->m_collRegisteredSlots.push_back(std::make_pair(boost::bind(f, static_cast<T*>(this), _1, _2), this)); 
    this->addConnection(pSignal, uIdx);
  }

  //--- 3
  //TODO

  CLAY_DLL_EXPORT void removeConnections(SignalBase* pSignal);

protected:
  CLAY_DLL_EXPORT void addConnection(SignalBase* pSignal, unsigned int uSignalIdx);

private:
  typedef std::deque<std::pair<CLAY::SignalBase*, unsigned int> > tSignalCollection;
  tSignalCollection m_collSignals;
};

}

#endif
