#ifndef _NETWORK_PRIV_H_
#define _NETWORK_PRIV_H_

#include "Network.h"

#include "logger.h"
#include "sys_mem.h"
#include "sys_net.h"

INetwork * network;
/**
 * @struct    NETWORK_PRIV_DATA_T
 * @brief     NETWORK私有数据结构
*/
typedef struct NETWORK_PRIV_DATA_T
{
    INT iFd;            // 操作的文件描述符 
    CHAR  sDevName[128];       //以太网名称
}NETWORK_PRIV_DATA_T;

/**
 * @struct    NETWORK_BASE_T
 * @brief     NETWORK基础数据结构
*/
typedef struct NETWORK_BASE_T
{
    INetwork           stInterface; /* 串口操作接口 */
    NETWORK_PRIV_DATA_T stPrivData;  /* 私有数据 */
}NETWORK_BASE_T;


#endif