#ifndef _STORAGER_MANAGER_PRIV_H_
#define _STORAGER_MANAGER_PRIV_H_

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
#include "logger.h"

#include "sys_medium.h"
#include "Storager.h"





/**@struct    STORAGER_MANAGER_PRIV_DATA_T
 * @brief     设备监听私有数据结构
 */
typedef struct
{
   SYS_MEDIUM_INFO_T mediainfo;
}STORAGER_MANAGER_PRIV_DATA_T;

/**@struct    STORAGER_MANAGER_BASE_T
 * @brief     UART基础数据结构
 */
typedef struct
{
    IStorager             stInterface; /* stroager接口 */
    STORAGER_MANAGER_PRIV_DATA_T stPrivData;  /* 私有数据 */
}STORAGER_MANAGER_BASE_T;


IStorager *storagermanager = NULL;


#endif