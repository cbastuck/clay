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

#ifndef MemoryOwner_H_
#define MemoryOwner_H_

#include <clay-core/base/ClayDefines.h>

namespace CLAY{ namespace MODEL{

class MemoryOwner
{
public:
  typedef char tDataType;
  typedef unsigned int tSizeType;

  MemoryOwner();
  MemoryOwner(const MemoryOwner& aRhs);
  ~MemoryOwner();

  bool attach(MemoryOwner& other);
  bool attach(tDataType* ptr, tSizeType aSize);
  bool attachConstCast(const MemoryOwner& other);

  tSizeType getCapacity() const;
  tSizeType getUsedSize() const;

  void copy(const tDataType* src, tSizeType aSize);
  void copy(const tDataType* src, tSizeType aDstIdx, tSizeType aNum);
  void fill(tDataType* dst, tSizeType aSize) const;

  void set(tSizeType uIdx, const tDataType* src, tSizeType aSize);
  const tDataType* get(tSizeType uIdx) const;

  template<class T>
  void set(tSizeType aIdx, const T& t, tSizeType aNum)
  {
    CLAY_ASSERT(m_aCapacity >= aIdx + (aNum * sizeof(T)));
    for(tSizeType i=0; i<aNum; ++i) //TODO: optimize me
    { 
      memcpy((void*)&m_pMemory[aIdx+(i*sizeof(T))], (void*)(&t), sizeof(T));
    }
    m_aUsedMemorySize = std::max(m_aUsedMemorySize, (tSizeType)(aIdx + aNum * sizeof(T)));
  }

  template<class T>
  void set(tSizeType aIdx, const T& t, tSizeType aNum, tSizeType aNumSkip, tSizeType aEndIdx)
  {
    CLAY_ASSERT(m_aCapacity >= aIdx + ((aNum + aNumSkip) * sizeof(T)));
    CLAY_ASSERT(m_aCapacity >= aEndIdx);
    while(aIdx < aEndIdx)
    {
      for(tSizeType i=0; i<aNum; ++i) //TODO: optimize me
      { 
        memcpy((void*)&m_pMemory[aIdx+(i*sizeof(T))], (void*)(&t), sizeof(T));
      }
      aIdx += (aNumSkip * sizeof(T)); //skip given number of elements after each iteration
    }
    m_aUsedMemorySize = aEndIdx * sizeof(T);
  }

  template<class T>
  void set(const T* pSrc, tSizeType aNum)
  {
    CLAY_ASSERT(m_aCapacity >= aNum);
    memcpy(m_pMemory, pSrc, aNum * sizeof(T));
  }

protected:
  void adopt(MemoryOwner& aAdoptee);
  void share(MemoryOwner& aShared, tSizeType aOffset);

  void init();
  void deInit();

  tDataType* getMemoryPtr();
  tDataType* getMemoryPtr() const;

  tDataType* reserve(tSizeType aMemorySize);
  tDataType* resize (tSizeType aMemorySize);

private:
  const MemoryOwner& operator=(const MemoryOwner& aRhs);
  tDataType* allocate(tSizeType aSize);
  void release();

  tDataType* m_pMemory;
  tSizeType m_aCapacity;
  tSizeType m_aUsedMemorySize;

  //origin of the memory
  enum MemoryOrigin
  {
    eThis,
    eOtherMemoryOwner,
    eUnknownOwner
  };

  MemoryOrigin m_eMemoryOrigin;
  MemoryOwner* m_pOriginInstance;
};
  
} }

#endif
