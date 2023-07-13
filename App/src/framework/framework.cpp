#include "framework.h"
#include "app_common.h"
IGsensor_manager *gsensor = NULL;
IDevMonitor *frdevmonitor = NULL;
INetwork *network = NULL;
INotification *notification = NULL;
IStorager *storager = NULL;
IMediumManager *medium = NULL;

static int Framework_Component_Init(App_Defaultconf_t *config)
{
  //instance
  //gsensor
  if(config->gsensorinfo.enable){
     init_gsensor_manager_component();
     gsensor = gsensor_manager_get_instance();
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
       network->Init(network,"192.168.1.100","255.255.255.0","10.65.38.254");
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
      init_storager_manager_component();
     storager = storager_manager_get_instance();
     storager->Init(storager);
     init_devmonitor_component();
     frdevmonitor = devmonitor_get_instance();
     frdevmonitor->Init(frdevmonitor);
     init_medium_manager_component();
     medium = medium_manager_get_instance();
     medium->Init(medium,config->storagerinfo.tfid,1);

  }
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

     
   }
   return 0;
}

void Framework_Init(App_Defaultconf_t *config)
{
   Framework_Component_Init(config);
   Framework_Component_DefaultParam(config);
   Framework_Component_DefaultStart(config);
}

INetwork* get_network_intferface()
{
    return network;
}