#include "Notification.h"
#include "Framework.h"
#include "avb_1722_talker.h"
#include "Framework_priv.h"

IAVB1722Talker * pAvb1722Talker;
void Framework_Init(void)
{
    AVB_1722_TALKER_INIT_PARAM_T InitParam;
    pAvb1722Talker = avb_1722_talker_instance(&InitParam);
    pAvb1722Talker->Init();


}

int Framework_work(void)
{

}