#include "app_device.h"
#include "app.h"


IDevMonitor* objDevMonitor ;


int app_main()
{
  
  Framework_Init();
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

