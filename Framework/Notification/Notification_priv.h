#ifndef _NOTIFICATION_PRIV_H_
#define _NOTIFICATION_PRIV_H_
#include "PingPongBuffer.h"
#include "Notification.h"
#include <stdio.h>
#include "sys_common.h"
#include "logger.h"
#include  "sys_mem.h"
#include  "DataCentor.h"


/* Event type enumeration */
typedef enum
{
        EVENT_NONE,
        EVENT_PUB_PUBLISH, // Publisher posted information
        EVENT_SUB_PULL,    // Subscriber data pull request
        EVENT_NOTIFY,      // Subscribers send notifications to publishers
        EVENT_TIMER,       // Timed event
        _EVENT_LAST
} EventCode_e;

/**
 * @enum   errorcode
 * @brief   
*/
typedef enum
{
    ERROR_NONE                = 0,
    ERROR_UNKNOW              = -1,
    ERROR_SIZE_MISMATCH       = -2,
    ERROR_UNSUPPORTED_REQUEST = -3,
    ERROR_NO_CALLBACK         = -4,
    ERROR_NO_CACHE            = -5,
    ERROR_NO_COMMITED         = -6,
    ERROR_NOT_FOUND           = -7,
    ERROR_PARAM_ERROR         = -8
} ErrorCode_e;

/**
 * @struct   Event parameter structure
 * @brief    eventparam
*/
typedef struct
{
        EventCode_t event; // Event type
        //Notification* tran;     // Pointer to sender
        //Notification* recv;     // Pointer to receiver
        void* pData;      // Pointer to data
        UINT32 size;     // The length of the data
} EventParam_t;


/**
 * @struct    NOTIFICATION_PRIV_DATA_T
 * @brief     NOTIFICATION私有数据结构
*/
typedef struct NOTIFICATION_PRIV_DATA_T
{ 
    PingPongBuffer_t BufferManager;
    UINT32 BufferSize;
}NOTIFICATION_PRIV_DATA_T;

/**
 * @struct    NOTIFICATION_BASE_T
 * @brief     NOTIFICATION基础数据结构
*/
typedef struct NOTIFICATION_BASE_T
{
    INotification           stInterface; /* notificition接口 */
    NOTIFICATION_PRIV_DATA_T stPrivData;  /* 私有数据 */
}NOTIFICATION_BASE_T;

#endif