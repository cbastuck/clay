#ifndef ADDER_MODULE_H_
#define ADDER_MODULE_H_

#include <clay-core/base/ModuleHelper.h>

namespace CLAY{ namespace MODULE{

class AdderModule : public ModuleHelper<AdderModule,
                                        HELPER::IntegerEncoding<'a','d','d','r'>::value,
                                        HELPER::IntegerEncoding<'C','O','M','N'>::value>
{
private:
  typedef ModuleHelper<AdderModule,
                       HELPER::IntegerEncoding<'a','d','d','r'>::value,
                       HELPER::IntegerEncoding<'C','O','M','N'>::value> tBase;
public:
  AdderModule(const tString& sId);
  virtual ~AdderModule();

  virtual void registerModuleInputs();
  virtual void registerModuleOutputs();

  virtual bool save(XERCES::DOMElement* pNode);
  virtual bool load(XERCES::DOMElement* pNode, Module::tConnectionMap* pInputConnections = NULL, Module::tConnectionMap* pOutputConnections = NULL);
  
  virtual ProcessResultCode process();

private:
};

} }

#endif
