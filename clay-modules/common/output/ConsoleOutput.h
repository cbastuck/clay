#ifndef CONSOLE_OUTPUT_H_
#define CONSOLE_OUTPUT_H_


#include <clay-core/base/ModuleHelper.h>

namespace CLAY{ namespace MODULE{

class ConsoleOutput : public ModuleHelper<ConsoleOutput,
                                          HELPER::IntegerEncoding<'c','o','u','t'>::value,
                                          HELPER::IntegerEncoding<'C','O','M','N'>::value>
{
private:
  typedef ModuleHelper<ConsoleOutput,
                       HELPER::IntegerEncoding<'c','o','u','t'>::value,
                       HELPER::IntegerEncoding<'C','O','M','N'>::value> tBase;
public:
  ConsoleOutput(const tString& sId);
  virtual ~ConsoleOutput();

  virtual void registerModuleInputs();

  virtual ProcessResultCode process();
};

} }

#endif
