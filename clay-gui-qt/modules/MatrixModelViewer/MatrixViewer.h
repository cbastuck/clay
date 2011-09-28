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

#ifndef MATRIX_VIEWER_H_
#define MATRIX_VIEWER_H_

#include <clay-data-types/base/Matrix.h>
#include <clay-data-types/base/VectorOp.h>

//QT
#include <QWidget>
#include <QPainter>
#include <QApplication>

//STL
#include <limits>

template<class T>
class MatrixViewer : public QWidget
{
private:
  typedef QWidget                         tBase;
  typedef typename CLAY::MODEL::Matrix<T> tMatrix;
  typedef typename tMatrix::tVector       tVector;
  typedef typename tMatrix::tConstVector  tConstVector;

public:
  MatrixViewer();
  MatrixViewer(QWidget* pParent);
  ~MatrixViewer();

  void plot(const tMatrix& aMatrix);
  void imagesc(const tMatrix& aMatrix);
  void bars(const tMatrix& aMatrix);
  void barsct(const tMatrix& aMatrix);

  virtual void paintEvent(QPaintEvent*);
  QColor getColor(unsigned int uIdx);
  
protected:
  void plot();
  void imagesc();
  void bars();
  void barsct();

  T getMatrixValue(unsigned int uRow, unsigned int uStartCol, unsigned int uNum);
  T getMatrixValue(unsigned int uStartRow, unsigned int uNumRows, unsigned int uStartCol, unsigned int uNumCols);

  void setMatrix(const tMatrix& aMatrix);

  //members
  enum DrawMode
  {  
    PLOT    = 0,
    IMAGESC = 1,
    BARS    = 2,
    BARSCT  = 4
  } m_eDrawMode;

  const tMatrix* m_pMatrix;
  T              m_aMinValue;
  T              m_aValueRange;
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------MatrixViewer
template<class T>
inline MatrixViewer<T>::MatrixViewer()
  : m_pMatrix(NULL),
    m_eDrawMode(PLOT),
    m_aMinValue(std::numeric_limits<T>::max()),
    m_aValueRange(T(0))
{
}

//---------------------------------------------MatrixViewer
template<class T>
MatrixViewer<T>::MatrixViewer(QWidget* pParent)
  : tBase(pParent),
    m_pMatrix(NULL),
    m_eDrawMode(PLOT),
    m_aMinValue(std::numeric_limits<T>::max()),
    m_aValueRange(T(0))
{
}

//---------------------------------------------~MatrixViewer
template<class T>
inline MatrixViewer<T>::~MatrixViewer()
{
}


//---------------------------------------------plot
template<class T>
inline void MatrixViewer<T>::plot(const tMatrix& aMatrix)
{
  setMatrix(aMatrix);
  m_eDrawMode = PLOT;
  tBase::show();
}

//---------------------------------------------imagesc
template<class T>
inline void MatrixViewer<T>::imagesc(const tMatrix& aMatrix)
{
  setMatrix(aMatrix);
  m_eDrawMode = IMAGESC;
  tBase::show();
}

//---------------------------------------------bars
template<class T>
inline void MatrixViewer<T>::bars(const tMatrix& aMatrix)
{
  setMatrix(aMatrix);
  m_eDrawMode = BARS;
  tBase::show();
}

//---------------------------------------------barsct
template<class T>
inline void MatrixViewer<T>::barsct(const tMatrix& aMatrix)
{
  setMatrix(aMatrix);
  m_eDrawMode = BARSCT;
  tBase::show();
}  

//---------------------------------------------paintEvent
template<class T>
inline void MatrixViewer<T>::paintEvent(QPaintEvent*)
{
  if(m_pMatrix && m_pMatrix->getNumColumns() && m_pMatrix->getNumRows())
  {
    if(m_eDrawMode == PLOT)
    {
      plot();
    }
    else if(m_eDrawMode == IMAGESC)
    {
      imagesc();
    } 
    else if(m_eDrawMode == BARS)
    {
      bars();
    }
    else if(m_eDrawMode == BARSCT)
    {
      barsct();
    }
  }
}

//---------------------------------------------getColor
template<class T>
inline QColor MatrixViewer<T>::getColor(unsigned int uIdx)
{
  const unsigned int n = 7;
  QColor aTable[n] = { 
    QColor(0, 0, 0),
    QColor(255, 0, 0),
    QColor(0, 255, 0),
    QColor(0, 0, 255),
    QColor(0, 255, 255),
    QColor(255, 0, 255),
    QColor(255, 255, 0)};

  return aTable[uIdx % n];
}

//---------------------------------------------plot
template<class T>
inline void MatrixViewer<T>::plot()
{
  QPainter aPainter(this);

  QRect aRect = rect();
  unsigned int uWidth  = aRect.width();
  unsigned int uHeight = aRect.height();

  QBrush aBrush(QColor(255, 255, 255));
  aPainter.fillRect(aRect, aBrush);

  unsigned int uNumRows = m_pMatrix->getNumRows();
  unsigned int uNumCols = m_pMatrix->getNumColumns();
  if(uNumCols < 2)
  {
    return; //can not plot one single value - need at least two
  }

  float fElemWidth = (float)(uWidth-1) / (float)(uNumCols-1);
  float fStepSize = 1.f;
  if(fElemWidth < 1.f) //we have less pixels than elements
  {
    fStepSize = (float)uNumCols / (float)uWidth;
    fElemWidth = 1.f;
  }

  //pixels and matrix indices are discrete so we accumulate integers ....
  unsigned int uStepSize  = static_cast<unsigned int>(floor(fStepSize));
  unsigned int uElemWidth = static_cast<unsigned int>(floor(fElemWidth));

  //... so every iteration we make an error (i.e. the fractional part)
  float fIterationIdxError   = (fStepSize  - (unsigned int)fStepSize); 
  float fIterationPixelError = (fElemWidth - (unsigned int)fElemWidth);

  for(unsigned int i=0, n=uNumRows; i<n; ++i)
  {
    QPen aPen(QColor(getColor(i)));
    aPainter.setPen(aPen);

    //compute the first point
    T v0 = getMatrixValue(i, 0, uStepSize);
    v0 = (v0 - m_aMinValue) / m_aValueRange;
    unsigned int y0 = uHeight - (abs(v0) * uHeight);
    QPoint p0(0, y0);

    //compute the following points
    unsigned int uCurrentCol   = uStepSize; //start after the first point
    float fIdxBresenhamErr     = fIterationIdxError; //the first 'iteration' was for p0
    float fPixelBresenhamErr   = fIterationPixelError;
    for(unsigned int x=uElemWidth; x<uWidth; x+=uElemWidth, uCurrentCol+=uStepSize)
    {
      //compensate the error due to accumulation of matrix indices
      unsigned int uCorrectionIdx = 0;
      fIdxBresenhamErr += fIterationIdxError;
      if(fIdxBresenhamErr >= 1.f)
      {
        uCorrectionIdx = 1;
        fIdxBresenhamErr -= 1.f;
      }

      T v = getMatrixValue(i, uCurrentCol, uStepSize + uCorrectionIdx);
      v = (v - m_aMinValue) / m_aValueRange;
      unsigned int y = uHeight - (abs(v) * uHeight);
      uCurrentCol += uCorrectionIdx;

      //compensate the error due to accumulation of pixels
      fPixelBresenhamErr += fIterationPixelError;
      if(fPixelBresenhamErr >= 1.f)
      {
        ++x;
        fPixelBresenhamErr -= 1.f;
      }

      //draw a line from p0 to p1
      QPoint p1(x, y);
      aPainter.drawLine(p0, p1);  
      p0 = p1; //set for next iteration
    }
  }
}
  
//---------------------------------------------imagesc
template<class T>
inline void MatrixViewer<T>::imagesc()
{
  QPainter aPainter(this);

  QRect aRect = rect();
  unsigned int uWidth  = aRect.width();
  unsigned int uHeight = aRect.height();

  QBrush aBrush(QColor(255, 255, 255));
  aPainter.fillRect(aRect, aBrush);
  aPainter.setPen(QColor(0, 0, 0));

  unsigned int uNumRows = m_pMatrix->getNumRows();
  unsigned int uNumCols = m_pMatrix->getNumColumns();

  float fElemWidth  = (float)(uWidth)  / (float)(uNumCols);
  float fElemHeight = (float)(uHeight) / (float)(uNumRows);
  float fStepSizeX = 1.f;
  float fStepSizeY = 1.f;
  if(fElemWidth < 1.f) //we have less pixels than elements
  {
    fStepSizeX = (float)uNumCols / (float)uWidth;
    fElemWidth = 1.f;
  }
  if(fElemHeight < 1.f)
  {
    fStepSizeY = (float)uNumRows / (float)uHeight;
    fElemHeight = 1.f;
  }

  //pixels and matrix indices are discrete so we accumulate integers ....
  unsigned int uStepSizeX  = static_cast<unsigned int>(floor(fStepSizeX));
  unsigned int uStepSizeY  = static_cast<unsigned int>(floor(fStepSizeY));
  unsigned int uElemWidth  = static_cast<unsigned int>(floor(fElemWidth));
  unsigned int uElemHeight = static_cast<unsigned int>(floor(fElemHeight));

  //... so every iteration we make an error (i.e. the fractional part)
  float fIterationIdxErrorX   = (fStepSizeX  - (unsigned int)fStepSizeX); 
  float fIterationIdxErrorY   = (fStepSizeY  - (unsigned int)fStepSizeY); 
  float fIterationPixelErrorX = (fElemWidth  - (unsigned int)fElemWidth);
  float fIterationPixelErrorY = (fElemHeight - (unsigned int)fElemHeight);

  unsigned int uCurrentRow  = 0;
  float fIdxBresenhamErrY   = 0.f; 
  float fPixelBresenhamErrY = 0.f;
  for(unsigned int y=0, i=0; y<uHeight; y+=uElemHeight, uCurrentRow+=uStepSizeY, ++i)
  {
    //compensate the error due to accumulation of matrix indices
    unsigned int uCorrectionIdxY = 0;
    fIdxBresenhamErrY += fIterationIdxErrorY;
    if(fIdxBresenhamErrY >= 1.f)
    {
      uCorrectionIdxY = 1;
      fIdxBresenhamErrY -= 1.f;
    }

    unsigned int uCurrentCol  = 0; 
    float fIdxBresenhamErrX   = 0.f; 
    float fPixelBresenhamErrX = 0.f;
    for(unsigned int x=0, j=0; x<uWidth; x+=uElemWidth, uCurrentCol+=uStepSizeX, ++j)
    {
      //compensate the error due to accumulation of matrix indices
      unsigned int uCorrectionIdxX = 0;
      fIdxBresenhamErrX += fIterationIdxErrorX;
      if(fIdxBresenhamErrX >= 1.f)
      {
        uCorrectionIdxX = 1;
        fIdxBresenhamErrX -= 1.f;
      }

      //get the matrix value for the given rect
      T v = getMatrixValue(uCurrentRow, uStepSizeY + uCorrectionIdxY, uCurrentCol, uStepSizeX + uCorrectionIdxX);
      float fVal  = ((v - m_aMinValue) / m_aValueRange);
      QColor aCol;
      aCol.setHsvF(fVal, 1.f, 1.f);
      aPainter.setBrush(aCol);
      aPainter.setPen(aCol);
      uCurrentCol += uCorrectionIdxX;

      //compensate the error due to accumulation of pixels
      fPixelBresenhamErrX += fIterationPixelErrorX;
      float fThresh = 0.9999f;
      if(fPixelBresenhamErrX >= fThresh)
      {
        ++x;
        fPixelBresenhamErrX -= fThresh;
      }

      aPainter.drawRect(x, y, uElemWidth, uElemHeight);  //draw a color encoded rect
    }
    uCurrentRow += uCorrectionIdxY;

    //compensate the error due to accumulation of pixels
    fPixelBresenhamErrY += fIterationPixelErrorY;
    float fThresh = 0.9999f;
    if(fPixelBresenhamErrY >= fThresh)
    {
      ++y;
      fPixelBresenhamErrY -= fThresh;
    }
  }
}

//---------------------------------------------bars
template<class T>
inline void MatrixViewer<T>::bars()
{
  QPainter aPainter(this);

  QRect aRect = rect();
  unsigned int uWidth  = aRect.width();
  unsigned int uHeight = aRect.height();

  QBrush aBrush(QColor(255, 255, 255));
  aPainter.fillRect(aRect, aBrush);
  aPainter.setPen(QColor(0, 0, 0));

  unsigned int uNumRows = m_pMatrix->getNumRows();
  unsigned int uNumCols = m_pMatrix->getNumColumns();

  float fElemWidth = (float)(uWidth) / (float)(uNumCols);
  float fStepSize = 1.f;
  if(fElemWidth < 1.f) //we have less pixels than elements
  {
    fStepSize = (float)uNumCols / (float)uWidth;
    fElemWidth = 1.f;
  }

  //pixels and matrix indices are discrete so we accumulate integers ....
  unsigned int uStepSize  = static_cast<unsigned int>(floor(fStepSize));
  unsigned int uElemWidth = static_cast<unsigned int>(floor(fElemWidth));

  //... so every iteration we make an error (i.e. the fractional part)
  float fIterationIdxError   = (fStepSize  - (unsigned int)fStepSize); 
  float fIterationPixelError = (fElemWidth - (unsigned int)fElemWidth);

  for(unsigned int i=0, n=uNumRows; i<n; ++i)
  {
    //compute the following points
    unsigned int uCurrentCol   = 0; 
    float fIdxBresenhamErr     = 0.f; 
    float fPixelBresenhamErr   = 0.f;
    for(unsigned int x=0, j=0; x<uWidth; x+=uElemWidth, uCurrentCol+=uStepSize, ++j)
    {
      aPainter.setBrush(getColor(j));

      //compensate the error due to accumulation of matrix indices
      unsigned int uCorrectionIdx = 0;
      fIdxBresenhamErr += fIterationIdxError;
      if(fIdxBresenhamErr >= 1.f)
      {
        uCorrectionIdx = 1;
        fIdxBresenhamErr -= 1.f;
      }

      T v = getMatrixValue(i, uCurrentCol, uStepSize + uCorrectionIdx);
      unsigned int y = ((v - m_aMinValue) / m_aValueRange) * uHeight;
      uCurrentCol += uCorrectionIdx;

      //compensate the error due to accumulation of pixels
      fPixelBresenhamErr += fIterationPixelError;
      float fThresh = 0.9999f;
      if(fPixelBresenhamErr >= fThresh)
      {
        ++x;
        fPixelBresenhamErr -= fThresh;
      }

      aPainter.drawRect(x, uHeight-y, uElemWidth, y);  
    }
  }
}

//---------------------------------------------barsct
template<class T>
inline void MatrixViewer<T>::barsct()
{
  QPainter aPainter(this);

  QRect aRect = rect();
  unsigned int uWidth  = aRect.width();
  unsigned int uHeight = aRect.height();
  unsigned int uMid    = uHeight / 2;    

  QBrush aBrush(QColor(255, 255, 255));
  aPainter.fillRect(aRect, aBrush);
  aPainter.setPen(QColor(0, 0, 0));

  unsigned int uNumRows = m_pMatrix->getNumRows();
  unsigned int uNumCols = m_pMatrix->getNumColumns();

  float fElemWidth = (float)(uWidth) / (float)(uNumCols);
  float fStepSize = 1.f;
  if(fElemWidth < 1.f) //we have less pixels than elements
  {
    fStepSize = (float)uNumCols / (float)uWidth;
    fElemWidth = 1.f;
  }

  //pixels and matrix indices are discrete so we accumulate integers ....
  unsigned int uStepSize  = static_cast<unsigned int>(floor(fStepSize));
  unsigned int uElemWidth = static_cast<unsigned int>(floor(fElemWidth));

  //... so every iteration we make an error (i.e. the fractional part)
  float fIterationIdxError   = (fStepSize  - (unsigned int)fStepSize); 
  float fIterationPixelError = (fElemWidth - (unsigned int)fElemWidth);

  for(unsigned int i=0, n=uNumRows; i<n; ++i)
  {
    //compute the following points
    unsigned int uCurrentCol   = 0; 
    float fIdxBresenhamErr     = 0.f; 
    float fPixelBresenhamErr   = 0.f;
    for(unsigned int x=0, j=0; x<uWidth; x+=uElemWidth, uCurrentCol+=uStepSize, ++j)
    {
      aPainter.setBrush(getColor(j));

      //compensate the error due to accumulation of matrix indices
      unsigned int uCorrectionIdx = 0;
      fIdxBresenhamErr += fIterationIdxError;
      if(fIdxBresenhamErr >= 1.f)
      {
        uCorrectionIdx = 1;
        fIdxBresenhamErr -= 1.f;
      }

      T v = getMatrixValue(i, uCurrentCol, uStepSize + uCorrectionIdx);
      unsigned int y = ((v - m_aMinValue) / m_aValueRange) * uHeight;
      uCurrentCol += uCorrectionIdx;

      //compensate the error due to accumulation of pixels
      fPixelBresenhamErr += fIterationPixelError;
      float fThresh = 0.9999f;
      if(fPixelBresenhamErr >= fThresh)
      {
        ++x;
        fPixelBresenhamErr -= fThresh;
      }

      aPainter.drawRect(x, uMid-y/2, uElemWidth, y);  
    }
  }
}

//---------------------------------------------getMatrixValue
template<class T>
inline T MatrixViewer<T>::getMatrixValue(unsigned int uRow, unsigned int uStartCol, unsigned int uNum)
{
  CLAY_ASSERT(m_pMatrix->getNumColumns() > uStartCol);
  CLAY_ASSERT(m_pMatrix->getNumColumns() >= uStartCol+uNum);

  if(uNum == 1) //only one value, i.e. nothing to accumulate
  {
    return (*m_pMatrix)(uRow, uStartCol);
  }
  tConstVector aRow = m_pMatrix->row(uRow)->subvector(uStartCol, uNum);
  return max(aRow());
}

//---------------------------------------------getMatrixValue
template<class T>
inline T MatrixViewer<T>::getMatrixValue(unsigned int uStartRow, unsigned int uNumRows, unsigned int uStartCol, unsigned int uNumCols)
{
  CLAY_ASSERT(m_pMatrix->getNumRows() >  uStartRow);
  CLAY_ASSERT(m_pMatrix->getNumRows() >= uStartRow+uNumRows);
  CLAY_ASSERT(m_pMatrix->getNumColumns() >  uStartCol);
  CLAY_ASSERT(m_pMatrix->getNumColumns() >= uStartCol+uNumCols);
    
  if(uNumRows == 1 && uNumCols == 1)
  {
    return (*m_pMatrix)(uStartRow, uStartCol);
  }

  T aMaxValue = (*m_pMatrix)(uStartRow, uStartCol);
  for(unsigned int i=0; i<uNumRows; ++i)
  {
    tConstVector aRow = m_pMatrix->row(i);
    aMaxValue = std::max(max(aRow->subvector(uStartCol, uNumCols)()), aMaxValue);
  }
  return aMaxValue;
}

//---------------------------------------------setMatrix
template<class T>
inline void MatrixViewer<T>::setMatrix(const tMatrix& aMatrix)
{
  if(aMatrix.getNumColumns() && aMatrix.getNumRows())
  {
    m_pMatrix = &aMatrix;

    T aMaxValue = aMatrix(0, 0); 
    unsigned int uNumCols = m_pMatrix->getNumColumns();
    for(unsigned int i=0, n=m_pMatrix->getNumRows(); i<n; ++i)
    {
      tConstVector aRow = m_pMatrix->row(i);
      m_aMinValue = std::min(min(aRow()), m_aMinValue);
      aMaxValue   = std::max(max(aRow()), aMaxValue);
    }

    m_aValueRange = aMaxValue - m_aMinValue;
  }
}

//---------------------------------------------
//-------------  convenience  ----------------
//---------------------------------------------

namespace DETAIL
{
  inline QApplication& getQApplication()
  {
    int argc = 0;
    char** argv = NULL;
    static QApplication aApp(argc, argv);
    return aApp;
  }
}

template<class T>
inline void plot(const CLAY::MODEL::Matrix<T>& a)
{
  QApplication& aApp = DETAIL::getQApplication();
  MatrixViewer<T> aViewer;
  aViewer.plot(a);
  
  aApp.exec();
}

template<class T>
inline void imagesc(const CLAY::MODEL::Matrix<T>& a)
{
  QApplication& aApp = DETAIL::getQApplication();
  MatrixViewer<T> aViewer;
  aViewer.imagesc(a);
  
  aApp.exec();
}

template<class T>
inline void bars(const CLAY::MODEL::Matrix<T>& a)
{
  QApplication& aApp = DETAIL::getQApplication();
  MatrixViewer<T> aViewer;
  aViewer.bars(a);
  
  aApp.exec();
}

template<class T>
inline void barsct(const CLAY::MODEL::Matrix<T>& a)
{
  QApplication& aApp = DETAIL::getQApplication();
  MatrixViewer<T> aViewer;
  aViewer.barsct(a);
  
  aApp.exec();
}

#endif 
