#include "Notification.h"
#include "Framework.h"
#include "Framework_priv.h"

//#include "avb_1722_talker.h"

IGsensor_manager *gsensor;
IDevMonitor *devmonitor;
//IAVB1722Talker *avb1722talker;

static int Framework_Component_Init(void)
{
  //instance
  //avb
   //avb1722talker = avb_1722_talker_instance();
  //gsensor
   //gsensor = gsensor_manager_init_instance();
   //devmoitor
   devmonitor = devmonitor_init_instance();
   

   //init
  // avb1722talker->Init(avb1722talker);
   // gsensor->Init(gsensor);
    devmonitor->Init(devmonitor);

   return 0;
}

static int Framework_Component_DefaultConfig(void)
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


static int Framework_Component_DefaultStart(void)
{
     //avb1722talker->Start(avb1722talker);
      // gsensor->Start(gsensor);
     return 0;
}

void Framework_Init(void)
{

   Framework_Component_Init();
   Framework_Component_DefaultConfig();
   Framework_Component_DefaultStart();
}

int Framework_work(void)
{
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