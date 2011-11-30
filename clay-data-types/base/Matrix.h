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
#ifndef CLAY_MATRIX_H_
#define CLAY_MATRIX_H_

#include <clay-core/base/ClayDefines.h>

#include <clay-data-types/base/Const.h>
#include <clay-data-types/base/Vector.h>
#include <clay-data-types/base/SharedData.h>

namespace CLAY{ namespace MODEL{

template<class T>
class Matrix
{
public:
  typedef Vector<T>      tVector;
  typedef Const<tVector> tConstVector;

  enum Alignment
  {
    ROW_ALIGNMENT,
    COLUMN_ALIGNMENT,
  };

  //lifetime
  Matrix();
  Matrix(unsigned int uNumRows, unsigned int uNumCols, Alignment aAlignment=ROW_ALIGNMENT);
  Matrix(const Matrix<T>& aRhs);
  ~Matrix();

  template<unsigned int i, unsigned int j> 
  static Matrix<T> create(T p[i][j]);

  Matrix<T>& operator=(const Matrix<T>& aRhs);

  Matrix<T> clone() const;
  void clear();
  void attach(const Matrix<T>& aRhs);

  //assignment
  Matrix<T>& set(const T& aVal);
  Matrix<T>& set(const Matrix<T>& aRhs);
  template<unsigned int i, unsigned int j>
  void set(const T p[i][j]);
  
  //matrix dimension
  unsigned int getNumRows() const;
  unsigned int getNumColumns() const;
  void resize(unsigned int uNumRows, unsigned int uCols, const T* pVal=NULL);
  void reserve(unsigned int uNumAlignedRows, unsigned int uNumAlignedCols);

  //read access
  T& operator()(unsigned int uRow, unsigned int uCol);
  const T& operator()(unsigned int uRow, unsigned int uCol) const;
  
  tVector row(unsigned int uRow);
  tConstVector row(unsigned int uRow) const;

  tVector column(unsigned int uCol);
  tConstVector column(unsigned int uCol) const;

  void appendRow(const tVector& aVec);
  void appendColumn(const tVector& aVec);

  //operations
  void transpose();
  Matrix<T> submatrix(unsigned int uRow, unsigned int uNumRows, unsigned int uCol, unsigned int uNumCols);
  Const< Matrix<T> > submatrix(unsigned int uRow, unsigned int uNumRows, unsigned int uCol, unsigned int uNumCols) const;

protected:
  enum
  {
    eNoOwnership = 1 << 0,
  };

  void setNumRows(unsigned int uNumRows);
  void setNumColumns(unsigned int uNumCols);

  unsigned int rowOffset(unsigned int uRow) const;
  unsigned int colOffset(unsigned int uCol) const;

  void deref();

private:
  SharedData<T*>* m_pData;
  unsigned int    m_uNumRows;
  unsigned int    m_uNumCols;
  unsigned int    m_uNumAlignedRows;
  unsigned int    m_uNumAlignedCols;
  Alignment       m_aAlignment;
};


//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------Matrix
template<class T>
inline Matrix<T>::Matrix()
  : m_pData(NULL)
  , m_uNumRows(0)
  , m_uNumCols(0)
  , m_uNumAlignedRows(0)
  , m_uNumAlignedCols(0)
  , m_aAlignment(ROW_ALIGNMENT)
{
  m_pData = new SharedData<T*>();
  m_pData->set(NULL); //initialize
  m_pData->inc();
}

//---------------------------------------------Matrix
template<class T>
inline Matrix<T>::Matrix(unsigned int uNumRows, unsigned int uNumCols, Alignment aAlignment)
  : m_pData(NULL)
  , m_uNumRows(uNumRows)
  , m_uNumCols(uNumCols)
  , m_uNumAlignedRows(uNumRows)
  , m_uNumAlignedCols(uNumCols)
  , m_aAlignment(aAlignment)
{
  m_pData = new SharedData<T*>(new T[m_uNumRows * m_uNumCols]);
  m_pData->inc();

  CLAY_DEBUG(memset(m_pData->get(), 255, m_uNumRows*m_uNumCols*sizeof(T)));
}

//---------------------------------------------Matrix
template<class T>
inline Matrix<T>::Matrix(const Matrix<T>& aRhs)
  : m_pData(NULL),
    m_uNumRows(0),
    m_uNumCols(0),
    m_uNumAlignedRows(0),
    m_uNumAlignedCols(0),
    m_aAlignment(ROW_ALIGNMENT)
{
  operator=(aRhs);
}

//---------------------------------------------~Matrix
template<class T>
inline Matrix<T>::~Matrix()
{
  deref();
}

//---------------------------------------------create
template<class T>
template<unsigned int i, unsigned int j> 
inline Matrix<T> Matrix<T>::create(T p[i][j])
{
  Matrix<T> aDst;

  aDst.m_uNumRows = i;
  aDst.m_uNumCols = j;
  aDst.m_uNumAlignedRows = i;
  aDst.m_uNumAlignedCols = j;
  aDst.m_aAlignment = ROW_ALIGNMENT;

  aDst.m_pData->set((float*)p);
  aDst.m_pData->addFlag(eNoOwnership);

  return aDst;
}


//---------------------------------------------operator=
template<class T>
inline Matrix<T>& Matrix<T>::operator=(const Matrix<T>& aRhs)
{
  if(aRhs.m_pData != m_pData)
  {
    deref();
    m_pData = aRhs.m_pData;
    m_pData->inc();

    m_uNumRows        = aRhs.m_uNumRows;
    m_uNumCols        = aRhs.m_uNumCols;
    m_uNumAlignedRows = aRhs.m_uNumAlignedRows;
    m_uNumAlignedCols = aRhs.m_uNumAlignedCols;
    m_aAlignment      = aRhs.m_aAlignment;
  }
  return *this;
}

//---------------------------------------------clone
template<class T>
inline Matrix<T> Matrix<T>::clone() const
{
  Matrix<T> aCpy;

  aCpy.m_aAlignment      = m_aAlignment;
  aCpy.m_uNumRows        = m_uNumRows;
  aCpy.m_uNumCols        = m_uNumCols;
  aCpy.m_uNumAlignedRows = m_uNumAlignedRows;
  aCpy.m_uNumAlignedCols = m_uNumAlignedCols;
  
  unsigned int uSize = m_uNumAlignedRows * m_uNumAlignedCols;
  aCpy.m_pData->setData(new T[uSize]);
  aCpy.m_pData->inc();

  memcpy(aCpy.m_pData->get(), m_pData->get(), uSize * sizeof(T));

  return aCpy;
}

//---------------------------------------------clear
template<class T>
inline void Matrix<T>::clear()
{
  deref();

  m_pData = new SharedData<T*>();
  m_pData->inc();

  m_uNumRows = 0;
  m_uNumCols = 0;
  m_uNumAlignedRows = 0;
  m_uNumAlignedCols = 0;
}

//---------------------------------------------attach
template<class T>
inline void Matrix<T>::attach(const Matrix<T>& aRhs)
{
  //TODO: remove
  operator=(aRhs); 
}

//---------------------------------------------set
template<class T>
inline Matrix<T>& Matrix<T>::set(const T& aVal)
{
  T* pData = m_pData->get();
  for(unsigned int i=0, n=m_uNumRows*m_uNumCols; i<n; ++i)
  {
    pData[i] = aVal;
  }
  return *this;
}

//---------------------------------------------set
template<class T>
inline Matrix<T>& Matrix<T>::set(const Matrix<T>& aRhs)
{
  CLAY_ASSERT(m_uNumRows == aRhs.m_uNumRows);
  CLAY_ASSERT(m_uNumCols == aRhs.m_uNumCols);
  for(unsigned int i=0; i<m_uNumRows; ++i)
  {
    for(unsigned int j=0; j<m_uNumCols; ++j)
    {
      (*this)(i, j) = aRhs(i, j);
    }
  }
  return *this;
}

//---------------------------------------------set
template<class T>
template<unsigned int i, unsigned int j>
inline void Matrix<T>::set(const T p[i][j])
{
  CLAY_ASSERT(m_uNumRows == i);
  CLAY_ASSERT(m_uNumCols == j);
  memcpy(m_pData->get(), p, i*j * sizeof(T));
}

//---------------------------------------------operator()
template<class T>
inline unsigned int Matrix<T>::getNumRows() const
{
  return m_uNumRows;
}

//---------------------------------------------operator()
template<class T>
inline unsigned int Matrix<T>::getNumColumns() const
{
  return m_uNumCols;
}

//---------------------------------------------resize
template<class T>
inline void Matrix<T>::resize(unsigned int uNumRows, unsigned int uNumCols, const T* pVal)
{
  if(uNumRows >= m_uNumRows && uNumRows <= m_uNumAlignedRows)
  {
    m_uNumRows = uNumRows; //we already have resrved space left for the rows
  }

  if(uNumCols >= m_uNumCols && uNumCols <= m_uNumAlignedCols)
  {
    m_uNumCols = uNumCols; //we already have resrved space left for the cols
  }

  if(m_uNumRows == uNumRows && m_uNumCols == uNumCols)
  {
    return; //nothing to do
  }

  Matrix<T> aResized(uNumRows, uNumCols, m_aAlignment);
  if(pVal)
  {
    aResized.set(*pVal); //set the default entry for new elements if available
  }
  if(uNumRows <= m_uNumRows && uNumCols <= m_uNumCols) //matrix shrinks
  {
    aResized.set(submatrix(0, uNumRows, 0, uNumCols));
  }
  else if(uNumRows >=m_uNumRows && uNumCols >= m_uNumCols) //matrix grows
  {
    Matrix<T> aSubmat = aResized.submatrix(0, m_uNumRows, 0, m_uNumCols);
    aSubmat.set(*this);
  }
  else if(uNumRows < m_uNumRows && uNumCols > m_uNumCols) //rows shrink, columns grow
  {
    Matrix<T> aSubmat = aResized.submatrix(0, uNumRows, 0, m_uNumCols);
    aSubmat.set(submatrix(0, uNumRows, 0, m_uNumCols));
  }
  else if(uNumRows > m_uNumRows && uNumCols < m_uNumCols) //rows grow, columns shrink
  {
    Matrix<T> aSubmat = aResized.submatrix(0, m_uNumRows, 0, uNumCols);
    aSubmat.set(submatrix(0, m_uNumRows, 0, uNumCols));
  }

  operator=(aResized);
}

//---------------------------------------------reserve
template<class T>
inline void Matrix<T>::reserve(unsigned int uNumAlignedRows, unsigned int uNumAlignedCols)
{
  unsigned int uNumRows = m_uNumRows;
  unsigned int uNumCols = m_uNumCols;
  resize(uNumAlignedRows, uNumAlignedCols, NULL);
  m_uNumRows = uNumRows;
  m_uNumCols = uNumCols;
}

//---------------------------------------------operator()
template<class T>
inline T& Matrix<T>::operator()(unsigned int uRow, unsigned int uCol)
{
  const Matrix<T>* pConst = this;
  return const_cast<T&>(pConst->operator()(uRow, uCol));
}

//---------------------------------------------operator()
template<class T>
inline const T& Matrix<T>::operator()(unsigned int uRow, unsigned int uCol) const
{
  CLAY_ASSERT(uRow < m_uNumRows);
  CLAY_ASSERT(uCol < m_uNumCols);
  return m_pData->get()[rowOffset(uRow) + colOffset(uCol)];
}

//---------------------------------------------row
template<class T>
inline typename Matrix<T>::tVector Matrix<T>::row(unsigned int uRow)
{
  CLAY_ASSERT(uRow < m_uNumRows);
  unsigned int uOffset = (m_aAlignment == ROW_ALIGNMENT) ? 1 : m_uNumRows;
  return tVector(&m_pData->get()[rowOffset(uRow)], uOffset, m_uNumCols);
}

//---------------------------------------------row
template<class T>
inline typename Matrix<T>::tConstVector Matrix<T>::row(unsigned int uRow) const
{
  tVector aVec = const_cast<Matrix<T>*>(this)->row(uRow); //const-ness is preserved
  return Const<tVector>(aVec);
}

//---------------------------------------------column
template<class T>
inline typename Matrix<T>::tVector Matrix<T>::column(unsigned int uCol)
{
  CLAY_ASSERT(uCol < m_uNumCols);
  unsigned int uOffset = (m_aAlignment == ROW_ALIGNMENT) ? m_uNumCols : 1;
  return tVector(&m_pData->get()[colOffset(uCol)], uOffset, m_uNumRows);
}

//---------------------------------------------column
template<class T>
inline typename Matrix<T>::tConstVector Matrix<T>::column(unsigned int uCol) const
{
  tVector aVec = const_cast<Matrix<T>*>(this)->column(uCol); //const-ness is preserved
  return Const<tVector>(aVec);
}

//---------------------------------------------appendRow
template<class T>
inline void Matrix<T>::appendRow(const tVector& aVec)
{
  unsigned int uRow = m_uNumRows;
  resize(m_uNumRows+1, m_uNumCols);
  row(uRow) = aVec;
}

//---------------------------------------------appendColumn
template<class T>
inline void Matrix<T>::appendColumn(const tVector& aVec)
{
  unsigned int uCol = m_uNumCols;
  resize(m_uNumRows, m_uNumCols+1);
  column(uCol) = aVec;
}

//---------------------------------------------transpose
template<class T>
inline void Matrix<T>::transpose()
{
  std::swap(m_uNumCols, m_uNumRows);
  std::swap(m_uNumAlignedCols, m_uNumAlignedRows);
  m_aAlignment = (m_aAlignment == ROW_ALIGNMENT) ? COLUMN_ALIGNMENT : ROW_ALIGNMENT;
}

//---------------------------------------------submatrix
template<class T>
inline Matrix<T> Matrix<T>::submatrix(unsigned int uRow, unsigned int uNumRows, unsigned int uCol, unsigned int uNumCols)
{
  Matrix<T> aMat;

  aMat.m_uNumRows        = uNumRows;
  aMat.m_uNumCols        = uNumCols;
  aMat.m_uNumAlignedRows = m_uNumRows;
  aMat.m_uNumAlignedCols = m_uNumCols;
  aMat.m_aAlignment      = m_aAlignment;

  aMat.m_pData->set(m_pData->get() + rowOffset(uRow) + colOffset(uCol));
  aMat.m_pData->addFlag(eNoOwnership);

  return aMat;
}

//---------------------------------------------submatrix
template<class T>
inline Const< Matrix<T> > Matrix<T>::submatrix(unsigned int uRow, unsigned int uNumRows, unsigned int uCol, unsigned int uNumCols) const
{
  Matrix<T> aMat = const_cast<Matrix<T>*>(this)->submatrix(uRow, uNumRows, uCol, uNumCols); //const-ness is preserved
  return Const< Matrix<T> >(aMat);
}

//---------------------------------------------setNumRows
template<class T>
inline void Matrix<T>::setNumRows(unsigned int uNumRows)
{
  m_uNumRows = uNumRows;
}

//---------------------------------------------setNumColumns
template<class T>
inline void Matrix<T>::setNumColumns(unsigned int uNumCols)
{
  m_uNumCols = uNumCols;
}

//---------------------------------------------rowOffset
template<class T>
inline unsigned int Matrix<T>::rowOffset(unsigned int uRow) const
{
  return (m_aAlignment == ROW_ALIGNMENT) ? (uRow * m_uNumAlignedCols) : uRow;
}

//---------------------------------------------colOffset
template<class T>
inline unsigned int Matrix<T>::colOffset(unsigned int uCol) const
{
  return (m_aAlignment == ROW_ALIGNMENT) ? uCol : (uCol * m_uNumAlignedRows);
}

//---------------------------------------------deref
template<class T>
inline void Matrix<T>::deref()
{
  CLAY_ASSERT(!m_pData || m_pData->count());
  if(m_pData && m_pData->dec() == 0)
  {
    if(!m_pData->testFlag(eNoOwnership))
    {
      delete[] m_pData->get(); //do the wall-e
    }
    delete m_pData;
  }
}

} }

#endif
