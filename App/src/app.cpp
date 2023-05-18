#include "app_device.h"
#include "app_config.h"
#include "app.h"


IDevMonitor* objDevMonitor ;
App_Defaultconf_t defaultconf;

int app_main()
{
  app_defaultconfig(&defaultconf);
  Framework_Init(&defaultconf);
  //app init
  objDevMonitor = Framework_GetDevMonitorinrterface();
  app_tf_regitster(objDevMonitor);
  while (1)
  {
    Framework_work();
    sleep(100);
  }
  
  return 0;
}

