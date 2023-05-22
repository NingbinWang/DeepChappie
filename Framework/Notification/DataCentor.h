#ifndef _DATACENTOR_H_
#define _DATACENTOR_H_

#include "sys_common.h"
#include "sys_mem.h"
#include "PingPongBuffer.h"

/* Event type enumeration */
typedef enum
{
        EVENT_NONE,
        EVENT_PUB_PUBLISH, // Publisher posted information
        EVENT_SUB_PULL,    // Subscriber data pull request
        EVENT_NOTIFY,      // Subscribers send notifications to publishers
        EVENT_TIMER,       // Timed event
        _EVENT_LAST
} EventCode_E;

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
} ErrorCode_E;




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

typedef int (*EventCallback_t)(EventParam_t* param);


#define DATACENTORIDMAX 128
/**
 * @struct   broker的节点
 * @brief    开始的数据
*/
typedef struct Broker_T
{
   CHAR ID[DATACENTORIDMAX];//Unique data ID
   PingPongBuffer_t BufferManager;
   EventCallback_t eventCallback;
   struct Broker_T *next;//指向直接下一个指针
}Broker_Node;

typedef struct DataCentor_T
{
    CHAR name[DATACENTORIDMAX]; //datacentor name
    UINT32 BufferSize;
    UINT32 len;
    struct Broker_T *next;//指向直接下一个指针
}DataCentor_Header;


DataCentor_Header* notifybroker_init(const char* master,UINT32 pingpongbuffsize);
void notifybroker_pushback(DataCentor_Header *head,const char* id);

int notifybroker_remove(DataCentor_Header *head,const char* id)



#endif