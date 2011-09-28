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

#ifndef MATRIX_MODEL_H_
#define MATRIX_MODEL_H_

#include "MemoryOwner.h"

#include <fstream>

namespace CLAY{ namespace MODEL{

template<class tElementType>
class MatrixModel : public MemoryOwner
{
private:
  typedef MemoryOwner               tBase;
  typedef MatrixModel<tElementType> tThis;

public:
  //---------------------------------------------MatrixModel
  MatrixModel() 
    : m_uNumRows(0),
      m_uNumCols(0),
      m_uNumReservedRows(0),
      m_uNumReservedCols(0)
  {
    tBase::init();
  }

  //---------------------------------------------MatrixModel
  MatrixModel(unsigned int uNumRows, unsigned int uNumCols)
    : m_uNumRows(0),
      m_uNumCols(0),
      m_uNumReservedRows(0),
      m_uNumReservedCols(0)
  {
    tBase::init();
    this->resize(uNumRows, uNumCols);
  }

  //---------------------------------------------~MatrixModel
  virtual ~MatrixModel()
  {
    tBase::deInit();
  }

  //---------------------------------------------operator=
  const tThis& operator=(const tElementType& a)
  {
    if(m_uNumReservedCols == m_uNumCols)
    {
      tBase::set(0, a, m_uNumRows * m_uNumCols);
    }
    else 
    {
      CLAY_ASSERT(m_uNumReservedCols > m_uNumCols);
      for(unsigned int i=0; i<m_uNumRows; ++i)
      {
        tBase::set(i*m_uNumReservedCols*s_uElementSize, a, m_uNumCols);
      }
    }
    return *this;
  }

  //---------------------------------------------insert
  const tThis& insert(const tThis& aRhs)
  {
    //copy the intersection of 'this' and rhs matrix to 'this'
    unsigned int uNumCopyRows = std::min(m_uNumRows, aRhs.m_uNumRows);
    unsigned int uNumCopyCols = std::min(m_uNumCols, aRhs.m_uNumCols);
    for(unsigned int i=0; i<uNumCopyRows; ++i)
    {
      this->setRow(i, aRhs.getRow(i), uNumCopyCols);
    }
    return *this;
  }

  //---------------------------------------------subMatrix
  tThis subMatrix(unsigned int uStartRow, unsigned int uNumRows, 
                  unsigned int uStartCol, unsigned int uNumCols)
  {
    tThis aSubmatrix;
    aSubmatrix.share(*this, uStartRow, uNumRows, uStartCol, uNumCols); 
    
    //TODO: only share the specified rows
    aSubmatrix.m_uNumRows = uNumRows;
    aSubmatrix.m_uNumCols = uNumCols;

    aSubmatrix.m_uNumReservedRows = m_uNumReservedRows;
    aSubmatrix.m_uNumReservedCols = m_uNumReservedCols;

    return aSubmatrix;
  }

  //---------------------------------------------transpose
  void transpose()
  {
    if(m_uNumCols == m_uNumRows) //optimzation for symmetric matrices
    {
      for(unsigned int i=0; i<m_uNumRows; ++i)
      {
        for(unsigned int j=i+1; j<m_uNumCols; ++j)
        {
          tElementType v = getElement(i, j);
          setElement(i, j, getElement(j, i));
          setElement(j, i, v); //swap element-wise
        }
      }
    }
    else
    {
      tThis aTransposed(m_uNumCols, m_uNumRows);
      for(unsigned int i=0, uNumRows=aTransposed.getNumRows(); i<uNumRows; ++i)
      {
        for(unsigned int j=0, uNumCols=aTransposed.getNumCols(); j<uNumCols; ++j)
        {
          aTransposed.setElement(i, j, getElement(j, i));
        }
      }
      adopt(aTransposed);
    }
  }

  //---------------------------------------------resize
  void resize(unsigned int uNumRows, unsigned int uNumCols)
  {
    this->resizeImpl(uNumRows, uNumCols, NULL); //no init-value specified
  }

  //---------------------------------------------resize
  void resize(unsigned int uNumRows, unsigned int uNumCols, const tElementType& aInitValue)
  {
    this->resizeImpl(uNumRows, uNumCols, &aInitValue);
  }

  //---------------------------------------------reserve
  void reserve(unsigned int uNumRows, unsigned int uNumCols)
  {
    if(uNumRows * uNumCols > m_uNumReservedCols * m_uNumReservedRows)
    {
      if(m_uNumRows && m_uNumCols) //the matrix already contains elements
      {
        tThis aMatrix;
        aMatrix.reserve(uNumRows, uNumCols);
        aMatrix.resize(this->getNumRows(), this->getNumCols());
        aMatrix.insert(*this);
        this->adopt(aMatrix); //give the responsibility of the temp matrix'memory to 'this' matrix
      }
      else //reserve is called on an empty matrix - i.e. nothing to copy
      {
        tBase::reserve(uNumRows * uNumCols * s_uElementSize);

        m_uNumReservedCols = uNumRows;
        m_uNumReservedCols = uNumCols;
      }
    }
  }

  //---------------------------------------------setData
  void setData(const tElementType* pSrc, unsigned int uNumRows, unsigned int uNumCols)
  {
    if(m_uNumRows * m_uNumCols < uNumRows * uNumCols)
    { 
      resize(uNumRows, uNumCols);
    }

    m_uNumRows = uNumRows;
    m_uNumCols = uNumCols;

    tBase::set(pSrc, uNumRows* uNumCols);
  }

  //---------------------------------------------setElement
  void setElement(unsigned int uRow, unsigned int uCol, const tElementType& aValue)
  {
    CLAY_ASSERT(uRow < m_uNumRows);
    CLAY_ASSERT(uCol < m_uNumCols);
    tBase::set((uRow * m_uNumReservedCols + uCol) * s_uElementSize,
               reinterpret_cast<const tBase::tDataType*>(&aValue), 
               s_uElementSize);
  }

  //---------------------------------------------getElement
  const tElementType& getElement(unsigned int uRow, unsigned int uCol) const
  {
    CLAY_ASSERT(uRow < m_uNumRows);
    CLAY_ASSERT(uCol < m_uNumCols);
    return *reinterpret_cast<const tElementType*>(tBase::get((uRow * m_uNumReservedCols + uCol) * s_uElementSize));
  }

  //---------------------------------------------setNumRows
  void setNumRows(unsigned int uNumRows)
  {
    CLAY_ASSERT(m_uNumReservedRows >= uNumRows);
    m_uNumRows = uNumRows;
  }

  //---------------------------------------------setNumCols
  void setNumCols(unsigned int uNumCols)
  {
    CLAY_ASSERT(m_uNumReservedCols >= uNumCols);
    m_uNumCols = uNumCols;
  }

  //---------------------------------------------getNumRows
  unsigned int getNumRows() const
  {
    return m_uNumRows;
  }

  //---------------------------------------------getNumCols
  unsigned int getNumCols() const
  {
    return m_uNumCols;
  }

  //---------------------------------------------getRow
  const tElementType* getRow(unsigned int uRow) const
  {
    CLAY_ASSERT(m_uNumRows > uRow);
    return reinterpret_cast<const tElementType*>(tBase::getMemoryPtr() + uRow * m_uNumReservedCols * s_uElementSize);
  }
    
  //---------------------------------------------setRow
  void setRow(unsigned int uRow, const tElementType* src)
  {
    setRow(uRow, src, m_uNumCols);
  }

  //---------------------------------------------setRow
  void setRow(unsigned int uRow, const tElementType* src, unsigned int uNumElements)
  {
    CLAY_ASSERT(m_uNumRows > uRow);
    tBase::copy(reinterpret_cast<const tBase::tDataType*>(src), uRow * m_uNumReservedCols * s_uElementSize, uNumElements * s_uElementSize);
  }

  //---------------------------------------------writeRowRaw
  bool writeRowRaw(unsigned int uRow, std::ostream& os)
  {
    os.write(reinterpret_cast<const char*>(getRow(uRow)), getNumCols() * s_uElementSize);
    return os.good();
  }

protected:
  //---------------------------------------------adopt
  void adopt(tThis& aAdoptee)
  {
    m_uNumRows = aAdoptee.m_uNumRows;
    m_uNumCols = aAdoptee.m_uNumCols;
    
    m_uNumReservedRows = aAdoptee.m_uNumReservedRows;
    m_uNumReservedCols = aAdoptee.m_uNumReservedCols;

    tBase::adopt(aAdoptee);
  }

  //---------------------------------------------share
  void share(tThis& aShared, unsigned int uStartRow, unsigned int uNumRows, unsigned int uStartCol, unsigned int uNumCols)
  {
    aShared.m_uNumReservedRows = m_uNumReservedRows;
    aShared.m_uNumReservedCols = m_uNumReservedCols;
    aShared.m_uNumRows = uNumRows;
    aShared.m_uNumCols = uNumCols;
    tBase::share(aShared, (uStartRow * m_uNumReservedCols + uStartCol) * s_uElementSize);
  }

  //---------------------------------------------resizeImpl
  void resizeImpl(unsigned int uNumRows, unsigned int uNumCols, const tElementType* pInitValue /*can be NULL*/) 
  {
    if(m_uNumRows == 0 && m_uNumCols == 0) //resizing an empty matrix - just allocate memory, i.e. nothing to copy
    {
      tBase::resize(uNumRows * uNumCols * s_uElementSize);

      m_uNumRows = uNumRows;
      m_uNumCols = uNumCols;

      if(m_uNumRows > m_uNumReservedRows)
      {
        m_uNumReservedRows = m_uNumRows;
      }

      if(m_uNumCols > m_uNumReservedCols)
      {
        m_uNumReservedCols = m_uNumCols;
      }

      if(pInitValue) //init the matrix if a value has been specified
      {
        (*this) = *pInitValue;
      }
    }
    else //resizing a matrix that already contains elements - keep existing elements
    {
      if(m_uNumReservedRows * m_uNumReservedCols >= uNumRows * uNumCols) //no reallocation needed - enough reserved memory 
      {
        unsigned int uNumOldRows = m_uNumRows;
        unsigned int uNumOldCols = m_uNumCols;
        unsigned int uNumNewRows = std::max(0, (int)(uNumRows - m_uNumRows));
        unsigned int uNumNewCols = std::max(0, (int)(uNumCols - m_uNumCols));
        
        m_uNumRows = uNumRows;
        m_uNumCols = uNumCols;

        if(pInitValue)
        {
          if(uNumNewRows) //init new rows
          {
            tBase::set(uNumOldRows * m_uNumReservedCols * s_uElementSize, //start-idx
                       *pInitValue,                                       //set value
                       uNumNewRows * m_uNumReservedCols);                 //num elements
          }

          if(uNumNewCols) //init new cols
          {
            tBase::set(uNumOldCols * s_uElementSize,                       //start idx
                       *pInitValue,                                        //set value
                       uNumNewCols,                                        //num elements
                       (m_uNumReservedCols - uNumOldCols) + uNumOldCols,   //num skip 
                       uNumOldRows * m_uNumReservedCols * s_uElementSize); //end idx
          }
        }
      }
      else //there is not enough reserved space left - reallocation needed
      {
        tThis aMatrix(uNumRows, uNumCols);
        if(pInitValue) //set the initial value if given
        {
          aMatrix = *pInitValue;
        }
        aMatrix.insert(*this);
        this->adopt(aMatrix); //give the responsibility of the temp matrix'memory to 'this' matrix
      } 
    }
  }

private:
  static const unsigned int s_uElementSize = sizeof(tElementType);

  unsigned int m_uNumRows;
  unsigned int m_uNumCols;

  unsigned int m_uNumReservedRows;
  unsigned int m_uNumReservedCols;
};

} }

#endif
