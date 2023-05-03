#ifndef _SYS_GSENSOR_H_
#define _SYS_GSENSOR_H_
#include "sys_common.h"

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

typedef struct 
{
   	UINT16 accx;     // 加速度 x坐标值 
	UINT16 accy;     // 加速度 y坐标值 
	UINT16 accz;     // 加速度 z坐标值 
	UINT16 gyrox;    // 角速度 x坐标值  
	UINT16 gyroy;    // 角速度 y坐标值  
	UINT16 gyroz;    // 角速度 z坐标值
    UINT16 magx;     // ecompass x坐标值 
    UINT16 magy;     // ecompass y坐标值 
    UINT16 magz;     // ecompass z坐标值 
    FLOAT32 tmp;    // sensor的温度
	LONG ts_sec;	// 时间戳秒
	LONG ts_usec;	// 时间戳纳秒
}sensor_t;


typedef struct 
{
    CHAR gyroname[30];//GYRO的名字
    UINT32 AXIS; //gyro的轴树
    UINT32 poll_tmr;     /* 轮训读取gyro信息定时器时间设置，单位ms */
	UINT32 accrng;    /* 设置accel 的满量程精确度 可选:±2G,±4G,±8G,±16G */
	UINT32 gyrorng;     /* 设置gyro 的满量程精确度 可选: ±250，±500,±1000,±2000 */
	UINT32 threshold;  /* 设置碰撞唤醒阈值 0~255(0~1020mg,4mg LSB)*/
	UINT32 wom;          /*  设置阈值唤醒功能，TRUE/FALSE*/
	UINT32 accvalidnum;     /* acc有效数据 */
	UINT32 gyrovalidnum;    /* gyro有效数据 */
	UINT32 acccoef;
	UINT32 gyrocoef;
}sensorinfo_t;

/**@fn        sys_gsensor_open    
 * @brief     打开gesnor     
 * @return    成功返回 gsensor的文件描述符 错误返回 ERROR
 */
INT32 sys_gsensor_open();

/**@fn         sys_gsensor_close
 * @brief      关闭gsensor的节点
 * @param[in]  gsensor的文件描述符
 * @return     成功返回 OK 错误返回 ERROR
 */
INT32 sys_gsensor_close(const UINT32 fd);

/**@fn         sys_gsensor_setpolltime
 * @brief      gsensor的名字
 * @param[in]  gsensor的文件描述符
 * @param[in]  polling的时间
 * @return     成功返回 OK 错误返回 ERROR
 */
INT32 sys_gsensor_setpolltime(const UINT32 fd,const UINT32 polltime);


/**@fn        sys_gsensor_lowpower
 * @brief      gsensor的低功耗模式
 * @param[in]  gsensor的文件描述符
 * @return     成功返回 OK 错误返回 ERROR
 */
INT32 sys_gsensor_lowpower(const UINT32 fd);

/**@fn        sys_gsensor_nomalpower
 * @brief      gsensor的正常模式
 * @param[in]  gsensor的文件描述符
 * @return     成功返回 OK 错误返回 ERROR
 */
INT32 sys_gsensor_normalpower(const UINT32 fd);

/**@fn        sys_gsensor_clearint
 * @brief      gsensor的清中断
 * @param[in]  gsensor的文件描述符
 * @return     成功返回 OK 错误返回 ERROR
 */
INT32 sys_gsensor_clearint(const UINT32 fd);

/**@fn        sys_gsensor_setwakeup
 * @brief      gsensor的设置唤醒模式
 * @param[in]  gsensor的文件描述符
 * @return     成功返回 OK 错误返回 ERROR
 */
INT32 sys_gsensor_setwakeup(const UINT32 fd,const UINT32 w_threshold);


/**@fn        sys_gesnsor_getdata
 * @brief      gsensor的获取数据
 * @param[in]  gsensor的文件描述符
 * @return     成功返回 OK 错误返回 ERROR
 */
INT32 sys_gsensor_getdata(const UINT32 fd,sensor_t * sensordata);

/**@fn        sys_gsensor_getinfo
 * @brief      gsensor的获取芯片信息
 * @param[in]  gsensor的文件描述符
 * @return     成功返回 OK 错误返回 ERROR
 */

INT32 sys_gsensor_getinfo(const UINT32 fd,sensorinfo_t * info);

/**@fn        sys_gsensor_getconfig
 * @brief      gsensor的获取量程
 * @param[in]  gsensor的文件描述符
 * @return     成功返回 OK 错误返回 ERROR
 */

INT32 sys_gsensor_getconfig(const UINT32 fd,sensorinfo_t * info);

/**@fn        sys_gsensor_setconfig
 * @brief      gsensor的设置量程
 * @param[in]  gsensor的文件描述符
 * @return     成功返回 OK 错误返回 ERROR
 */

INT32 sys_gsensor_setconfig(const UINT32 fd,const UINT32 accrng);

/**@fn        sys_gsensor_getparam
 * @brief      gsensor的获取数据值
 * @param[in]  gsensor的文件描述符
 * @return     成功返回 OK 错误返回 ERROR
 */

INT32 sys_gsensor_getparam(const UINT32 fd,sensorinfo_t * info);

#ifdef __cplusplus
}
#endif/*__cplusplus*/


#endif
