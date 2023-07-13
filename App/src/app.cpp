#include "app_config.h"
#include "app_storager.h"
#include "app.h"

#include "framework.h"

App_Defaultconf_t defaultconf;

int app_main()
{
  app_defaultconfig(&defaultconf);//loading ini
  Framework_Init(&defaultconf);
  while (1)
  {
    sleep(100);
  }
  
  return 0;
}