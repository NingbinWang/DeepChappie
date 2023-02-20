#ifndef _AVB_1722_TALKER_PRIV_H_
#define _AVB_1722_TALKER_PRIV_H_
#include "sys_common.h"
#include  "avb_1722_talker.h"
#include "avtp_adaptor.h"



#ifdef LOGGER
#define AVB_ERROR SYS_COMMON_ERROR
#define AVB_INFO SYS_COMMON_INFO
#define AVB_WARN SYS_COMMON_WARN
#else
#define _AVB_LOG(format, ...)      printf("[AVB]"format"\r\n", ##__VA_ARGS__)
#define AVB_INFO(format, ...)  _AVB_LOG("[Info] "format, ##__VA_ARGS__)
#define AVB_WARN(format, ...)  _AVB_LOG("[Warn] "format, ##__VA_ARGS__)
#define AVB_ERROR(format, ...) _AVB_LOG("[Error] "format, ##__VA_ARGS__)
#endif

 /**
* @struct AVB_1722_TALKER_FRAME_INFO_T
* @brief 帧数据结构体定义
*/
 typedef struct 
{
    BOOL         bFirstFrame;      /* 首帧标识 */
    UINT8        uRes[3];
    UINT8        *pFrameBuf;      /* 帧缓冲区 */
    UINT32       uFrameBufSize;   /* 帧缓冲区大小 */
}AVB_1722_TALKER_FRAME_INFO_T;  
    
/**
* @enum AVB_1722_STREAM_TYPE_E
* @brief 控制指令类型枚举定义
 */
typedef enum
{
    AVB_STREAM_TYPE_H264VIDEO = 0,
}AVB_STREAM_TYPE_E;

/**
* @struct AVB_1722_TALKER_TRANSMIT_PARAM_T
* @brief
*/
 typedef struct 
{
    INT32         iSock;      
    CHAR          aInterfaceName[128];
    CHAR          aMacAddr[256];
    INT32         iProtocol;
}AVB_1722_TALKER_TRANSMIT_PARAM_T;  
    

/**
* @struct AVB_1722_TALKER_PRIV_DATA_T
* @brief 私有数据结构体定义
*/
typedef struct
{
    struct sockaddr_ll                  stDstSockAddr;    /* 目的socket */
    TASK_ID                             stThreadID;     /* 推流线程 */
    INT32                               iWaitTime;        /* 超时时长 */
    INT32                               iSock;           /* 套接字句柄 */
    UINT8                               uSeq;            /* avtp包序号 */
    UINT32                              uMaxPayloadLen;  /* avtp报文payload长度 */ 
    UINT32                              uChannel;  
    MSG_Q_ID                            stReqMsgID;     /* 请求消息任务ID */
    MSG_Q_ID                            stRespMsgID;    /* 响应消息任务ID */
    AVB_1722_TALKER_TRANSMIT_PARAM_T    stTransmitParam;
    AVB_STREAM_TYPE_E                   uStreamType;
    INT32 iFlag;                 
}AVB_1722_TALKER_PRIV_DATA_T;

/**
* @struct AVB_1722_TALKER_BASE_T
* @brief 对象实体结构体定义
*/
typedef struct
{
        IAVB1722Talker stInterface;                     /* 对外API接口 */
        AVB_1722_TALKER_PRIV_DATA_T stPrivData;   /* 私有数据结构体 */
}AVB_1722_TALKER_BASE_T;







#endif