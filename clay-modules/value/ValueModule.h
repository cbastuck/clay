#ifndef CLAY_VALUE_MODULE_H_
#define CLAY_VALUE_MODULE_H_

#include <clay-core/base/Module.h>
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

template<class tValue>
class ValueModule : public Module
                  , public ValueModuleBase
{
public:
  ValueModule(const tString& sId);

  bool init(XERCES::DOMNode* pNode);

  virtual void registerModuleOutputs();

  virtual bool save(XERCES::DOMElement* pNode);
  virtual bool load(XERCES::DOMElement* pNode, Module::tConnectionMap* pInputConnections = NULL, Module::tConnectionMap* pOutputConnections = NULL);
  
protected:
  virtual const tValue* getValue();
  virtual bool setValue(const tValue& aVal);

private:
};

//---------------------------------------------
//---------------------------------------------
//---------------------------------------------

//---------------------------------------------ValueModule
template<class tValue>
inline ValueModule<tValue>::ValueModule(const tString& sId)
  : Module(sId)
{
    registerModuleOutputs();
}

//---------------------------------------------init
template<class tValue>
inline bool ValueModule<tValue>::init(XERCES::DOMNode* pNode)
{
  if(!Module::init(pNode))
  {
      return false;
  }

  registerModuleOutputs();

  return false;
}

//---------------------------------------------registerModuleOutputs
template<class tValue>
inline void ValueModule<tValue>::registerModuleOutputs()
{
  registerOutput(new ModuleOutput<tValue>(), "value");
}

//---------------------------------------------registerModuleOutputs
template<class tValue>
inline const tValue* ValueModule<tValue>::getValue()
{
  ModuleOutput<tValue>* pOutput = static_cast<ModuleOutput<tValue>*>(getOutput("value"));
  if(pOutput)
  {
    return &pOutput->getData();
  }
  return NULL;
}

//---------------------------------------------registerModuleOutputs
template<class tValue>
bool ValueModule<tValue>::setValue(const tValue& val)
{
  ModuleOutput<tValue>* pOutput = static_cast<ModuleOutput<tValue>*>(getOutput("value"));
  if(pOutput)
  {
    pOutput->setData(val);
    return true;
  }
  return false;
}

//---------------------------------------------save
template<class tValue>
bool ValueModule<tValue>::save(XERCES::DOMElement* pParent)
{
  XERCES::DOMElement* pNode = XercesXML::appendNode(pParent, "value-module");

  const tValue* pValue = getValue();
  if(pValue)
  {
    tString sValue = LexicalConversion::toString(*pValue);
    XercesXML::addAttribute(pNode, "value", sValue);
  }

  XERCES::DOMElement* pBase = XercesXML::appendNode(pNode, "base");
  return save(pBase);
}

//---------------------------------------------load
template<class tValue>
bool ValueModule<tValue>::load(XERCES::DOMElement* pParent, Module::tConnectionMap* pInputConnections, Module::tConnectionMap* pOutputConnections)
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

  return load(pBase, pInputConnections, pOutputConnections);
}

} }

#endif
