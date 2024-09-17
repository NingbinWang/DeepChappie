#ifndef _DEVMONITOR_PRIV_H_
#define _DEVMONITOR_PRIV_H_
#include "sys_common.h"
#include "sys_posix.h"
#include "sys_time.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <ctype.h> 
#include <sys/un.h> 
#include <sys/ioctl.h> 
#include <sys/socket.h> 
#include <linux/types.h> 
#include <linux/netlink.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "sys_mem.h"
#include "Devmonitor.h"
#include "logger.h"


IDevMonitor *devmonitor = NULL;

#define DEV_MONITOR_ACTION_SYMBOL_REMOVE "remove"
#define DEV_MONITOR_ACTION_SYMBOL_ADD    "add"
#define DEV_MONITOR_ACTION_SYMBOL_CHANGE "change"

/**@struct    DEV_MONITOR_PRIV_DATA_T
 * @brief     设备监听私有数据结构
 */
typedef struct
{
    BOOL  bExist;
    UINT8 uRes[3];
    INT iFd;            /* 操作的文件描述符 */
    CHAR strBuf[SIZE_4KB]; /*接收缓冲区*/
    DevActionCallBack pCallBack; /*暂时先支持设备监听回调函数*/
}DEV_MONITOR_PRIV_DATA_T;

/**@struct    DEV_MONITOR_BASE_T
 * @brief     DEV MONITOR基础数据结构
 */
typedef struct
{
    IDevMonitor             stInterface; /* devmontor接口 */
    DEV_MONITOR_PRIV_DATA_T stPrivData;  /* 私有数据 */
}DEV_MONITOR_BASE_T;

#endif