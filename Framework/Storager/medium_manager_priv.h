
#ifndef _MEDIUM_MANAGER_PRIV_H_
#define _MEDIUM_MANAGER_PRIV_H_

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <ctype.h> 
#include <sys/un.h> 
#include <sys/ioctl.h> 
#include <sys/socket.h> 
#include <linux/types.h> 
#include <linux/netlink.h> 
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>



#include "sys_common.h"
#include "logger.h"
#include "sys_mutex.h"
#include "sys_pthread.h"
#include "sys_mem.h"
#include "sys_medium.h"
#include "sys_posix.h"
#include "sys_time.h"
#include "sys_list.h"
#include "sys_utils.h"
#include "sys_file.h" 
#include "sys_sem.h"

#include "Medium.h"
#include "Devmonitor.h"
#include "Notification.h"
#include "Storager.h"


#define MEDIUM_USER_SPACE_PROTECT_MEM_SIZE  SIZE_512MB
#define MEDIUM_MOUNT_PATH_LEN		128

#define MEDIUM_TF_DEV_PART_PATH	   		"/dev/mmcblk%dp%d"
#define MEDIUM_EMMC_DEV_PART_PATH		"/dev/mmcblk%dp%d"
#define MEDIUM_DISK_DEV_PART_PATH    	"/dev/sd%c%d"

#define MEDIUM_TF_DEV_NODE_PATH			"/dev/mmcblk%d"
#define MEDIUM_EMMC_DEV_NODE_PATH		"/dev/mmcblk%d"
#define MEDIUM_DISK_DEV_NODE_PATH		"/dev/sd%c"


typedef struct
{
     NODE_T                  node;
    MEDIUM_STATE_E        eStates;                                   //介质状态
	UINT8	              uMediumId; 									//介质ID
	UINT8	              uPartId;									    //分区ID
    MEDIUM_TYPE_E         uMeidumType;                                   //介质类型
    CHAR                  strRootPath[MEDIUM_MOUNT_PATH_LEN];           //分区根节点
	CHAR	              strDevPath[MEDIUM_MOUNT_PATH_LEN];			//设备节点
    CHAR  	              strMountPath[MEDIUM_MOUNT_PATH_LEN];		//挂载地址
}MEDIUM_PART_INFO_T;

//介质状态
typedef enum MEDIUM_STATE_ACTION_E
{
    MEDIUM_STATE_ACTION_REMOVE,
    MEDIUM_STATE_ACTION_ADD,
    MEDIUM_STATE_ACTION_CHANGE,
    MEDIUM_STATE_ACTION_UNKNOWN
}MEDIUM_STATE_ACTION_E;

typedef struct
{  
    INT                   iMediumNo;          // 介质ID
    INT                   iMediumPart;        // 介质分区号
    MEDIUM_STATE_ACTION_E   eAction;            // 介质插拔动作
    MEDIUM_TYPE_E           eType;
}MEDIUM_MANAGER_MSG_T;



//存储介质状态信息节点
typedef struct
{
     NODE_T                  node;
    INT                   iMediumNo;          // 介质ID号
    INT                   iPartNo;            // 介质分区号
    UINT                  uTotalSpaceSize;    // 总容量
    UINT                  uRemainSpaceSize;   // 剩余容量
    UINT                  uUserSpaceSize;     // 用户使用空间
    UINT                  uClusterSize;
    SYS_MEDIUM_FS_TYPE_E    eFileType;          // 文件系统
    MEDIUM_STATE_E          eStates;            // 介质状态
    MEDIUM_ACTION_E         eAction;            // 介质动作
    MEDIUM_TYPE_E           eType;              // 介质类型
}MEDIUM_INFO_NODE_T;


typedef struct
{
    MSG_Q_ID stMediumMsgID;                                 // 介质插拔状态消息队列
    LIST_T   MediumInfoList;                                // 介质信息链表
    MUTEX_ID MediumInfoMutex;                               // 介质信息链表锁
    LIST_T	 MediumPartInfoList;							// 介质分区信息链表
    MUTEX_ID  MediumformatMutex;                             //介质格式化的锁


}MEDIUM_MANAGER_PRIV_DATA_T;

//存储管理实例结构体
typedef struct
{
    IMediumManager             stInterface;//对外API接口
    MEDIUM_MANAGER_PRIV_DATA_T stPrivData; //私有数据结构体
}MEDIUM_MANAGER_BASE_T;



IMediumManager *mediummanager = NULL;


#endif



