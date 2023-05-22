#include "Notification.h"
#include "Framework.h"
#include "Framework_priv.h"

//#include "avb_1722_talker.h"

IGsensor_manager *gsensor = NULL;
IDevMonitor *devmonitor = NULL;
INetwork *network = NULL;
INotification *notification = NULL;
//IAVB1722Talker *avb1722talker;

static int Framework_Component_Init(App_Defaultconf_t *config)
{
  //instance
  //gsensor
  if(config->gsensorinfo.enable){
     gsensor = gsensor_manager_init_instance();
  }
   //devmoitor
  if(config->devmonitorinfo.enable){
     devmonitor = devmonitor_init_instance();
     //devmonitor  init
     devmonitor->Init(devmonitor);
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
     notification = notification_init_instance(config->notificationinfo.pingpongsize);
     notification->Init(notification);
  }

  return 0;
}

static int Framework_Component_DefaultParam(App_Defaultconf_t *config)
{
  /*
     AVB_1722_TALKER_SET_PARAM_T  avb1722talkerSetParam;
     //avb talker config
     avb1722talkerSetParam.stParam.iSockPriority = 6;
     avb1722talkerSetParam.stParam.uMaxPayloadLen = 1400;
     avb1722talkerSetParam.stParam.uMaxTransmitTime = 4;
     avb1722talkerSetParam.stParam.uStreamID = 0x93E0f000FE120004;
     strncpy(avb1722talkerSetParam.stParam.aMacAddr."02:47:57:4D:00:91",sizeof(avb1722talkerSetParam.stParam.aMacAddr));
     strncpy(avb1722talkerSetParam.stParam.aInterfaceName."eth0.3",sizeof(avb1722talkerSetParam.stParam.aInterfaceName));
     avb1722talkerSetParam.uFrameBufSize = 0; 
     avb1722talker->SetParam(avb1722talker,&avb1722talkerSetParam);
   */


     return 0;
}


static int Framework_Component_DefaultStart(App_Defaultconf_t *config)
{
   if(config->gsensorinfo.enable){
      gsensor->Start(gsensor);
   }
   if(config->notificationinfo.enable){
      if(config->gsensorinfo.enable)
         notification->Subscribe(notification,"IMU");
      if(config->networkinfo.enable)
         notification->Subscribe(notification,"NET");
      if(config->devmonitorinfo.enable)
         notification->Subscribe(notification,"DEVMONTOR");
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
    if(config->notificationinfo.enable){
      if(config->gsensorinfo.enable){
         
      }
    }
    return 0;
}

IGsensor_manager* Framework_GetGsensorinterface(void)
{
  return gsensor;
}

IDevMonitor* Framework_GetDevMonitorinrterface(void)
{
  return devmonitor;
}

INetwork* Framework_GetNetworkinrterface(void)
{
  return network;
}