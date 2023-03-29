/*
 * @file gyro.h  
 * @brief GYRO统一接口。  
 *
 * @author alexking
 * @date 2020/04/19  
 *
 * @note 下面的note和warning为可选项目  
 * @note 这里填写本文件的详细功能描述和注解  
 * @note 历史记录：  
 * @note V1.0.0 创建  
 *
 * @warning 这里填写本文件相关的警告信息  
 * encoding UTF-8  
 */

#ifndef _SYS_GSENSOR_PRIV_H_
#define _SYS_GSENSOR_PRIV_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <math.h>
#include <sys/time.h>
#include "sys_common.h"
#include "sys_common_inside.h"

#define GYRO_DEVICE_NAME "/dev/gyro"
#define GYRO_IOC_MAGIC               'Y' 
#define LEN 32
#define WOM_ENABLE 1
#define WOM_DISBALE 0

struct gyro_info
{
    char name[LEN];
    unsigned int type;
};

struct gyro_value {
	unsigned short accel_x;     /* 加速度 x坐标值  */
	unsigned short accel_y;     /* 加速度 y坐标值  */ 
	unsigned short accel_z;     /* 加速度 z坐标值  */
	unsigned short gyro_x;      /* 角速度 x坐标值  */
	unsigned short gyro_y;      /* 角速度 y坐标值  */
	unsigned short gyro_z;      /* 角速度 z坐标值  */
	long ts_sec;			    /* 时间戳秒 */
	long ts_usec;		        /* 时间戳纳秒 */
};

struct gyro_config {
	unsigned int poll_tmr;     /* 轮训读取gyro信息定时器时间设置，单位ms */
	unsigned int accel_rng;    /* 设置accel 的满量程精确度 可选:±2G,±4G,±8G,±16G */
	unsigned int gyro_rng;     /* 设置gyro 的满量程精确度 可选: ±250，±500,±1000,±2000 */
	unsigned int w_threshold;  /* 设置碰撞唤醒阈值 0~255(0~1020mg,4mg LSB)*/
	unsigned int wom;          /*  设置阈值唤醒功能，TRUE/FALSE*/
};

struct gyro_INTconfig
{
	unsigned int int_clear;    /*  清中断标志位 */
    unsigned int int_src;      /*  获取中断源   */
};

enum gyro_type
{
    GYRO_TYPE_DEFAULT = 0,
    GYRO_BYTE_THREE_AXIS,
    GYRO_TYPE_SIX_AXIS,
	GYRO_TYPE_NINE_AXIS,
};

struct gyro_param
{
	unsigned int accvalidnum;     /* acc有效数据 */
	unsigned int gyrovalidnum;    /* gyro有效数据 */
	unsigned int acccoef;
	unsigned int gyrocoef;
}

#define HAT_GYRO_INIT           _IOR(GYRO_IOC_MAGIC, 1, struct gyro_info *)
#define HAT_GYRO_TMR_SET        _IOW(GYRO_IOC_MAGIC, 2, struct gyro_config *)
#define HAT_GYRO_DATA_FLUSH     _IOW(GYRO_IOC_MAGIC, 3, struct gyro_config *)
#define HAT_GYRO_DATA_READ      _IOR(GYRO_IOC_MAGIC, 4, struct gyro_value *)
#define HAT_GYRO_READ_CONFIG    _IOR(GYRO_IOC_MAGIC, 5, struct gyro_config *)
#define HAT_GYRO_SET_RNG        _IOW(GYRO_IOC_MAGIC, 6, struct gyro_config *)
#define HAT_GYRO_WOM_SET        _IOW(GYRO_IOC_MAGIC, 7, struct gyro_config *)
#define HAT_GYRO_CLEAR_INT      _IOW(GYRO_IOC_MAGIC, 8, struct gyro_INTconfig *)
#define HAT_GYRO_GET_INT_SRC    _IOR(GYRO_IOC_MAGIC, 9, struct gyro_INTconfig *)
#define HAT_GYRO_LOW_POWER      _IOR(GYRO_IOC_MAGIC, 10, struct gyro_info *)
#define HAT_GYRO_NORMAL_POWER   _IOR(GYRO_IOC_MAGIC, 11, struct gyro_info *)
#define HAT_GYRO_GET_PARAM      _IOR(GYRO_IOC_MAGIC, 12, struct gyro_param *)

#endif