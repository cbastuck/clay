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

#ifndef CLAY_DOUBLE_BUFFER_H_
#define CLAY_DOUBLE_BUFFER_H_

#include <clay-core/base/ClayDefines.h>
#include <clay-core/helper/Mutex.h>

namespace CLAY{ 

template<class T>
class DoubleBuffer
{
public:
  DoubleBuffer();
  DoubleBuffer(T* pFrontBuffer, T* pBackBuffer);
  ~DoubleBuffer();
  
  void acquirePreventSwitchLock();
  void releasePreventSwitchLock(bool bScheduleSwitch=true);

  bool trySwitchIfPending();

  T* getFrontBuffer();
  T* getBackBuffer();

private:
  T*    m_pFrontBuffer;
  T*    m_pBackBuffer;
  Mutex m_aPreventSwitchLock;
  bool  m_bSwitchPending;
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------DoubleBuffer
template<class T>
inline DoubleBuffer<T>::DoubleBuffer()
  : m_pFrontBuffer(NULL),
    m_pBackBuffer(NULL),
    m_bSwitchPending(false)
{
  m_pFrontBuffer = new T;
  m_pBackBuffer  = new T;
}

//---------------------------------------------DoubleBuffer
template<class T>
inline DoubleBuffer<T>::DoubleBuffer(T* pFrontBuffer, T* pBackBuffer)
  : m_pFrontBuffer(pFrontBuffer),
    m_pBackBuffer(pBackBuffer),
    m_bSwitchPending(false)
{
}

//---------------------------------------------~DoubleBuffer
template<class T>
inline DoubleBuffer<T>::~DoubleBuffer()
{
  delete m_pFrontBuffer;
  delete m_pBackBuffer;
}

//---------------------------------------------acquirePreventSwitchLock
template<class T>
inline void DoubleBuffer<T>::acquirePreventSwitchLock()
{
  m_aPreventSwitchLock.lock();
}

//---------------------------------------------releasePreventSwitchLock
template<class T>
inline void DoubleBuffer<T>::releasePreventSwitchLock(bool bScheduleSwitch)
{
  m_bSwitchPending = bScheduleSwitch;
  m_aPreventSwitchLock.unlock();
}

//---------------------------------------------trySwitchIfPending
template<class T>
inline bool DoubleBuffer<T>::trySwitchIfPending()
{
  if(m_bSwitchPending)
  {
    if(m_aPreventSwitchLock.try_lock())
    {
      std::swap(m_pFrontBuffer, m_pBackBuffer);
      m_bSwitchPending = false;
      m_aPreventSwitchLock.unlock();
      return true;
    }
  }
  return false; //no switch (needed or resource blocked)
}

//---------------------------------------------getFrontBuffer
template<class T>
inline T* DoubleBuffer<T>::getFrontBuffer()
{
  return m_pFrontBuffer;
}

//---------------------------------------------getBackBuffer
template<class T>
inline T* DoubleBuffer<T>::getBackBuffer()
{
  return m_pBackBuffer;
}

}

#endif
