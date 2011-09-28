#include "ClayAllocator.h"

#include <exception> // for std::bad_alloc
#include <cstdlib> // for malloc() and free()

//---------------------------------------------clayNew
void* ClayAllocator::clayNew(size_t size)
{
  void* p=malloc(size); 
  if(p==0) // did malloc succeed?
  {
    throw std::bad_alloc(); // ANSI/ISO compliant behavior
  }
  return p;
}

//---------------------------------------------clayDelete
void ClayAllocator::clayDelete(void* p)
{
  free(p);
}

//---------------------------------------------clayArrayNew
void* ClayAllocator::clayArrayNew(size_t size)
{
  void* p=malloc(size); 
  if(p==0) // did malloc succeed?
  {
    throw std::bad_alloc(); // ANSI/ISO compliant behavior
  }
  return p;
}

//---------------------------------------------clayArrayDelete
void  ClayAllocator::clayArrayDelete(void *p)
{
  free(p);
}