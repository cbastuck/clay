#include <clay-core/base/ClayDefines.h>

#include <clay-data-types/base/Matrix.h>
#include <clay-data-types/base/SharedData.h>

//---------------------------------------------
template<class T0, class T1>
bool checkMatrix(const T0& m, const T1& p, unsigned int uNumRows, unsigned int uNumCols, unsigned int uOffsetRows, unsigned int uOffsetCols)
{
  for(unsigned int i=0; i<uNumRows; ++i)
  {
    for(unsigned int j=0; j<uNumCols; ++j)
    {
      CLAY_ASSERT(m(i,j) == p[i+uOffsetRows][j+uOffsetCols]);
      if(m(i,j) != p[i+uOffsetRows][j+uOffsetCols])
      {
        return false;
      }
    }
  }
  return true;
}

//---------------------------------------------
template<class T0, class T1>
bool checkMatrix(const T0& m, const T1& p, unsigned int uNumRows, unsigned int uNumCols)
{
  for(unsigned int i=0; i<uNumRows; ++i)
  {
    for(unsigned int j=0; j<uNumCols; ++j)
    {
      CLAY_ASSERT(m(i,j) == p[i][j]);
      if(m(i,j) != p[i][j])
      {
        return false;
      }
    }
  }
  return true;
}

//---------------------------------------------
template<class T0, class T1>
bool checkVector(const T0& v, const T1& p, unsigned int uStep)
{
  for(unsigned int i=0, n=v.size(); i<n; ++i)
  {
    CLAY_ASSERT(v(i) == p[uStep*i]);
    if(v(i) != p[uStep*i])
    {
      return false;
    }
  }
  return true;
}

//---------------------------------------------
void submatCheck()
{
  float ptr[4][5] = { {0.f, 0.1f, 0.2f, 0.3f, 0.4f},
                      {1.f, 1.1f, 1.2f, 1.3f, 1.4f},
                      {2.f, 2.1f, 2.2f, 2.3f, 2.4f},
                      {3.f, 3.1f, 3.2f, 3.3f, 3.4f} };

  for(unsigned int i=0; i<4; ++i)
  {
    for(unsigned int j=0; j<5; ++j)
    {
      CLAY::MODEL::Matrix<float> m1 = CLAY::MODEL::Matrix<float>::create<4, 5>(ptr);

      CLAY::MODEL::Matrix<float> s1 = m1.submatrix(i, 4-i, j, 5-j);

      CLAY_ASSERT(s1.getNumRows()    == 4-i);
      CLAY_ASSERT(s1.getNumColumns() == 5-j);
      CLAY_ASSERT(checkMatrix(s1, ptr, 4-i, 5-j, i, j));
    }
  }
}

//---------------------------------------------
void rowCheck()
{
  float ptr[4][5] = { {0.f, 0.1f, 0.2f, 0.3f, 0.4f},
                      {1.f, 1.1f, 1.2f, 1.3f, 1.4f},
                      {2.f, 2.1f, 2.2f, 2.3f, 2.4f},
                      {3.f, 3.1f, 3.2f, 3.3f, 3.4f} };

  CLAY::MODEL::Matrix<float> m1 = CLAY::MODEL::Matrix<float>::create<4, 5>(ptr);

  for(unsigned int i=0; i<4; ++i)
  {
    CLAY::MODEL::Vector<float> v = m1.row(i);
    
    CLAY_ASSERT(v.size() == m1.getNumColumns());
    CLAY_ASSERT(checkVector(v, ptr[i], 1));
  }
}

//---------------------------------------------
void columnCheck()
{
  float ptr[4][5] = { {0.f, 0.1f, 0.2f, 0.3f, 0.4f},
                      {1.f, 1.1f, 1.2f, 1.3f, 1.4f},
                      {2.f, 2.1f, 2.2f, 2.3f, 2.4f},
                      {3.f, 3.1f, 3.2f, 3.3f, 3.4f} };

  CLAY::MODEL::Matrix<float> m1 = CLAY::MODEL::Matrix<float>::create<4, 5>(ptr);

  for(unsigned int i=0; i<5; ++i)
  {
    CLAY::MODEL::Vector<float> v = m1.column(i);

    CLAY_ASSERT(v.size() == m1.getNumRows());
    CLAY_ASSERT(checkVector(v, ptr[0]+i, 5));
  }
}

//---------------------------------------------
void matResizeCheck()
{
  float ptr[4][5] = { {0.f, 0.1f, 0.2f, 0.3f, 0.4f},
                      {1.f, 1.1f, 1.2f, 1.3f, 1.4f},
                      {2.f, 2.1f, 2.2f, 2.3f, 2.4f},
                      {3.f, 3.1f, 3.2f, 3.3f, 3.4f} };

  for(unsigned int i=1; i<10; ++i)
  {
    for(unsigned int j=1; j<10; ++j)
    {
      CLAY::MODEL::Matrix<float> m1 = CLAY::MODEL::Matrix<float>::create<4, 5>(ptr);
      m1.resize(i, j);
      CLAY_ASSERT(m1.getNumRows() == i);
      CLAY_ASSERT(m1.getNumColumns() == j);
      CLAY_ASSERT(checkMatrix(m1, ptr, std::min(i, 4u), std::min(j, 5u)));
    }
  }
}

//---------------------------------------------
void matAssignCheck()
{
  float ptr[4][5] = { {0.f, 0.1f, 0.2f, 0.3f, 0.4f},
                      {1.f, 1.1f, 1.2f, 1.3f, 1.4f},
                      {2.f, 2.1f, 2.2f, 2.3f, 2.4f},
                      {3.f, 3.1f, 3.2f, 3.3f, 3.4f} };

  CLAY::MODEL::Matrix<float> m1 = CLAY::MODEL::Matrix<float>::create<4, 5>(ptr);
  checkMatrix(m1, ptr, 4, 5);
}

//---------------------------------------------
int main(int argc, char** argv)
{
  matAssignCheck();
  matResizeCheck();
  columnCheck();
  rowCheck();
  submatCheck();
  
  return 0;
}
