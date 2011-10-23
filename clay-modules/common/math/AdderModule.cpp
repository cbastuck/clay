
#include "AdderModule.h"

namespace CLAY{ namespace MODULE{

//---------------------------------------------
AdderModule::AdderModule(const tString& sId)
  : tBase(sId)
{
}

//---------------------------------------------
AdderModule::~AdderModule()
{
}

//---------------------------------------------
void AdderModule::registerModuleInputs()
{
  registerInput(new ModuleInput<unsigned int>(), "in1");
  registerInput(new ModuleInput<unsigned int>(), "in2");
}

//---------------------------------------------
void AdderModule::registerModuleOutputs()
{
  registerOutput(new ModuleOutput<unsigned int>(), "out");
}

//---------------------------------------------
bool AdderModule::save(XERCES::DOMElement* pNode)
{
  return tBase::save(pNode);
}

//---------------------------------------------
bool AdderModule::load(XERCES::DOMElement* pNode, Module::tConnectionMap* pInputConnections, Module::tConnectionMap* pOutputConnections)
{
  return tBase::load(pNode, pInputConnections, pOutputConnections);
}

//---------------------------------------------
Module::ProcessResultCode AdderModule::process()
{
  ModuleInput<unsigned int>* pIn1 = static_cast<ModuleInput<unsigned int>*>(getInput("in1"));
  ModuleInput<unsigned int>* pIn2 = static_cast<ModuleInput<unsigned int>*>(getInput("in2"));

  unsigned int uIn1 = (pIn1->isConnected()) ? *pIn1->getData() : 0;
  unsigned int uIn2 = (pIn2->isConnected()) ? *pIn2->getData() : 0;

  ModuleOutput<unsigned int>* pOut = static_cast<ModuleOutput<unsigned int>*>(getOutput("out"));
  pOut->setData(uIn1 + uIn2);

  return Module::Process_OK;
}

} }
