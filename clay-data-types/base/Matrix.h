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

  void clone(Matrix<T>& aDst) const;
  void clear();
  void attach(const Matrix<T>& aRhs);

  //assignment
  Matrix<T>& operator=(const T& aVal);
  Matrix<T>& operator=(const Matrix<T>& aRhs);

  //ownership
  void adopt(Matrix<T>& aRhs);

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

  //write access
  template<unsigned int i, unsigned int j>
  void set(const T p[i][j]);

  void appendRow(const tVector& aVec);
  void appendColumn(const tVector& aVec);

  //operations
  void transpose();
  Matrix<T> submatrix(unsigned int uRow, unsigned int uNumRows, unsigned int uCol, unsigned int uNumCols);
  Const< Matrix<T> > submatrix(unsigned int uRow, unsigned int uNumRows, unsigned int uCol, unsigned int uNumCols) const;

protected:
  void setNumRows(unsigned int uNumRows);
  void setNumColumns(unsigned int uNumCols);

  unsigned int rowOffset(unsigned int uRow) const;
  unsigned int colOffset(unsigned int uCol) const;

private:
  T*           m_pData;
  unsigned int m_uNumRows;
  unsigned int m_uNumCols;
  unsigned int m_uNumAlignedRows;
  unsigned int m_uNumAlignedCols;
  Alignment    m_aAlignment;
  bool         m_bMemoryOwner;
};


//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------Matrix
template<class T>
inline Matrix<T>::Matrix()
  : m_pData(NULL),
    m_uNumRows(0),
    m_uNumCols(0),
    m_uNumAlignedRows(0),
    m_uNumAlignedCols(0),
    m_aAlignment(ROW_ALIGNMENT),
    m_bMemoryOwner(true)
{
}

//---------------------------------------------Matrix
template<class T>
inline Matrix<T>::Matrix(unsigned int uNumRows, unsigned int uNumCols, Alignment aAlignment)
  : m_pData(NULL),
    m_uNumRows(uNumRows),
    m_uNumCols(uNumCols),
    m_uNumAlignedRows(uNumRows),
    m_uNumAlignedCols(uNumCols),
    m_aAlignment(aAlignment),
    m_bMemoryOwner(true)
{
  m_pData = new T[m_uNumRows * m_uNumCols];
  CLAY_DEBUG(memset(m_pData, 255, m_uNumRows*m_uNumCols*sizeof(T)));
}

//---------------------------------------------Matrix
template<class T>
inline Matrix<T>::Matrix(const Matrix<T>& aRhs)
  : m_pData(NULL),
    m_uNumRows(0),
    m_uNumCols(0),
    m_uNumAlignedRows(0),
    m_uNumAlignedCols(0),
    m_aAlignment(ROW_ALIGNMENT),
    m_bMemoryOwner(false)
{
  attach(aRhs); //copies are never memory owners - they just use the memory of another
}

//---------------------------------------------~Matrix
template<class T>
inline Matrix<T>::~Matrix()
{
  clear();
}

//---------------------------------------------operator()
template<class T>
inline void Matrix<T>::clone(Matrix<T>& aDst) const
{
  aDst.m_aAlignment      = m_aAlignment;
  aDst.m_uNumRows        = m_uNumRows;
  aDst.m_uNumCols        = m_uNumCols;
  aDst.m_uNumAlignedRows = m_uNumAlignedRows;
  aDst.m_uNumAlignedCols = m_uNumAlignedCols;
  aDst.m_bMemoryOwner = true;

  unsigned int uSize = m_uNumAlignedRows * m_uNumAlignedCols;
  aDst.m_pData = new T[uSize];
  memcpy(aDst.m_pData, m_pData, uSize * sizeof(T));
}

//---------------------------------------------clear
template<class T>
inline void Matrix<T>::clear()
{
  if(m_bMemoryOwner && m_pData)
  {
    CLAY_DEBUG(memset(m_pData, 255, m_uNumRows*m_uNumCols*sizeof(T)));
    delete[] m_pData;
  }

  m_pData = NULL;
  m_uNumRows = 0;
  m_uNumCols = 0;
  m_uNumAlignedRows = 0;
  m_uNumAlignedCols = 0;
}

//---------------------------------------------attach
template<class T>
inline void Matrix<T>::attach(const Matrix<T>& aRhs)
{
  if(m_bMemoryOwner && m_pData)
  {
    delete[] m_pData;
  }

  m_pData           = aRhs.m_pData;
  m_uNumRows        = aRhs.m_uNumRows;
  m_uNumCols        = aRhs.m_uNumCols;  
  m_aAlignment      = aRhs.m_aAlignment;
  m_bMemoryOwner    = false;
  m_uNumAlignedRows = aRhs.m_uNumAlignedRows;
  m_uNumAlignedCols = aRhs.m_uNumAlignedCols;
}

//---------------------------------------------operator()
template<class T>
inline Matrix<T>& Matrix<T>::operator=(const T& aVal)
{
  for(unsigned int i=0, n=m_uNumRows*m_uNumCols; i<n; ++i)
  {
    m_pData[i] = aVal;
  }
  return *this;
}

//---------------------------------------------operator()
template<class T>
inline Matrix<T>& Matrix<T>::operator=(const Matrix<T>& aRhs)
{
  CLAY_ASSERT(m_uNumRows == aRhs.m_uNumRows);
  CLAY_ASSERT(m_uNumCols == aRhs.m_uNumCols);
  if(m_aAlignment == ROW_ALIGNMENT) //optimize according alignment
  {
    for(unsigned int i=0; i<m_uNumRows; ++i)
    {
      row(i) = aRhs.row(i);
    }
  }
  else if(m_aAlignment == COLUMN_ALIGNMENT)
  {
    for(unsigned int i=0; i<m_uNumCols; ++i)
    {
      column(i) = aRhs.column(i);
    }
  }
  return *this;
}

//---------------------------------------------adopt
template<class T>
void Matrix<T>::adopt(Matrix<T>& aRhs)
{
  CLAY_ASSERT(aRhs.m_bMemoryOwner); //can only adopt if ownership is correct
  if(m_pData && m_bMemoryOwner)
  {
    delete[] m_pData;
  }

  m_pData           = aRhs.m_pData;
  m_uNumRows        = aRhs.m_uNumRows;
  m_uNumCols        = aRhs.m_uNumCols;
  m_uNumAlignedRows = aRhs.m_uNumAlignedRows;
  m_uNumAlignedCols = aRhs.m_uNumAlignedCols;
  m_aAlignment      = aRhs.m_aAlignment;

  m_bMemoryOwner      = true;
  aRhs.m_bMemoryOwner = false; //ownserhip transferred
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
    aResized = *pVal; //set the default entry for new elements if available
  }
  if(uNumRows <= m_uNumRows && uNumCols <= m_uNumCols) //matrix shrinks
  {
    Matrix<T> aSubmat = submatrix(0, uNumRows, 0, uNumCols);
    aResized = aSubmat;
  }
  else if(uNumRows >=m_uNumRows && uNumCols >= m_uNumCols) //matrix grows
  {
    Matrix<T> aSubmat = aResized.submatrix(0, m_uNumRows, 0, m_uNumCols);
    aSubmat = *this;
  }
  else if(uNumRows < uNumRows && uNumCols > m_uNumCols) //rows shrink, columns grow
  {
    Matrix<T> aSubmat = submatrix(0, uNumRows, 0, m_uNumCols);
    aResized = aSubmat;
  }
  else if(uNumRows > m_uNumRows && uNumCols < m_uNumCols) //rows grow, columns shrink
  {
    Matrix<T> aSubmat = aResized.submatrix(0, m_uNumRows, 0, uNumCols);
    aSubmat = *this;
  }

  adopt(aResized);
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
  return m_pData[rowOffset(uRow) + colOffset(uCol)];
}

//---------------------------------------------row
template<class T>
inline typename Matrix<T>::tVector Matrix<T>::row(unsigned int uRow)
{
  CLAY_ASSERT(uRow < m_uNumRows);
  unsigned int uOffset = (m_aAlignment == ROW_ALIGNMENT) ? 1 : m_uNumRows;
  return tVector(&m_pData[rowOffset(uRow)], uOffset, m_uNumCols);
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
  return tVector(&m_pData[colOffset(uCol)], uOffset, m_uNumRows);
}

//---------------------------------------------column
template<class T>
inline typename Matrix<T>::tConstVector Matrix<T>::column(unsigned int uCol) const
{
  tVector aVec = const_cast<Matrix<T>*>(this)->column(uCol); //const-ness is preserved
  return Const<tVector>(aVec);
}

//---------------------------------------------operator()
template<class T>
template<unsigned int i, unsigned int j>
inline void Matrix<T>::set(const T p[i][j])
{
  CLAY_ASSERT(m_uNumRows == i);
  CLAY_ASSERT(m_uNumCols == j);
  memcpy(m_pData, p, i*j * sizeof(T));
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
  aMat.m_bMemoryOwner    = false;
  aMat.m_pData           = m_pData + rowOffset(uRow) + colOffset(uCol);

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

} }

#endif
