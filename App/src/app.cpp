#include "app_device.h"
#include "app_config.h"
#include "app.h"


IDevMonitor* objDevMonitor ;
App_Defaultconf_t defaultconf;

int app_main()
{
  app_defaultconfig(&defaultconf);//loading ini
  Framework_Init(&defaultconf);
  //app init
  objDevMonitor = Framework_GetDevMonitorinrterface();
  if(objDevMonitor != NULL){
     app_tf_regitster(objDevMonitor);
  }
  while (1)
  {
    Framework_work(&defaultconf);
    sleep(100);
  }
  
  return 0;
}

