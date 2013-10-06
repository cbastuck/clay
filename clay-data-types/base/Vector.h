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

#ifndef CLAY_VECTOR_H_
#define CLAY_VECTOR_H_

#include <clay-core/base/ClayDefines.h>
#include <clay-data-types/base/Const.h>
#include <clay-data-types/base/SharedData.h>

#include <algorithm>

namespace CLAY{ namespace MODEL{

template<class T>
class Vector
{
public:
  typedef T tValueType;

  Vector();
  Vector(unsigned int uSize);
  Vector(T* p, unsigned int uStepwidth, unsigned int uNum);
  Vector(const Vector& aRhs);
  ~Vector();

  //assignment
  Vector<T>& operator=(const Vector<T>& aRhs);

  //ownership
  void adopt(Vector<T>& aRhs);

  //read/write access
  template<unsigned int i>
  void set(T p[i]);
  void set(const Vector<T>& aRhs);
  void set(const T& v);

  T& operator()(unsigned int i);
  const T& operator()(unsigned int i) const;

  T& operator[](unsigned int i);
  const T& operator[](unsigned int i) const;

  unsigned int size() const;
  void resize(unsigned int uSize);

  Vector<T> subvector(unsigned int uPos, unsigned int uNum);
  Const< Vector<T> > subvector(unsigned int uPos, unsigned int uNum) const;

  void normalize();
  T getMaxValue() const;

protected:
  enum
  {
    eNoOwnership = 1 << 0,
  };
  void deref();

private:
  SharedData<T*>* m_pData;
  unsigned int    m_uStepwidth;
  unsigned int    m_uNum;
};


//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------
template<class T>
inline Vector<T>::Vector()
   : m_pData(NULL)
   , m_uStepwidth(1)
   , m_uNum(0)
{
  m_pData = new SharedData<T*>();
  m_pData->set(NULL); //initialize
  m_pData->inc();
}

//---------------------------------------------Vector
template<class T>
inline Vector<T>::Vector(unsigned int uSize)
   : m_pData(NULL)
   , m_uStepwidth(1)
   , m_uNum(uSize)
{
  m_pData = new SharedData<T*>(new T[uSize]);
  m_pData->inc();
}

//---------------------------------------------Vector
template<class T>
inline Vector<T>::Vector(T* p, unsigned int uStepwidth, unsigned int uNum)
 : m_pData(NULL)
 , m_uStepwidth(uStepwidth)
 , m_uNum(uNum)
{
  m_pData = new SharedData<T*>(p);
  m_pData->inc();

  /*
   * we do not take the ownership so the actual data 
   * will never be destroyed by us. Take care that the 
   * data is not destroyed during the liftime of the vector!
   */
  m_pData->addFlag(eNoOwnership); 
}

//---------------------------------------------Vector
template<class T>
inline Vector<T>::Vector(const Vector& aRhs)
 : m_pData(NULL)
 , m_uStepwidth(aRhs.m_uStepwidth)
 , m_uNum(aRhs.m_uNum)
{
  operator=(aRhs);
}

//---------------------------------------------~Vector
template<class T>
inline Vector<T>::~Vector()
{
  deref();
}

//---------------------------------------------operator=
template<class T>
inline Vector<T>& Vector<T>::operator=(const Vector<T>& aRhs)
{
  if(aRhs.m_pData != m_pData)
  {
    deref();
    m_pData = aRhs.m_pData;
    m_pData->inc();

    m_uStepwidth = aRhs.m_uStepwidth;
    m_uNum = aRhs.m_uNum;
  }
  return *this;
}

//---------------------------------------------adopt
template<class T>
inline void Vector<T>::adopt(Vector<T>& aRhs)
{
  operator=(aRhs); //adopting and assigning is the same due to reference counting
}

//---------------------------------------------set
template<class T>
template<unsigned int i>
inline void Vector<T>::set(T p[i])
{
  CLAY_ASSERT(m_uNum == i);
  memcpy(m_pData->get(), p, m_uNum * sizeof(T));
}

//---------------------------------------------set
template<class T>
inline void Vector<T>::set(const Vector<T>& aRhs)
{
  CLAY_ASSERT(m_uNum == aRhs.m_uNum);
  if(m_uStepwidth == 1 && aRhs.m_uStepwidth == 1)
  {
    memcpy(m_pData->get(), aRhs.m_pData->get(), m_uNum * sizeof(T)); //optimized version
  }
  else
  {
    for(unsigned int i=0; i<m_uNum; ++i)
    {
      operator()(i) = aRhs(i);
    }
  }
}

//---------------------------------------------set
template<class T>
inline void Vector<T>::set(const T& aVal)
{
  for(unsigned int i=0; i<m_uNum; ++i)
  {
    operator()(i) = aVal;
  }
}

//---------------------------------------------operator()
template<class T>
inline T& Vector<T>::operator()(unsigned int i)
{
  CLAY_ASSERT(i < m_uNum);
  return m_pData->get()[i * m_uStepwidth];
}

//---------------------------------------------operator()
template<class T>
inline const T& Vector<T>::operator()(unsigned int i) const
{
  CLAY_ASSERT(i < m_uNum);
  return m_pData->get()[i * m_uStepwidth];
}

//---------------------------------------------operator[]
template<class T>
T& Vector<T>::operator[](unsigned int i)
{
  return operator()(i);
}

//---------------------------------------------operator[]
template<class T>
const T& Vector<T>::operator[](unsigned int i) const
{
  return operator()(i);
}

//---------------------------------------------size
template<class T>
inline unsigned int Vector<T>::size() const
{
  return m_uNum;
}

//---------------------------------------------resize
template<class T>
inline void Vector<T>::resize(unsigned int uNum)
{
  if(uNum == m_uNum)
  {
    return; //nothing to do
  }

  Vector<T> aResized(uNum);
  if(uNum < m_uNum) //vector shrinks
  {
    Vector<T> aSubvec = subvector(0, uNum);
    aResized.set(aSubvec);
  }
  else //vector grows
  {
    Vector<T> aSubvec = aResized.subvector(0, m_uNum);
    aSubvec.set(*this);
  }
  adopt(aResized);
}

//---------------------------------------------subvector
template<class T>
inline Vector<T> Vector<T>::subvector(unsigned int uPos, unsigned int uNum)
{
  CLAY_ASSERT(uNum <= m_uNum);

  Vector<T> aVec;

  aVec.m_uNum = std::min(uNum, m_uNum);
  aVec.m_uStepwidth = m_uStepwidth;

  aVec.m_pData->set(m_pData->get() + uPos * m_uStepwidth);
  aVec.m_pData->addFlag(eNoOwnership);

  return aVec;
}

//---------------------------------------------subvector
template<class T>
inline Const<Vector<T> > Vector<T>::subvector(unsigned int uPos, unsigned int uNum) const
{
  Vector<T> aVec = const_cast<Vector<T>*>(this)->subvector(uPos, uNum); //const-ness is preserved
  return Const< Vector<T> >(aVec);
}

//---------------------------------------------deref
template<class T>
inline void Vector<T>::deref()
{
  CLAY_ASSERT(!m_pData || m_pData->count());
  if(m_pData && m_pData->dec() == 0)
  {
    if(!m_pData->testFlag(eNoOwnership))
    {
      delete[] m_pData->get(); //do the wall-e
    }
    delete m_pData; //not done in d'tor, but only if counter is 0
    m_pData = NULL;
  }
}

//---------------------------------------------deref
template<class T>
inline void Vector<T>::normalize()
{
  //step1: accumulate the elements
  T aAcc = static_cast<T>(0);
  unsigned int n=size();
  for(unsigned int i=0; i<n; ++i)
  {
    aAcc += operator()(i);
  }

  //step2: devide by accumulate value
  for(unsigned int i=0; i<n; ++i)
  {
    operator()(i) /= aAcc;
  }
}

//---------------------------------------------
template<class T>
inline T Vector<T>::getMaxValue() const
{
  if(m_uStepwidth == 1)
  {
    return *std::max_element(m_pData->get(), m_pData->get()+m_uNum);
  }
  else if(m_uNum)
  {
    T* ptr = m_pData->get();
    T tMax = ptr[0];
    for(unsigned int i=1; i<m_uNum; ++i)
    {
      if(ptr[i] > tMax)
      {
        tMax = ptr[i];
      }
    }
    return tMax;
  }
  CLAY_FAIL();
  return 0;
}

} }

#endif
