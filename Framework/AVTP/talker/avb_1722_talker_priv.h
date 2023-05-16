#ifndef _AVB_1722_TALKER_PRIV_H_
#define _AVB_1722_TALKER_PRIV_H_

#include  "avb_1722_talker.h"
#include "avtp_adaptor.h"
#include "avtp_pdu.h"
#include "logger.h"

#define AVB_1722_TALKER_FRAME_BUFF_SIZE (0x00080000u) /* 帧读取缓冲区大小 默认SIZE_512KB */


/**
* @enum AVB_1722_TALKER_CMD_E
* @brief 控制指令类型枚举定义
*/
typedef enum
{
    AVB_1722_TALKER_CMD_INIT,
    AVB_1722_TALKER_CMD_START,   /*开始*/
    AVB_1722_TALKER_CMD_STOP,    /*停止*/
}AVB_1722_TALKER_CMD_E;

/**
* @enum AVB_1722_TALKER_RESULT_E
* @brief 控制指令结果枚举定义
*/
typedef enum
{
    AVB_1722_TALKER_RESULT_DEFAULT,
    AVB_1722_TALKER_RESULT_SUCCESS,
    AVB_1722_TALKER_RESULT_FAILED,
}AVB_1722_TALKER_RESULT_E;

/**
* @struct AVB_1722_TALKER_MSG_T
* @brief 控制指令结构体定义
*/
typedef struct
{
    AVB_1722_TALKER_CMD_E    eCmd;
    AVB_1722_TALKER_RESULT_E eResult;//结果
    UINT32 uRes;
}AVB_1722_TALKER_MSG_T;



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
    AVB_STREAM_TYPE_UNKNOW = 99,
}AVB_STREAM_TYPE_E;

/**
* @struct AVB_1722_TALKER_TRANSMIT_PARAM_T
* @brief 
*/
 typedef struct 
{
     INT8 iSockPriority;   //传输套接字优先级，0~6
     UINT8 uIgnoreSyncLoss; //忽略gPTP的sync报文丢失
     UINT8 aMacAddr[6]; //talker:目的mac地址，listener:源（本地）mac地址
     CHAR  aInterfaceName[32];//网卡名称
     UINT32 uMaxTransmitTime;//最大传输时间 单位ms
     UINT32 uMaxPayloadLen;//avtp报文payload最大长度
     UINT64 uStreamID;       //SteamID
     UINT32 uRes[4];
}AVB_1722_TALKER_TRANSMIT_PARAM_T;  
    

/**
* @struct AVB_1722_TALKER_PRIV_DATA_T
* @brief 私有数据结构体定义
*/
typedef struct
{
    IAvtpPdu                            *pIAvtpPdu;        /* Avtpdu对象单例指针 */
    struct sockaddr_ll                  stDstSockAddr;    /* 目的socket */
    TASK_ID                             stThreadID;     /* 推流线程 */
    INT32                               iWaitTime;        /* 超时时长 */
    INT32                               iSock;           /* 套接字句柄 */
    UINT8                               uSeq;            /* avtp包序号 */
    UINT32                              uMaxPayloadLen;  /* avtp报文payload长度 */ 
    UINT32                              uChannel;  
    MSG_Q_ID                            stReqMsgID;     /* 请求消息任务ID */
    MSG_Q_ID                            stRespMsgID;    /* 响应消息任务ID */
    AVB_1722_TALKER_TRANSMIT_PARAM_T    stTransmitParam;  //配置参数
    AVB_STREAM_TYPE_E                   uStreamType;  
    AVB_1722_TALKER_FRAME_INFO_T        st1722FrameInfo;    /* 读取的帧信息 */            
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