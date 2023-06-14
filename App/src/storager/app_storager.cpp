#include "Framework.h"
#include "app_storager.h"


ERRORCODE_E app_storager_onevent(EVENTPARAM_T* param)
{
    Storager_Notifybroker_T* broker;
    if (param->size != sizeof(Storager_Notifybroker_T))
    {
        return ERROR_SIZE_MISMATCH;
    }
    broker = (Storager_Notifybroker_T*)param->pData;
    if(param->event == EVENT_NOTIFY)
    {
        if(broker->eState == STORAGER_STATE_ABNORMAL)
        {
            printf("storager is error\n");
        }else if(broker->eState == STORAGER_STATE_NORMAL){
            printf("storager normal\n");
        }
        
    }
    return ERROR_NONE;
}

int app_storager_init(void)
{
    IStorager *storager = NULL;
    storager = storager_manager_get_instance();
    if(storager == NULL)
    {
        return -1;
    }
    storager->OnEvent(storager,app_storager_onevent);
    return 0;
}