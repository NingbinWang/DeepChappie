#include "app_config.h"
#include "app_storager.h"
#include "app.h"
#include "app_test.h"
#include "framework.h"
#include "ANO_FUNC.h"

#include "app_media.h"

App_Defaultconf_t defaultconf;

static int GetBuildTime(void)
{ 
	printf("----------------------------------------\n");
	printf("Build at %s : %s \n",__DATE__,__TIME__);
	printf("----------------------------------------\n");		
	return 0;
}

int app_mediainit()
{
    return 0;
}

int app_init()
{
   ANO_Init();
   return 0;
}




int app_main()
{
  GetBuildTime();
  app_defaultconfig(&defaultconf);//loading ini
  Framework_Init(&defaultconf);
  app_init();
  app_mediainit();
  while (1)
  {
    sleep(1);
  }
  
  return 0;
}