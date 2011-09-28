#ifndef CLAY_CONST_H_
#define CLAY_CONST_H_

#include <clay-core/base/ClayDefines.h>

namespace CLAY{ namespace MODEL{

template<class T>
class Const
{
public:
  Const(T& t) : m_aInstance(t){}
  const T& get()       { return m_aInstance;  }
  const T* operator->(){ return &m_aInstance; }
  const T& operator()(){ return m_aInstance;  }
  operator const T&()  { return m_aInstance;  }

private:
  T m_aInstance;
};

} }

#endif
