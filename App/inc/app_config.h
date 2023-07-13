#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_

#include "app_common.h"
#include <string.h>
#include "framework.h"


#ifndef PATH_APPCONF
#define PATH_APPCONF     "/etc/config.ini"
#endif



#define ENBALE 1





int app_defaultconfig(App_Defaultconf_t* defaultconf);

#endif