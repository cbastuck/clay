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

#include "MemoryOwner.h"

#include <string.h>

namespace CLAY
{
  namespace MODEL
  {

    //---------------------------------------------MemoryOwner
    MemoryOwner::MemoryOwner()
      : m_pMemory(NULL),
        m_aCapacity(tDataType(0)),
        m_aUsedMemorySize(tDataType(0)),
        m_pOriginInstance(NULL)
    {

    }

    //---------------------------------------------MemoryOwner
    MemoryOwner::MemoryOwner(const MemoryOwner& aRhs)
    {
      this->operator=(aRhs);
    }

    //---------------------------------------------~MemoryOwner
    MemoryOwner::~MemoryOwner()
    {
      release();
      CLAY_ASSERT(!m_pMemory); //did you forget to release the memory
    }

    //---------------------------------------------reserve
    MemoryOwner::tDataType* MemoryOwner::reserve(tSizeType aMemorySize)
    {
      if(m_aCapacity < aMemorySize)
      {
        allocate(aMemorySize);
      }
      m_aCapacity = aMemorySize;
      return m_pMemory;
    }

    //---------------------------------------------resize
    MemoryOwner::tDataType* MemoryOwner::resize(tSizeType aMemorySize)
    {
      if(aMemorySize > m_aCapacity)
      {
        reserve(aMemorySize);
      }
      m_aUsedMemorySize = aMemorySize;
      return m_pMemory;
    }

    //---------------------------------------------attach
    bool MemoryOwner::attach(MemoryOwner& other)
    {
      release();
      m_eMemoryOrigin = eOtherMemoryOwner;
      m_pOriginInstance = &other;
      m_pMemory = other.getMemoryPtr();
      m_aUsedMemorySize = other.m_aUsedMemorySize;
      m_aCapacity = other.m_aCapacity;
      return true;
    }

    //---------------------------------------------attach
    bool MemoryOwner::attach(char* ptr, tSizeType aSize)
    {
      release();
      m_eMemoryOrigin = eUnknownOwner;
      m_pMemory = ptr;
      m_aCapacity = aSize;
      m_aUsedMemorySize = aSize;
      return true;
    }

    //---------------------------------------------attachConstCast
    bool MemoryOwner::attachConstCast(const MemoryOwner& other)
    {
      return attach(const_cast<MemoryOwner&>(other));
    }

    //---------------------------------------------getSize
    MemoryOwner::tSizeType MemoryOwner::getCapacity() const
    {
      return m_aCapacity;
    }

    //---------------------------------------------getUsedSize
    MemoryOwner::tSizeType MemoryOwner::getUsedSize() const
    {
      return m_aUsedMemorySize;
    }

    //---------------------------------------------copy
    void MemoryOwner::copy(const tDataType* src, tSizeType aSize)
    {
      CLAY_ASSERT(m_aCapacity > aSize);

      memcpy(m_pMemory, src, aSize);
      if(aSize > m_aUsedMemorySize)
      {
        m_aUsedMemorySize = aSize;
      }
    }

    //---------------------------------------------copy
    void MemoryOwner::copy(const tDataType* src, tSizeType aDstIdx, tSizeType aNum)
    {
      tSizeType aTargetSize = aDstIdx + aNum; 
      CLAY_ASSERT(m_aCapacity >= aTargetSize);

      memcpy(m_pMemory + aDstIdx, src, aNum);
      if(aTargetSize > m_aUsedMemorySize)
      {
        m_aUsedMemorySize = aTargetSize; //update the number of used bytes
      }
    }

    //---------------------------------------------fill
    void MemoryOwner::fill(tDataType* dst, tSizeType aSize) const
    {
      CLAY_ASSERT(m_aCapacity > aSize);
      memcpy(dst, m_pMemory, aSize);
    }

    //---------------------------------------------getMemoryPtr
    MemoryOwner::tDataType* MemoryOwner::getMemoryPtr()
    {
      return m_pMemory;
    }

    //---------------------------------------------getMemoryPtr
    MemoryOwner::tDataType* MemoryOwner::getMemoryPtr() const
    {
      return m_pMemory;
    }

    //---------------------------------------------set
    void MemoryOwner::set(tSizeType uIdx, const tDataType* src, tSizeType aSize)
    {
      CLAY_ASSERT(m_aCapacity > uIdx);
      memcpy(&m_pMemory[uIdx], src, aSize);
      m_aUsedMemorySize = std::max(m_aUsedMemorySize, uIdx);
    }

    //---------------------------------------------get
    const MemoryOwner::tDataType* MemoryOwner::get(tSizeType uIdx) const
    {
      return &m_pMemory[uIdx];
    }
    
    //---------------------------------------------adopt
    void MemoryOwner::adopt(MemoryOwner& aAdoptee)
    {
      m_aUsedMemorySize = aAdoptee.m_aUsedMemorySize;
      m_aCapacity = aAdoptee.m_aCapacity;
      m_eMemoryOrigin   = eThis;
      m_pOriginInstance = this;
      m_pMemory = aAdoptee.m_pMemory;

      //reset the adoptee
      aAdoptee.m_pMemory = NULL;
      aAdoptee.m_aUsedMemorySize = 0;
      aAdoptee.m_aCapacity = 0;
    }

    //---------------------------------------------share
    void MemoryOwner::share(MemoryOwner& aShared, tSizeType aOffset)
    {
      aShared.m_aUsedMemorySize = m_aUsedMemorySize;
      aShared.m_aCapacity       = m_aCapacity;
      aShared.m_eMemoryOrigin   = eOtherMemoryOwner;
      aShared.m_pOriginInstance = this;
      aShared.m_pMemory         = m_pMemory + aOffset;
    }

    //---------------------------------------------init
    void MemoryOwner::init()
    {

    }

    //---------------------------------------------deInit
    void MemoryOwner::deInit()
    {
      release();
    }

    //---------------------------------------------operator=
    const MemoryOwner& MemoryOwner::operator=(const MemoryOwner& aRhs)
    {
      /*
      if(&aRhs != this)
      {
        reserve(aRhs.m_aCapacity);
        m_aUsedMemorySize = aRhs.m_aUsedMemorySize;
        m_eMemoryOrigin   = eThis;
        m_pOriginInstance = this;
        memcpy(m_pMemory, aRhs.m_pMemory, m_aUsedMemorySize);
      }
      */
      memcpy(this, &aRhs, sizeof(aRhs) * 8);
      return *this;
    }

    //---------------------------------------------allocate
    MemoryOwner::tDataType* MemoryOwner::allocate(tSizeType aSize)
    {
      release();
      m_pMemory = new tDataType[aSize];
      m_eMemoryOrigin = eThis;
      return m_pMemory;
    }

    //---------------------------------------------release
    void MemoryOwner::release()
    {
      if(m_pMemory)
      {
        if(m_eMemoryOrigin == eThis)
        {
          delete[] m_pMemory;
        }
        m_pMemory = NULL;
      }
      m_eMemoryOrigin = eUnknownOwner;
      m_pOriginInstance = NULL;
    }
  }
}
