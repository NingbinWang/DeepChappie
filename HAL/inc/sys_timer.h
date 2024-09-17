/**@file    sys_timer.h
 * @note    Hangzhou Hikvision Automotive Technology Co., Ltd. All Right Reserved.
 * @brief   定时器相关结构参数定义
 *          1.创建定时器
 *          2.设置定时器参数
 *          3.获取当前定时器参数
 *          4.删除定时器参数
 *          5.获取定时器超限个数
 * @author  wch
 * @date    2019-04-26
 * @version V1.0.0 2019-04-26 wch基本业务功能实现
 *          V1.1.0 2021-2-2   wch命名规范修改,标准化定义
 */
#ifndef _SYS_TIMER_INTERFACE_H_
#define _SYS_TIMER_INTERFACE_H_
#include "sys_common.h"

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

typedef LONG TIMER_ID;

/**@struct SYS_TIMER_SPEC_T 
 * @brief  
 */
typedef struct
{
    UINT64 uSecond;  /*单位s*/
    UINT64 uNSecond; /*单位ns*/
}SYS_TIMER_SPEC_T;

/**@struct SYS_TIMER_ITIMES_SPEC_T 
 * @brief  
 */
typedef struct
{
    SYS_TIMER_SPEC_T stInterval; /*循环定时时间参数*/
    SYS_TIMER_SPEC_T stValue;    /*首次定时时间参数*/
}SYS_TIMER_ITIMES_SPEC_T;

/**@enum   SYS_TIMER_SIG_E 
 * @brief  定时器信号枚举
 */
typedef enum
{
    SYS_TIMER_SIGEV_NONE, /*不希望产生信号*/
    SYS_TIMER_SIGEV_SIGNAL, /*SIGEV_SIGNAL(产生信号) */
    SYS_TIMER_SIGEV_THREAD, /*SIGEV_THREAD(新线程处理) */
    SYS_TIMER_SIGEV_THREAD_ID, /*SIGEV_THREAD_ID（指定线程处理） */
}SYS_TIMER_SIG_E;

/**@enum   SYS_TIMER_CLOCK_E 
 * @brief  时钟类型枚举
 */
typedef enum
{
    SYS_TIMER_CLOCK_REALTIME, /*可设置的全系统实时时钟。，计算从1970年1月1日午夜0点起的毫秒数*/
    SYS_TIMER_CLOCK_MONOTONIC,/*一种不可设置的单调递增时钟，从过去某个未指定的点开始测量时间，该点在系统启动后不会改变*/
    SYS_TIMER_CLOCK_PROCESS_CPUTIME_ID,/*一种时钟，用于测量（用户和系统）调用进程（所有线程）消耗的CPU时间*/
    SYS_TIMER_CLOCK_THREAD_CPUTIME_ID, /*测量（用户和系统）调用线程消耗的CPU时间的时钟。*/
    SYS_TIMER_CLOCK_BOOTTIME,/*和CLOCK_单调一样，这是一个单调递增的时钟。然而，虽然CLOCK_单调时钟不测量系统暂停时的时间，但CLOCK_BOOTTIME时钟确实包括暂停期间的时间系统暂停运行。这对于需要挂起感知的应用程序很有用。CLOCK_REALTIME不适用于此类应用，因为该时钟会受到系统不连续变化的影响时钟*/
    SYS_TIMER_CLOCK_REALTIME_ALARM,/*该时钟与SYS_TIMER_CLOCK_REALTIME类似，但如果暂停，则会唤醒系统。调用者必须具有CAP_WAKE_闹钟功能，才能根据该时钟设置计时器。*/
    SYS_TIMER_CLOCK_BOOTTIME_ALARM,/*该时钟与SYS_TIMER_CLOCK_BOOTTIME类似，但如果暂停，则会唤醒系统。调用者必须具有CAP_WAKE_闹钟功能，才能根据该时钟设置计时器。*/
}SYS_TIMER_CLOCK_E;

/**@union SYS_TIMER_SIGVAL_U 
 * @brief  
 */
typedef union
{
    INT iSigvalue;
    VOID *pSigValue;
}SYS_TIMER_SIGVAL_U;

/**@struct SYS_TIMER_SIG_EVENT_T 
 * @brief  
 */
typedef struct
{
    INT iNoitfy;  /*notification type*/
    INT iSignNo;  /*signal number*/
    SYS_TIMER_SIGVAL_U uSigval; /*signal value*/
    VOID (*NotifyFunc)(SYS_TIMER_SIGVAL_U uSigval);/*当SYS_TIMER_SIGEV_THREAD 生效*/
    UINT8 uRes[8];
}SYS_TIMER_SIG_EVENT_T;

/**@fn         sys_timer_create      
 * @brief      创建定时器
 * @param[in]  eClockID 时钟ID
 * @param[in]  pStSigEvent 信号事件信息
 * @param[out] pTimerID    时钟ID指针
 * @return     成功返回 OK,错误返回 ERROR
 */
INT sys_timer_create(SYS_TIMER_CLOCK_E eClockID, SYS_TIMER_SIG_EVENT_T *pStSigEvent, TIMER_ID *pTimerID);

/**@fn         sys_timer_settime      
 * @brief      设置定时器参数，arm启动或停止定时器
 * @param[in]  tTimerID 定时器ID
 * @param[in]  iFlag 　 参数标识
 * @param[in]  pStITimerSpecNew 新参数
 * @param[in]  pStITimerSpecOld 老参数
 * @return     成功返回 OK 错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT sys_timer_settime(TIMER_ID tTimerID, INT iFlag, SYS_TIMER_ITIMES_SPEC_T *pStITimerSpecNew, SYS_TIMER_ITIMES_SPEC_T *pStITimerSpecOld);

/**@fn         sys_timer_gettime      
 * @brief      获取计时器下次到期前的剩余时间，以及计时器的间隔设置
 * @param[in]  tTimerID 定时器ID
 * @param[in]  pStITimerSpec 参数
 * @return     成功返回 OK 错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT sys_timer_gettime(TIMER_ID tTimerID, SYS_TIMER_ITIMES_SPEC_T *pStITimerSpec);

/**@fn         sys_timer_delete      
 * @brief      删除定时器
 * @param[in]  tTimerID 定时器ID
 * @return     成功返回 OK 错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT sys_timer_delete(TIMER_ID tTimerID);

/**@fn         sys_timer_get_over_run      
 * @brief      获取定时器超时计数，返回最后一个计时器过期的溢出计数。
 * @param[in]  tTimerID 定时器ID
 * @return     成功返回次数 错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT sys_timer_get_over_run(TIMER_ID tTimerID);

/**@fn         sys_timer_fd_create      
 * @brief      创建定时器
 * @param[in]  eClockID 时钟ID
 * @param[in]  iFlag 　 参数标识
 * @return     成功返回 OK,错误返回 ERROR
 */
INT sys_timer_fd_create(SYS_TIMER_CLOCK_E eClockID, INT iFlag);

/**@fn         sys_timer_fd_settime      
 * @brief      设置定时器参数，arm启动或停止定时器
 * @param[in]  iFd      定时器文件描述符
 * @param[in]  iFlag 　 参数标识
 * @param[in]  pStITimerSpecNew 新参数
 * @param[in]  pStITimerSpecOld 老参数
 * @return     成功返回 OK 错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT sys_timer_fd_settime(INT iFd, INT iFlag, SYS_TIMER_ITIMES_SPEC_T *pStITimerSpecNew, SYS_TIMER_ITIMES_SPEC_T *pStITimerSpecOld);

/**@fn         sys_timer_fd_gettime      
 * @brief      获取计时器下次到期前的剩余时间，以及计时器的间隔设置
 * @param[in]  iFd      定时器文件描述符
 * @param[in]  pStITimerSpec 参数
 * @return     成功返回 OK 错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT sys_timer_fd_gettime(INT iFd, SYS_TIMER_ITIMES_SPEC_T *pStITimerSpec);

#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif/*__SYS_TIMER_INTERFACE_H__*/