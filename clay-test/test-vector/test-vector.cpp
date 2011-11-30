#include <clay-core/base/ClayDefines.h>

#include <clay-data-types/base/Vector.h>
#include <clay-data-types/base/SharedData.h>

#include <vector>

//---------------------------------------------
class X
{
public:
  X() : m_pRef(NULL)
  {
    m_pRef = new CLAY::MODEL::SharedData<char*>(new char[1024]);
    m_pRef->inc();
  }

  ~X()
  {
    deref();
  }

  X(const X& rhs) : m_pRef(NULL)
  {
    operator=(rhs);
  }

  X& operator=(const X& rhs)
  {
    if(rhs.m_pRef != m_pRef)
    {
      deref();
      m_pRef = rhs.m_pRef;
      m_pRef->inc();
    }
    return *this;
  }

  void deref()
  {
    if(m_pRef && m_pRef->dec() == 0)
    {
      //cleanup
      delete[] m_pRef->get();
      delete m_pRef;
    }
  }

  void check(){ m_pRef->inc(); m_pRef->dec(); } //testing memory on heap

private:
  CLAY::MODEL::SharedData<char*>* m_pRef;
};

//---------------------------------------------
template<class T0, class T1>
bool isEqual(const T0& a, unsigned int an, const T1& b, unsigned int bn)
{
  if(an != bn)
  {
    return false;
  }

  for(unsigned int i=0; i<an; ++i)
  {
    if(a[i] != b[i])
    {
      return false;
    }
  }

  return true;
}

//---------------------------------------------
void subvectorCheck()
{
  float ptr[5] = {0.f, 1.f, 2.f, 3.f, 4.f };
  CLAY::MODEL::Vector<float> v1(ptr, 1, 5);
  {
    for(unsigned int i=0; i<5; ++i)
    {
      unsigned int uNum = 5-i;
      CLAY::MODEL::Vector<float> v2 = v1.subvector(i, uNum);
      CLAY_ASSERT(isEqual(v2, uNum, ptr+i, uNum));
      CLAY_ASSERT(v2.size() == uNum);
    }
  }

  CLAY_ASSERT(ptr[0] == 0.f);
  CLAY_ASSERT(ptr[1] == 1.f);
  CLAY_ASSERT(ptr[2] == 2.f);
  CLAY_ASSERT(ptr[3] == 3.f);
  CLAY_ASSERT(ptr[4] == 4.f);
}

//---------------------------------------------
void resizeCheck()
{
  float ptr[5] = {0.f, 1.f, 2.f, 3.f, 4.f };

  //grow
  {
    CLAY::MODEL::Vector<float> v1(ptr, 1, 5);
    v1.resize(10);
    CLAY_ASSERT(isEqual(v1, 5, ptr, 5));
    CLAY_ASSERT(v1.size() == 10);
  }


  //shrink
  {
    CLAY::MODEL::Vector<float> v1(ptr, 1, 5);
    v1.resize(2);
    CLAY_ASSERT(isEqual(v1, 2, ptr, 2));
    CLAY_ASSERT(v1.size() == 2);
  }

  CLAY_ASSERT(ptr[0] == 0.f);
  CLAY_ASSERT(ptr[1] == 1.f);
  CLAY_ASSERT(ptr[2] == 2.f);
  CLAY_ASSERT(ptr[3] == 3.f);
  CLAY_ASSERT(ptr[4] == 4.f);
}

//---------------------------------------------
void vecCheck()
{
  float ptr[5] = {0.f, 1.f, 2.f, 3.f, 4.f };
  {
    CLAY::MODEL::Vector<float> v1(ptr, 1, 5);
  
    {
      CLAY::MODEL::Vector<float> v2 = v1;
      CLAY_ASSERT(isEqual(v1, v1.size(), v2, v2.size()));
    }

    CLAY_ASSERT(isEqual(v1, v1.size(), ptr, 5));
  }

  CLAY_ASSERT(ptr[0] == 0.f);
  CLAY_ASSERT(ptr[1] == 1.f);
  CLAY_ASSERT(ptr[2] == 2.f);
  CLAY_ASSERT(ptr[3] == 3.f);
  CLAY_ASSERT(ptr[4] == 4.f);
}

//---------------------------------------------
int main(int argc, char** argv)
{
  vecCheck();
  resizeCheck();
  subvectorCheck();

  {
    X x1;
    {
      X x2 = x1;
      x1 = x2;
      
      X x3;
      x3 = x2;
    }
    x1;
  }

  {
    std::vector<X> aVec;
    for(unsigned int i=0; i<100; ++i)
    {
      aVec.push_back(X());
    }

    std::vector<X>::iterator it = aVec.begin();
    std::vector<X>::iterator ed = aVec.end();
    for(; it!=ed; ++it)
    {
      it->check();
    }
  }


  {
    CLAY::MODEL::Vector<float> v1(10);

    CLAY::MODEL::Vector<float> v2 = v1;

    CLAY::MODEL::Vector<float> v3;
    
    v3 = v2;
  }
  return 0;
}
