#include "ConsoleOutput.h"

namespace CLAY{ namespace MODULE{

//---------------------------------------------
ConsoleOutput::ConsoleOutput(const tString& sId)
  : tBase(sId)
{
}

//---------------------------------------------
ConsoleOutput::~ConsoleOutput()
{
}

//---------------------------------------------
void ConsoleOutput::registerModuleInputs()
{
  registerInput(new ModuleInput<tString>(), "in");
}

//---------------------------------------------
Module::ProcessResultCode ConsoleOutput::process()
{
  ModuleInput<tString>* pIn = static_cast<ModuleInput<tString>*>(getInput("in"));
  if(pIn->isConnected())
  {
    std::cout << *pIn->getData() << std::endl;
  }
  return Module::Process_OK;
}

} }
