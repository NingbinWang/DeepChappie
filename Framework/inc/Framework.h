#ifndef _FRAMEWORK_H_
#define _FRAMEWORK_H_
#include "Gsensor.h"
#include "Devmonitor.h"
#include "Serial.h"
#include "Network.h"
#include "Storager.h"
#include "Media.h"
#include "Notification.h"
#include "Medium.h"

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/



typedef struct 
{
    NetworkInfo_t networkinfo;
    Storager_Info_T storagerinfo;
    Media_Info_T mediainfo;
    Gsensor_Info_T gsensorinfo;
    Devmonitor_Info_t devmonitorinfo;
    Notification_Info_t notificationinfo;
    Serial_Info_t serialinfo0;

}App_Defaultconf_t;





void Framework_Init(App_Defaultconf_t *config);
int Framework_work(App_Defaultconf_t *config);

IGsensor_manager* Framework_GetGsensor(void);
IDevMonitor* Framework_GetDevMonitor(void);
INetwork* Framework_GetNetwork(void);
INotification* Framework_Getnotification(void);
IStorager* Framework_Getstorager(void);
IMediumManager* Framework_Getmedium(void);


#ifdef __cplusplus
}
#endif/*__cplusplus*/


#endif