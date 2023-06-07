#include "Notification_priv.h"

/**
  * @brief  在初始化broker
  * @param  name:  Account ID
  * @retval If the search is successful, return the pointer of the account
  */
VOID notifybroker_init(BROKER_NODE_T *pStNode,UINT32 BufferSize)
{
    UINT8* buf0 = NULL;
    UINT8* buf1 = NULL;
    if(NULL == pStNode)
    {
        LOGGER_ERROR("notifybroker_init get pStNode failed\n");
        return;
    }
    pStNode->BufferSize = BufferSize;
    UINT8 *buffer = (UINT8*) sys_mem_malloc((pStNode->BufferSize) * sizeof(UINT8) * 2);
    if (!buffer)
    {
        LOGGER_ERROR("buffer malloc failed!");
        sys_mem_free(pStNode);
        return;
    }
    memset(buffer, 0, (pStNode->BufferSize) * sizeof(UINT8) * 2);
    buf0 = buffer;
    buf1 = buffer + (pStNode->BufferSize);
    PingPongBuffer_Init(&pStNode->BufferManager, buf0, buf1);
}


/**@fn         notifybroker_pushback  
 * @brief      在
 * @param[in]  pINotification   pINotification对象操作指针
 * @return     成功返回OK     失败返回错误码
 */

void notifybroker_pushback(NOTIFICATION_PRIV_DATA_T *pStPrivData,const char* ID,UINT32 BufferSize)
{
     BROKER_NODE_T *pStNode = NULL;
    if(NULL == pStPrivData)
    {
        LOGGER_ERROR("notifybroker_pushback get pStPrivData failed\n");
        return;
    }

    pStNode = (BROKER_NODE_T*)sys_mem_malloc(sizeof(BROKER_NODE_T));
    if(!pStNode)
    {
        LOGGER_ERROR("sys_mem_malloc failed\n");
        return;
    }
    memset(pStNode,0,sizeof(BROKER_NODE_T));
    notifybroker_init(pStNode,BufferSize);
    strcpy(pStNode->ID,ID);
    sys_mutex_lock(&pStPrivData->brokerMutex,WAIT_FOREVER);
    list_add(&pStPrivData->publishers,&pStNode->node);
    pStPrivData->publishers.count++;
    sys_mutex_unlock(&pStPrivData->brokerMutex);
}




/**@fn         notification_subscribe  
 * @brief      初始化参数
 * @param[in]  pINotification   pINotification对象操作指针
 * @return     成功返回OK     失败返回错误码
 */
int notifybroker_remove(LIST_T* publishers, BROKER_NODE_T *pBrokernode)
{
    if(publishers == NULL || pBrokernode == NULL)
    {
        LOGGER_ERROR("notifybroker_remove input param failed\n");
        return ERROR;
    }
    list_delete(publishers,&pBrokernode->node);
    publishers->count--;
    return OK;
}
