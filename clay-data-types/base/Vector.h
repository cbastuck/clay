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

namespace CLAY{ namespace MODEL{

template<class T>
class Vector
{
public:
  Vector();
  Vector(unsigned int uSize);
  Vector(T* p, unsigned int uOffset, unsigned int uNum);
  Vector(const Vector& aRhs);
  ~Vector();

  //assignment
  Vector<T>& operator=(const Vector<T>& aRhs);
  Vector<T>& operator=(const T& v);

  //ownership
  void adopt(Vector<T>& aRhs);

  //write access
  template<unsigned int i>
  void set(T p[i]);

  //read access
  T& operator()(unsigned int i);
  const T& operator()(unsigned int i) const;

  unsigned int size() const;
  void resize(unsigned int uSize);

  Vector<T> subvector(unsigned int uPos, unsigned int uNum);
  Const< Vector<T> > subvector(unsigned int uPos, unsigned int uNum) const;

private:
  T*           m_pData;
  unsigned int m_uOffset;
  unsigned int m_uNum;
  bool         m_bMemoryOwner;
};


//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------
template<class T>
inline Vector<T>::Vector()
   : m_pData(NULL),
     m_uOffset(1),
     m_uNum(0),
     m_bMemoryOwner(true)
{
}

//---------------------------------------------Vector
template<class T>
inline Vector<T>::Vector(unsigned int uSize)
   : m_pData(NULL),
     m_uOffset(1),
     m_uNum(uSize),
     m_bMemoryOwner(true)
{
  m_pData = new(T[uSize]);
}

//---------------------------------------------Vector
template<class T>
inline Vector<T>::Vector(T* p, unsigned int uOffset, unsigned int uNum)
 : m_pData(p),
   m_uOffset(uOffset),
   m_uNum(uNum),
   m_bMemoryOwner(false)
{
}

//---------------------------------------------Vector
template<class T>
inline Vector<T>::Vector(const Vector& aRhs)
 : m_pData(aRhs.m_pData),
   m_uOffset(aRhs.m_uOffset),
   m_uNum(aRhs.m_uNum),
   m_bMemoryOwner(false) //copies of a vector have never the ownership
{
}

//---------------------------------------------~Vector
template<class T>
inline Vector<T>::~Vector()
{
  if(m_bMemoryOwner && m_pData)
  {
    delete[] m_pData;
  }
}

//---------------------------------------------operator=
template<class T>
inline Vector<T>& Vector<T>::operator=(const Vector<T>& aRhs)
{
  CLAY_ASSERT(m_uNum == aRhs.m_uNum);
  if(m_uOffset == 1 && aRhs.m_uOffset == 1)
  {
    memcpy(m_pData, aRhs.m_pData, m_uNum * sizeof(T));
  }
  else
  {
    for(unsigned int i=0; i<m_uNum; ++i)
    {
      operator()(i) = aRhs(i);
    }
  }
  return *this;
}

//---------------------------------------------operator=
template<class T>
inline Vector<T>& Vector<T>::operator=(const T& aRhs)
{
  for(unsigned int i=0; i<m_uNum; ++i)
  {
    operator()(i) = aRhs(i);
  }

  return *this;
}

//---------------------------------------------adopt
template<class T>
inline void Vector<T>::adopt(Vector<T>& aRhs)
{
  CLAY_ASSERT(aRhs.m_bMemoryOwner); //adoptee must have ownership

  m_uNum    = aRhs.m_uNum;
  m_pData   = aRhs.m_pData;
  m_uOffset = aRhs.m_uOffset;

  m_bMemoryOwner = true;
  aRhs.m_bMemoryOwner = false; //ownership transferred
}

//---------------------------------------------set
template<class T>
template<unsigned int i>
inline void Vector<T>::set(T p[i])
{
  CLAY_ASSERT(m_uNum == i);
  memcpy(m_pData, p, m_uNum * sizeof(T));
}

//---------------------------------------------operator()
template<class T>
inline T& Vector<T>::operator()(unsigned int i)
{
  CLAY_ASSERT(i < m_uNum);
  return m_pData[i * m_uOffset];
}

//---------------------------------------------operator()
template<class T>
inline const T& Vector<T>::operator()(unsigned int i) const
{
  CLAY_ASSERT(i < m_uNum);
  return m_pData[i * m_uOffset];
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
    aResized = aSubvec;
  }
  else //vector grows
  {
    Vector<T> aSubvec = aResized.subvector(0, m_uNum);
    aSubvec = *this;
  }
  adopt(aResized);
}

//---------------------------------------------subvector
template<class T>
inline Vector<T> Vector<T>::subvector(unsigned int uPos, unsigned int uNum)
{
  CLAY_ASSERT(uNum < m_uNum);

  Vector<T> aVec;

  aVec.m_pData        = m_pData + uPos * m_uOffset;
  aVec.m_uNum         = std::min(uNum, m_uNum);
  aVec.m_uOffset      = m_uOffset;
  aVec.m_bMemoryOwner = false;

  return aVec;
}

//---------------------------------------------subvector
template<class T>
inline Const<Vector<T> > Vector<T>::subvector(unsigned int uPos, unsigned int uNum) const
{
  Vector<T> aVec = const_cast<Vector<T>*>(this)->subvector(uPos, uNum); //const-ness is preserved
  return Const< Vector<T> >(aVec);
}

} }

#endif
