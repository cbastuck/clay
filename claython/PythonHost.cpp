#include "PythonHost.h"

#include <clay-core/helper/XMLHelper.h>
#include <clay-core/registry/ModuleRegistry.h>

namespace CLAY{

class HostStatics
{
public:
  HostStatics()
  {
    CLAY::ClayXML::init();
    CLAY::MODEL::ClayTypeChecking::init();
  }

  ~HostStatics()
  {
    CLAY::MODEL::ClayTypeChecking::deInit();
    CLAY::ClayXML::deInit();
  }

  ModuleRegistry m_registry;
};

static HostStatics statics;

//---------------------------------------------
PythonHost::PythonHost()
  : Host(&statics.m_registry)
{

}

//---------------------------------------------
PythonHost::~PythonHost()
{ 

}

//---------------------------------------------
bool PythonHost::connect(Module* from, Module* to)
{
  return to->connectFrom(from) == Module::Connect_OK;
}

//---------------------------------------------
Module* PythonHost::createModule(const char* sModuleURI, const tString& sRuntimeModuleId)
{
  return Host::createModule(sModuleURI, sRuntimeModuleId, NULL, true);
}

}
