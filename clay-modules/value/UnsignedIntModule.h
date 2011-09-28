#ifndef CLAY_UNSIGNED_INT_MODULE_H_
#define CLAY_UNSIGNED_INT_MODULE_H_

#include <clay-modules/value/ValueModule.h>

#include <clay-core/helper/LexicalConversion.h>

namespace CLAY{ namespace MODULE{

class UnsignedIntModule : public ValueModule<UnsignedIntModule, 
                                             unsigned int, 
                                             HELPER::IntegerEncoding<'U','I','N','T'> >
{
private:
  typedef ValueModule<UnsignedIntModule, 
                      unsigned int, 
                      HELPER::IntegerEncoding<'U','I','N','T'> > tBase;

public:
  UnsignedIntModule(const tString& sId);
  virtual ~UnsignedIntModule();

  virtual bool setString(const tString& sValue);
  virtual bool getString(tString& sDst);
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------UnsignedIntModule
inline UnsignedIntModule::UnsignedIntModule(const tString& sId)
  : tBase(sId)
{
}

//---------------------------------------------~UnsignedIntModule
inline UnsignedIntModule::~UnsignedIntModule()
{
}

//---------------------------------------------setString
inline bool UnsignedIntModule::setString(const tString& sValue)
{
  unsigned int uValue = LexicalConversion::toNumber<unsigned int>(sValue);
  return setValue(uValue);
}

//---------------------------------------------getString
inline bool UnsignedIntModule::getString(tString& sDst)
{
  const unsigned int* pInt = getValue();
  if(pInt)
  {
    sDst = LexicalConversion::toString(*pInt);
    return true;
  }
  return false;
}

} }

#endif
