#ifndef CLAY_VALUE_MODULE_H_
#define CLAY_VALUE_MODULE_H_

#include <clay-core/base/ModuleHelper.h>
#include <clay-core/base/ModuleOutput.h>
#include <clay-core/helper/LexicalConversion.h>

namespace CLAY{ namespace MODULE{

class ValueModuleBase
{
public:
  virtual bool setString(const tString& sValue) = 0;
  virtual bool getString(tString& sDst) = 0;
}; 

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

template<class tDerived, class tValue, class tModuleId>
class ValueModule : public ModuleHelper<tDerived,
                                        tModuleId::value,
                                        HELPER::IntegerEncoding<'C','O','M','N'>::value>,
                    public ValueModuleBase
{
private:
  typedef ModuleHelper<tDerived, 
                       tModuleId::value, 
                       HELPER::IntegerEncoding<'C','O','M','N'>::value> tBase;
  typedef tValue T;

public:
  ValueModule(const tString& sId);

  virtual void registerModuleOutputs();

  virtual bool save(XERCES::DOMElement* pNode);
  virtual bool load(XERCES::DOMElement* pNode, Module::tConnectionMap* pInputConnections = NULL, Module::tConnectionMap* pOutputConnections = NULL);
  
protected:
  virtual const T* getValue();
  virtual bool setValue(const T& aVal);

private:
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------ValueModule
template<class tDerived, class tValue, class tModuleId>
inline ValueModule<tDerived, tValue, tModuleId>::ValueModule(const tString& sId)
  : tBase(sId)
{
}

//---------------------------------------------registerModuleOutputs
template<class tDerived, class tValue, class tModuleId>
inline void ValueModule<tDerived, tValue, tModuleId>::registerModuleOutputs()
{
  registerOutput(new ModuleOutput<T>(), "value");
}

//---------------------------------------------registerModuleOutputs
template<class tDerived, class tValue, class tModuleId>
inline const typename ValueModule<tDerived, tValue, tModuleId>::T* ValueModule<tDerived, tValue, tModuleId>::getValue()
{
  ModuleOutput<T>* pOutput = static_cast<ModuleOutput<T>*>(tBase::getOutput("value"));
  if(pOutput)
  {
    return &pOutput->getData();
  }
  return NULL;
}

//---------------------------------------------registerModuleOutputs
template<class tDerived, class tValue, class tModuleId>
bool ValueModule<tDerived, tValue, tModuleId>::setValue(const T& aVal)
{
  ModuleOutput<T>* pOutput = static_cast<ModuleOutput<T>*>(tBase::getOutput("value"));
  if(pOutput)
  {
    pOutput->setData(aVal);
    return true;
  }
  return false;
}

//---------------------------------------------save
template<class tDerived, class tValue, class tModuleId>
bool ValueModule<tDerived, tValue, tModuleId>::save(XERCES::DOMElement* pParent)
{
  XERCES::DOMElement* pNode = XercesXML::appendNode(pParent, "value-module");

  const tValue* pValue = getValue();
  if(pValue)
  {
    tString sValue = LexicalConversion::toString(*pValue);
    XercesXML::addAttribute(pNode, "value", sValue);
  }

  XERCES::DOMElement* pBase = XercesXML::appendNode(pNode, "base");
  return tBase::save(pBase);
}

//---------------------------------------------load
template<class tDerived, class tValue, class tModuleId>
bool ValueModule<tDerived, tValue, tModuleId>::load(XERCES::DOMElement* pParent, Module::tConnectionMap* pInputConnections, Module::tConnectionMap* pOutputConnections)
{
  XERCES::DOMElement* pNode = XercesXML::findChildNode(pParent, "value-module");
  if(!pNode)
  {
    return false;
  }

  tString sValue;
  if(!XercesXML::getAttributeValue(pNode, "value", sValue))
  {
    return false;
  }

  setString(sValue);

  XERCES::DOMElement* pBase = XercesXML::findChildNode(pNode, "base");
  if(!pBase)
  {
    return false;
  }

  return tBase::load(pBase, pInputConnections, pOutputConnections);
}

} }

#endif
