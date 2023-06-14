#ifndef _NOTIFICATION_PRIV_H_
#define _NOTIFICATION_PRIV_H_

#include <stdio.h>



#include "sys_common.h"
#include "logger.h"
#include "sys_mem.h"
#include "sys_list.h"
#include "sys_mutex.h"


#include "PingPongBuffer.h"
#include "Notification.h"


#define DATACENTORIDMAX 128

typedef struct 
{
   NODE_T             node;
   CHAR               ID[DATACENTORIDMAX];   //Unique data ID
   PingPongBuffer_t   BufferManager;
   EventCallback_t    eventCallback;
   UINT32             BufferSize;
}BROKER_NODE_T;


/**
 * @struct    NOTIFICATION_PRIV_DATA_T
 * @brief     NOTIFICATION私有数据结构
*/
typedef struct
{
    //LIST_T subscribers;
    LIST_T publishers;
    MUTEX_ID brokerMutex;
   
}NOTIFICATION_PRIV_DATA_T;

/**
 * @struct    NOTIFICATION_BASE_T
 * @brief     NOTIFICATION基础数据结构
*/
typedef struct
{
    INotification           stInterface; /* notificition接口 */
    NOTIFICATION_PRIV_DATA_T stPrivData;  /* 私有数据 */
}NOTIFICATION_BASE_T;

void notifybroker_pushback(NOTIFICATION_PRIV_DATA_T *pStPrivData,const char* ID,void *pData,UINT32 BufferSize);
int notifybroker_remove(LIST_T* publishers, BROKER_NODE_T *pBrokernode);


















#endif