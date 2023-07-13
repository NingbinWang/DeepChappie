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
    int  enable;
    char DevName[NETWORKSTRMAX];      //eth名字
   	char IP[NETWORKSTRMAX];           // IP地址
	  char NetMask[NETWORKSTRMAX];      // netmask 
	  char GateWay[NETWORKSTRMAX];      // gateway 
}NetworkInfo_t;

typedef struct 
{
    char IP[NETWORKSTRMAX];           // IP地址
}Network_Notifybroker_T;

typedef struct 
{
    int enable;
    int tfid;
    int emmcid;
    unsigned int dirnum;//根目录下有多少个文件夹
    char dirname[DIRNUMMAX][DIRNAMESTRMAX];//相关的名字
}Storager_Info_T;

typedef struct 
{
    STORAGER_ID_T    id;
    STORAGER_STATE_E    eState;        //介质状态
}Storager_Notifybroker_T;

typedef struct 
{
    int  enable;
}Devmonitor_Info_t;

typedef struct
{
   int  enable;
}Gsensor_Info_T;


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
INetwork* get_network_intferface();



#ifdef __cplusplus
}
#endif/*__cplusplus*/


#endif