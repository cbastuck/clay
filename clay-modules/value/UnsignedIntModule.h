#ifndef CLAY_UNSIGNED_INT_MODULE_H_
#define CLAY_UNSIGNED_INT_MODULE_H_

#include <clay-modules/value/ValueModule.h>

#include <clay-core/helper/LexicalConversion.h>

namespace CLAY{ namespace MODULE{

class UnsignedIntModule : public ValueModule<unsigned int>
{
private:
  typedef ValueModule<unsigned int> tBase;

public:
  static const char* staticModuleURI();

  UnsignedIntModule(const tString& sId);
  virtual ~UnsignedIntModule();

  virtual const char* getModuleURI() const;

  virtual bool setString(const tString& sValue);
  virtual bool getString(tString& sDst);
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------
inline const char* UnsignedIntModule::staticModuleURI()
{
    return "http://claymodules.org/common/uint";
}

//---------------------------------------------UnsignedIntModule
inline UnsignedIntModule::UnsignedIntModule(const tString& sId)
  : tBase(sId)
{
}

//---------------------------------------------~UnsignedIntModule
inline UnsignedIntModule::~UnsignedIntModule()
{
}

//---------------------------------------------
inline const char* UnsignedIntModule::getModuleURI() const
{
    return staticModuleURI();
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
