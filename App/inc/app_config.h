#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_

#include "app_common.h"
#include <string.h>
#include "Framework.h"


#ifndef PATH_APPCONF
#define PATH_APPCONF     "/etc/config.ini"
#endif









int app_defaultconfig(App_Defaultconf_t* defaultconf);

#endif