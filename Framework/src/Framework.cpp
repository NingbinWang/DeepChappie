#include "Notification.h"
#include "Framework.h"
#include "Framework_priv.h"

//#include "avb_1722_talker.h"

IGsensor_manager *gsensor = NULL;
IDevMonitor *frdevmonitor = NULL;
INetwork *network = NULL;
INotification *notification = NULL;
IStorager *storager = NULL;
IMediumManager *medium = NULL;
//ISerial *serial0=NULL;
//IAVB1722Talker *avb1722talker;

static int Framework_Component_Init(App_Defaultconf_t *config)
{
  //instance
  //gsensor
  if(config->gsensorinfo.enable){
     gsensor = gsensor_manager_init_instance();
     gsensor->Init(gsensor);
  }
   //network
  if(config->networkinfo.enable){
    if(strlen(config->networkinfo.DevName) != 0){
       network = network_init_instance(config->networkinfo.DevName);
       network->Init(network,config->networkinfo.IP,config->networkinfo.NetMask,config->networkinfo.GateWay);
    }else{
       printf("no ini set eth0!!!\n");
       network = network_init_instance("eth0");
       network->Init(network,"10.65.38.165","255.255.255.0","10.65.38.254");
    }
  }
  //notification
  if(config->notificationinfo.enable){
     init_notification_component();
     notification = notification_get_notifybroker();
     notification->Init(notification);
  }
  //storager
  if(config->storagerinfo.enable){
   //devmonitor  init
     init_devmonitor_component();
     frdevmonitor = devmonitor_get_instance();
     frdevmonitor->Init(frdevmonitor);
     init_medium_manager_component();
     medium = medium_manager_get_instance();
     medium->Init(medium,&(config->storagerinfo));
     storager = storager_manager_init_instance();
     storager->Init(storager);
  }
  //serial
 // if(config->serialinfo0.enable){
  //   serial0 = serial_init_instance("ttyS",0);
  //}
  return 0;
}

static int Framework_Component_DefaultParam(App_Defaultconf_t *config)
{



     return 0;
}


static int Framework_Component_DefaultStart(App_Defaultconf_t *config)
{
   if(config->gsensorinfo.enable){
      gsensor->Start(gsensor);
   }
   if(config->storagerinfo.enable){
     medium->Start(medium);
      medium->Format(medium,0,MEDIUM_FORMAT_FAT32);
   }
   return 0;
}

void Framework_Init(App_Defaultconf_t *config)
{
   Framework_Component_Init(config);
   Framework_Component_DefaultParam(config);
   Framework_Component_DefaultStart(config);
}

int Framework_work(App_Defaultconf_t *config)
{
   /*
    if(config->notificationinfo.enable){
      if(config->gsensorinfo.enable){
         Sensordata_t data;
         gsensor->GetData(gsensor,&data);
         notification->Commit(notification,"IMU",&data,sizeof(Sensordata_t));
      }
    }
    */
    return 0;
}

IGsensor_manager* Framework_GetGsensor(void)
{
  return gsensor;
}

IDevMonitor* Framework_GetDevMonitor(void)
{
  return frdevmonitor;
}

INetwork* Framework_GetNetwork(void)
{
  return network;
}

INotification* Framework_Getnotification(void)
{
  return notification;
}

IStorager* Framework_Getstorager(void)
{
  return storager;
}

IMediumManager* Framework_Getmedium(void)
{
   return medium;
}