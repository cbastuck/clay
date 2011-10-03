#include <clay-core/base/Module.h>
#include <clay-modules/host/HostModule.h>

int main(int argc, char** charv)
{
  {
    CLAY::Module aModule("my Module");
    aModule.init(NULL);
    aModule.deInit();
  }

  {
    CLAY::MODULE::HostModule aHost("host");
    unsigned int i=aHost.getNumHostedModules();
    CLAY_ASSERT(i==0);
  }

  return 0;
}
