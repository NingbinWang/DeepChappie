#ifndef _FRAMEWORK_H_
#define _FRAMEWORK_H_
#include "Gsensor.h"
#include "Devmonitor.h"
#include "Serial.h"
#include "Network.h"
#include "Storager.h"
#include "Media.h"

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/



typedef struct 
{
    NetworkInfo_t networkinfo;
    Storager_Info_T storagerinfo;
}App_Defaultconf_t;





void Framework_Init(App_Defaultconf_t *config);
int Framework_work(void);

IGsensor_manager* Framework_GetGsensorinterface(void);
IDevMonitor* Framework_GetDevMonitorinrterface(void);


#ifdef __cplusplus
}
#endif/*__cplusplus*/


#endif