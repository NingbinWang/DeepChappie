#ifndef  _MEDIUM_H_
#define  _MEDIUM_H_
#include "Storager.h"
#ifdef __cplusplus
    extern "C" {
#endif

/* 存储介质类型 */
typedef enum
{
    MEDIUM_TYPE_TF = 0,                 // 普通TF卡
    MEDIUM_TYPE_HARD_DISK,              // 硬盘
    MEDIUM_TYPE_EMMC,                    // MMC
}MEDIUM_TYPE_E;

//格式化文件系统类型
typedef enum
{
    MEDIUM_FORMAT_DEFAULT,              // 默认格式,外界可不指定
    MEDIUM_FORMAT_FAT32,                // fat32
    MEDIUM_FORMAT_EXFAT,                // exfat
    MEDIUM_FORMAT_FAT16,                // fat16
    MEDIUM_FORMAT_NTFS,                 // ntfs
    MEDIUM_FORMAT_EXT4,                 /*ext4*/
    MEDIUM_FORMAT_UNKNOWN
}MEDIUM_FORMAT_TYPE_E;

typedef enum
{
    MEDIUM_STATE_NOT_EXIST = 0,             /*存储介质不存在*/
    MEDIUM_STATE_ABNORMAL = 1,              /*存储介质异常*/
    MEDIUM_STATE_NORMAL = 2,                /*存储介质正常*/
    MEDIUM_STATE_WRITE_SLOW = 3,            /*存储介质写入速度慢*/
    MEDIUM_STATE_MEM_FULL = 4,              /*存储卡满异常*/
    MEDIUM_STATE_FORMATTING = 5,            /*存储介质正在格式化*/
    MEDIUM_STATE_UNINITIALIZED = 6,         /*未初始化,需要格式化*/
    MEDIUM_STATE_CAPACITY_NO_SUPPORT = 7,   /*容量不支持*/
    MEDIUM_STATE_SYSTEM_NO_SUPPORT = 8,     /*文件系统不支持*/ 
    MEDIUM_STATE_CLUSTER_NO_SUPPORT = 9,    /*簇大小不支持 */
    MEDIUM_STATE_UNKNWON,
}MEDIUM_STATE_E;


//介质动作枚举
typedef enum
{
    MEDIUM_ACTION_UNKNOWN = 0x00,
    MEDIUM_ACTION_REMOVE,  //介质拔出操作动作
    MEDIUM_ACTION_ADD,     //介质插入操作动作
}MEDIUM_ACTION_E;

//存储介质信息结构体 包括 存储介质类型 存储状态  存储介质总大小 存储介质剩余大小
/*
typedef struct
{           
    MEDIUM_TYPE_E        eType;         //介质类型
    MEDIUM_STATE_E       eState;        //介质状态
    MEDIUM_FORMAT_TYPE_E eFileSystem;   //文件系统
    UINT               uTotalSize;    //总大小
    UINT               uRemainSize;   //剩余空间大小
    UINT               uUserSize;     //用户使用空间
	INT                iMediumId;     //介质id
    INT                iPartId;       //分区id
}MEDIUM_INFO_T;
*/


typedef struct IMediumManager IMediumManager;

struct IMediumManager{
    /**@fn         Init   
     * @brief      初始化参数
     * @param[in]  pIMediumManager   设备监听对象
     * @return     成功返回OK     失败返回错误码
     */
    int (*Init)(IMediumManager *pIMediumManager,unsigned char uMediumId,unsigned char uPartId);

    /**@fn	       Start	  
     * @brief	   开启介质管理
     * @param[in]  pIMediumManager 介质管理对象指针
     * @return	   成功返回OK  失败返回错误码
     */
    int (*Start)(IMediumManager *pIMediumManager);

    /**@fn	       Format	  
     * @brief	   介质格式化
     * @param[in]  pIMediumManager     介质管理对象指针
     * @param[in]  iMediumChan         介质通道号
     * @param[in]  eType               介质类型
     * @return	   成功返回OK  失败返回错误码
     */
    int (*Format)(IMediumManager *pIMediumManager,int iMediumChan, MEDIUM_FORMAT_TYPE_E eType);


};




void init_medium_manager_component(void);
IMediumManager *medium_manager_get_instance(void);

#ifdef __cplusplus
}
#endif/*__cplusplus*/


#endif
