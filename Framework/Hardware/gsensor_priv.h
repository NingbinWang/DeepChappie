#ifndef _GSENSOR_PRIV_H_
#define _GSENSOR_PRIV_H_



#include "gsensor.h"
#include "sys_imu.h"
#include "sys_pthread.h"
#include "sys_mem.h"
#include "sys_timer.h"
#include "sys_posix.h"
#include "sys_mqueue.h"//信号量



/*mode*/
typedef enum
{
    LOWPOWER_MODE    = 0,
    NORMALE_MODE     = 1,
    HIGHRESO_MODE    = 2,  /*压缩视频格式(Compressed Video Format), 如RFC H.264 CVF*/
    NOTALLOW_MODE    = -1,  /*时钟参考格式(Clock Reference Format)*/
}GSENSOR_MODE_E;

/**
* @enum GSENSOR_MANAGER_CMD_E
* @brief 控制指令类型枚举定义
 */
typedef enum
{
    GSENSOR_MANAGER_CMD_INIT,
    GSENSOR_MANAGER_CMD_START,   /*开始*/
    GSENSOR_MANAGER_CMD_STOP,    /*停止*/
}GSENSOR_MANAGER_CMD_E;

/**
* @enum GSENSOR_MANAGER_RESULT_E
* @brief 控制指令结果枚举定义
*/
typedef enum
{
    GSENSOR_MANAGER_RESULT_DEFAULT,
    GSENSOR_MANAGER_RESULT_SUCCESS,
    GSENSOR_MANAGER_RESULT_FAILED,
}GSENSOR_MANAGER_RESULT_E;

/**
* @struct GSENSOR_MANAGER_MSG_T
* @brief 控制指令结构体定义
*/
typedef struct
{
    GSENSOR_MANAGER_CMD_E    eCmd;
    GSENSOR_MANAGER_RESULT_E eResult;//结果
    UINT32 uRes;
}GSENSOR_MANAGER_MSG_T;

/**
* @struct GSENSOR_MANAGER_PRVI_DATA_T
* @brief 私有的一笔真实数据
*/
typedef struct 
{
    INT32 accx;     // 加速度 x mg
	INT32 accy;     // 加速度 y mg
	INT32 accz;     // 加速度 z mg
	INT32 gyrox;    // 角速度 x坐标值  
	INT32 gyroy;    // 角速度 y坐标值  
	INT32 gyroz;    // 角速度 z坐标值  
    INT32 magx;     // ecompass x坐标值 
    INT32 magy;     // ecompass y坐标值 
    INT32 magz;     // ecompass z坐标值 
}GSENSOR_MANAGER_PRVI_DATA_T;


/**
* @struct GSENSOR_MANAGER_PRVI_DATA_T
* @brief 私有的一笔真实数据
*/
typedef struct 
{
   UINT32                                      uAxis; //gyro的轴树
   UINT32                                      uPolltmr;     /* 轮训读取gyro信息定时器时间设置，单位ms */
   UINT32                                      uAccrng;    /* 设置accel 的满量程精确度 可选:±2G,±4G,±8G,±16G */
   UINT32                                      uGyrorng;     /* 设置gyro 的满量程精确度 可选: ±250，±500,±1000,±2000 */
   UINT32                                      uThreshold;  /* 设置碰撞唤醒阈值 0~255(0~1020mg,4mg LSB)*/
   UINT32                                      uWom;          /*  设置阈值唤醒功能，TRUE/FALSE*/
   UINT32                                      accvalidnum;     /* acc有效数据 */
   UINT32                                      gyrovalidnum;    /* gyro有效数据 */
   UINT32                                      acccoef;
   UINT32                                      gyrocoef;
}GSENSOR_MANAGER_PRVI_INFO_T;

/**
* @struct SENSOR_MANAGER_PRIV_DATA_T
* @brief 私有数据结构体定义
*/
typedef struct
{
   INT32                                       iFd;                //gsensor的文件描述符
   TASK_ID                                     stThreadID;         // gsenor线程
   CHAR                                        cGyroname[30];//GYRO的名字
   GSENSOR_MANAGER_PRVI_INFO_T                 info;         //信息
   GSENSOR_MODE_E                              eMode;         //模式
   MSG_Q_ID                                    stMsgID;     // 请求消息任务ID
   INT32                                       iWaitTime;      /* 超时时长 */
   INT32                                       iFlag;  
   GSENSOR_MANAGER_CMD_E                       eState;         /* gsensor目前状态 */
   GSENSOR_MANAGER_PRVI_DATA_T                 data;     
}GSENSOR_MANAGER_PRIV_DATA_T;


/**
* @struct GSENSOR_MANAGER_BASE_T
* @brief 对象实体结构体定义
*/
typedef struct
{
        IGsensor_manager stInterface;                     /* 对外API接口 */
        GSENSOR_MANAGER_PRIV_DATA_T stPrivData;   /* 私有数据结构体 */
}GSENSOR_MANAGER_BASE_T;

#define GYRO_NAME_LIS2DH12			"GYRO_LIS2DH12"



#endif
