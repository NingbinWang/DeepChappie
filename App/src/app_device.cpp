#include "app_device.h"

void app_device_tf_callback(DEV_MONITOR_TYPE_E eType, int iDevIndex, int iPartIndex, DEV_MONITOR_ACTION_E eAction)
{
    printf("app_tf_device_callback type:%d index:%d,part:%d,active:%d \n",eType,iDevIndex,iPartIndex,eAction);
    switch (eType)
    {
    case DEV_MONITOR_TYPE_TF:
        
        break;
    
    default:
        break;
    }




    
}



int app_tf_regitster(IDevMonitor* devmonitor)
{
    devmonitor->RegisterActionCallBack(devmonitor,DEV_MONITOR_TYPE_TF,app_device_tf_callback);
    return 0;
}