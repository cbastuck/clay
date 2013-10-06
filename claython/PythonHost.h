#ifndef CLAY_PYTHONHOST_H
#define CLAY_PYTHONHOST_H

#include <clay-core/host/Host.h>

namespace CLAY{

class PythonHost : public Host
{
public:
  PythonHost();
  virtual ~PythonHost();

  bool connect(Module* from, Module* to);
  Module* createModule(const char* sModuleURI, const tString& sRuntimeModuleId);
};

}

#endif
