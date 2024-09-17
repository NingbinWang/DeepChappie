#include"sys_timer_priv.h"

typedef void (*SigEnvNotifyFunction)(union sigval);

/**@fn         sys_timer_create      
 * @brief      创建定时器
 * @param[in]  eClockID 时钟ID
 * @param[in]  pStSigEvent 信号事件信息
 * @param[out] pTimerID    时钟ID指针
 * @return     成功返回 OK,错误返回 ERROR
 */
INT sys_timer_create(SYS_TIMER_CLOCK_E eClockID, SYS_TIMER_SIG_EVENT_T *pStSigEvent, TIMER_ID *pTimerID)
{
    INT iRet = ERROR;
    timer_t tTimerID;
    clockid_t cClockID = 0;
    struct sigevent stSigEvent;
    if(NULL == pStSigEvent || NULL == pTimerID)
    {
        return iRet;
    }
    switch (eClockID)
    {
    case SYS_TIMER_CLOCK_REALTIME:
        cClockID = CLOCK_REALTIME;
        break;
    case SYS_TIMER_CLOCK_MONOTONIC:
        cClockID = CLOCK_MONOTONIC;
        break;
    case SYS_TIMER_CLOCK_PROCESS_CPUTIME_ID:
        cClockID = CLOCK_PROCESS_CPUTIME_ID;
        break;
    case SYS_TIMER_CLOCK_THREAD_CPUTIME_ID:
        cClockID = CLOCK_THREAD_CPUTIME_ID;
        break;
    case SYS_TIMER_CLOCK_BOOTTIME:
        cClockID = CLOCK_BOOTTIME;
        break;
    case SYS_TIMER_CLOCK_REALTIME_ALARM:
        cClockID = CLOCK_REALTIME_ALARM;
        break;
    case SYS_TIMER_CLOCK_BOOTTIME_ALARM:
        cClockID = CLOCK_BOOTTIME_ALARM;
        break;
    default:
        break;
    }
    memset(&stSigEvent, 0, sizeof(stSigEvent));
    stSigEvent.sigev_notify_function = (SigEnvNotifyFunction)pStSigEvent->NotifyFunc;
    stSigEvent.sigev_signo  = pStSigEvent->iSignNo;
    stSigEvent.sigev_value.sival_ptr = pStSigEvent->uSigval.pSigValue;
    stSigEvent.sigev_notify = pStSigEvent->iNoitfy;
    iRet = timer_create(cClockID, &stSigEvent, &tTimerID);
    if(iRet < 0)
    {
        return iRet;
    }
    SYS_COMMON_INFO("Timer Create ID :0x%lx \n",(LONG)tTimerID);
    *pTimerID = (TIMER_ID)tTimerID;
    return iRet;
}

/**@fn         sys_timer_settime      
 * @brief      设置定时器参数，arm启动或停止定时器
 * @param[in]  tTimerID 定时器ID
 * @param[in]  iFlag 　 参数标识
 * @param[in]  pStITimerSpecNew 新参数
 * @param[in]  pStITimerSpecOld 老参数
 * @return     成功返回 OK 错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT sys_timer_settime(TIMER_ID tTimerID, INT iFlag, SYS_TIMER_ITIMES_SPEC_T *pStITimerSpecNew, SYS_TIMER_ITIMES_SPEC_T *pStITimerSpecOld)
{
    INT iRet = ERROR;
    struct itimerspec iTimerSpecNew;
    timer_t tTimerId = (timer_t)tTimerID;
    if(NULL == pStITimerSpecNew)
    {
        return iRet;
    }
    iTimerSpecNew.it_value.tv_sec = pStITimerSpecNew->stValue.uSecond;
    iTimerSpecNew.it_value.tv_nsec = pStITimerSpecNew->stValue.uNSecond;
    iTimerSpecNew.it_interval.tv_sec = pStITimerSpecNew->stInterval.uSecond;
    iTimerSpecNew.it_interval.tv_nsec = pStITimerSpecNew->stInterval.uNSecond;
    iRet = timer_settime(tTimerId, iFlag, &iTimerSpecNew, NULL);
    if(iRet < 0)
    {
        return iRet;
    }
    return iRet;
}

/**@fn         sys_timer_gettime      
 * @brief      获取清时期参数定时器参数
 * @param[in]  tTimerID 定时器ID
 * @param[in]  pStITimerSpec 参数
 * @return     成功返回 OK 错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT sys_timer_gettime(TIMER_ID tTimerID, SYS_TIMER_ITIMES_SPEC_T *pStITimerSpec)
{
    INT iRet = ERROR;
    struct itimerspec iTimerSpec;
    timer_t tTimerId = (timer_t)tTimerID;
    if(NULL == pStITimerSpec)
    {
        return iRet;
    }
    
    iRet = timer_gettime(tTimerId, &iTimerSpec);
    if(iRet < 0)
    {
        return iRet;
    }
    pStITimerSpec->stValue.uSecond = iTimerSpec.it_value.tv_sec;
    pStITimerSpec->stValue.uNSecond = iTimerSpec.it_value.tv_nsec;
    pStITimerSpec->stInterval.uSecond = iTimerSpec.it_interval.tv_sec;
    pStITimerSpec->stInterval.uNSecond = iTimerSpec.it_interval.tv_nsec;
    return iRet;
}

/**@fn         sys_timer_delete      
 * @brief      删除定时器
 * @param[in]  tTimerID 定时器ID
 * @return     成功返回 OK 错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT sys_timer_delete(TIMER_ID tTimerID)
{
    INT iRet = ERROR;
    timer_t tTimerId = (timer_t)tTimerID;
    iRet = timer_delete(tTimerId);
    return iRet;
}

/**@fn         sys_timer_get_over_run      
 * @brief      获取定时器超时技术
 * @param[in]  tTimerID 定时器ID
 * @return     成功返回次数 错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT sys_timer_get_over_run(TIMER_ID tTimerID)
{
    INT iRet = ERROR;
    timer_t tTimerId = (timer_t)tTimerID;
    iRet = timer_getoverrun(tTimerId);
    return iRet;
}