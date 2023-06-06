#ifndef _NOTIFICATION_PRIV_H_
#define _NOTIFICATION_PRIV_H_
#include "PingPongBuffer.h"
#include "Notification.h"
#include <stdio.h>
#include "sys_common.h"
#include "logger.h"
#include  "sys_mem.h"
#include  "DataCentor.h"




/**
 * @struct    NOTIFICATION_PRIV_DATA_T
 * @brief     NOTIFICATION私有数据结构
*/
typedef struct
{
    DataCentor_Header* subscribers;
    DataCentor_Header* publishers;
    UINT32 BufferSize;
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
















#endif