#include "Notification.h"
#include "Framework.h"
#include "Framework_priv.h"
#include "gsensor.h"

IGsensor_manager *gsensor;
void Framework_Init(void)
{
   gsensor = gsensor_manager_init_instance();
   gsensor->Init(gsensor);
   gsensor->Start(gsensor);
}

int Framework_work(void)
{
    return 0;
}