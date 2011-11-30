#ifndef SHARED_DATA_H_
#define SHARED_DATA_H_

namespace CLAY{ namespace MODEL{

template<class T>
class SharedData
{
public:
  SharedData();
  SharedData(T data);
  ~SharedData();

  void inc();
  unsigned int dec();
  
  T get() const;
  void set(T data);

  void setFlags(unsigned int uFlag);
  unsigned int getFlags() const;
  void addFlag(unsigned int uFlag);
  void removeFlag(unsigned int uFlag);
  bool testFlag(unsigned int uFlag) const;

  unsigned int count() const;

private:
  unsigned int m_uCounter;
  T            m_data;
  unsigned int m_uFlags;
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------
template<class T>
inline SharedData<T>::SharedData()
  : m_uCounter(0)
   , m_uFlags(0)
{
  //note: m_data is uninitialized
}

//---------------------------------------------
template<class T>
inline SharedData<T>::SharedData(T data) 
  : m_data(data)
  , m_uCounter(0)
  , m_uFlags(0)
{
}

//---------------------------------------------
template<class T>
inline SharedData<T>::~SharedData()
{ 
  m_uCounter = 0; 
}

//---------------------------------------------
template<class T>
inline void SharedData<T>::inc()
{ 
  ++m_uCounter; 
}

//---------------------------------------------
template<class T>
inline unsigned int SharedData<T>::dec()
{ 
  return --m_uCounter; 
}

//---------------------------------------------
template<class T>
inline T SharedData<T>::get() const
{ 
  return m_data;  
}

//---------------------------------------------
template<class T>
inline void SharedData<T>::set(T data)
{
  m_data = data;
}

//---------------------------------------------
template<class T>
inline void SharedData<T>::setFlags(unsigned int uFlag)
{
}

//---------------------------------------------
template<class T>
inline unsigned int SharedData<T>::getFlags() const
{
}

//---------------------------------------------
template<class T>
inline void SharedData<T>::addFlag(unsigned int uFlag)
{
  m_uFlags |= uFlag;
}

//---------------------------------------------
template<class T>
inline void SharedData<T>::removeFlag(unsigned int uFlag)
{
  m_uFlags &= (~uFlag);
}

//---------------------------------------------
template<class T>
inline bool SharedData<T>::testFlag(unsigned int uFlag) const
{
  return (m_uFlags & uFlag) != 0;
}

//---------------------------------------------
template<class T>
inline unsigned int SharedData<T>::count() const
{
  return m_uCounter;
}

} }

#endif 
