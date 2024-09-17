#include"sys_time_priv.h"

/**@fn         sys_time_get_msec      
 * @brief      获取系统启动后运行的毫秒数
 * @param[out] uMSecond 毫秒数指针
 * @return     成功返回 OK 错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT sys_time_get_msec(UINT64 *puMSecond)
{
    INT iRet = ERROR;
    struct timespec stTimeRun = {0};
    memset(&stTimeRun,0,sizeof(stTimeRun));
    if(NULL == puMSecond)
    {
        return iRet;
    }
    iRet = clock_gettime(CLOCK_MONOTONIC, &stTimeRun);
    if(iRet < 0)
    {
        return iRet;
    }
    *puMSecond  = stTimeRun.tv_sec * 1000 + stTimeRun.tv_nsec/1000000;
    return iRet;      
}

/**@fn         sys_time_get_sec      
 * @brief      获取系统启动后运行的秒数
 * @param[out] uMSecond 秒数指针
 * @return     成功返回 OK 错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT sys_time_get_sec(UINT *puSecond)
{
    INT iRet = ERROR;
    struct timespec stTimeRun = {0};
    memset(&stTimeRun,0,sizeof(stTimeRun));
    if(NULL == puSecond)
    {
        return iRet;
    }
    iRet = clock_gettime(CLOCK_MONOTONIC, &stTimeRun);
    if(iRet < 0)
    {
        return iRet;
    }
    *puSecond  = stTimeRun.tv_sec;
    return iRet;      
}

/**@fn         sys_time_get_utc_sec      
 * @brief      获取系统当前从1970-1-1 00:00:00的绝对时间
 * @param[out] puSecond 秒数指针
 * @return     成功返回 OK, 整数值 错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT sys_time_get_utc_sec(UINT *puSecond)
{   
    INT iRet = ERROR;
    struct timeval stTimeVal = {0};
    if(NULL == puSecond)
    {
        return iRet;
    }
    iRet = gettimeofday(&stTimeVal,NULL);
    if(iRet < 0)
    {
        return iRet;
    }
    *puSecond = (UINT)stTimeVal.tv_sec;
    return iRet; 
}

/**@fn         sys_time_get_utc_msec      
 * @brief      获取系统UTC时间
 * @param[out] puUtcMSec UTC时间毫秒
 * @return     成功返回 OK  错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT sys_time_get_utc_msec(UINT64 *puUtcMSec)
{   
    INT iRet = ERROR;
    struct timeval stTimeVal = {0};
    if(NULL == puUtcMSec)
    {
        return iRet;
    }
    iRet = gettimeofday(&stTimeVal,NULL);
    if(iRet < 0)
    {
        return iRet;
    }
    *puUtcMSec = (1000000 * stTimeVal.tv_sec+stTimeVal.tv_usec)/1000;
    //SYS_COMMON_ERROR("uUtcSec :%lld :%lld \n",uUtcSec,tz.tz_minuteswest);
    return iRet;
}
/* @fn	      sys_time_convert_tm	  
 * @brief	  时间转换系统标准接口
 * @param[in] pStDateTime 时间结构体指针
 * @return    成功返回 OK
 * @return    错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT sys_time_convert_tm(const SYS_DATE_TIME_T *pStDateTime,struct tm *pStTime)
{
    INT iRet = ERROR;
    if(!pStDateTime || !pStTime)
    {
        return iRet;
    }
    if (pStDateTime->uYear < 1970 ||
        pStDateTime->uMonth < 1 ||
        pStDateTime->uMonth > 12 ||
        pStDateTime->uDay < 1 ||
        pStDateTime->uDay > 31 ||
        pStDateTime->uWeek > 6 ||
        pStDateTime->uHour > 23 ||
        pStDateTime->uMinute > 59 ||
        pStDateTime->uSec > 59)
    {   
        SYS_COMMON_ERROR("error formate time :%04d-%02d-%02d  %02d:%02d:%02d \n",pStDateTime->uYear,pStDateTime->uMonth,pStDateTime->uDay,pStDateTime->uHour,pStDateTime->uMinute,pStDateTime->uSec);
        return ERROR;
    }
    
    pStTime->tm_year=  pStDateTime->uYear - 1900; //注意struct tm结构中的年是从1970年开始的计数
    pStTime->tm_mon =  pStDateTime->uMonth - 1; //注意struct tm结构中的月份是从0开始的
    pStTime->tm_mday=  pStDateTime->uDay;
    pStTime->tm_hour= pStDateTime->uHour;
    pStTime->tm_min = pStDateTime->uMinute;
    pStTime->tm_sec =  pStDateTime->uSec;
    return OK;
}


/* @fn	      sys_time_set_hw	  
 * @brief	  设置硬件时间
 * @param[in] stTime  时间
 * @return    成功返回 秒数, 整数值
 * @return    错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT sys_time_set_hw(time_t stTime)
{
    INT iRet = ERROR;
    INT iRtcFd = -1;
    iRet = open("/dev/rtc0", O_WRONLY);
    if(iRet < 0) 
    {
        return iRet;
    }
    iRtcFd = iRet;
    iRet = ioctl(iRtcFd, RTC_SET_TIME, gmtime(&stTime));
    if(iRet < 0) 
    {
        close(iRtcFd);
        return iRet;
    }
    close(iRtcFd);
    return iRet;
}

/**@fn         sys_time_set_struct      
 * @brief      时间设置
 * @param[in]  pStDateTime 时间结构体指针
 * @return     成功返回 OK 错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT sys_time_set_in_struct(const SYS_DATE_TIME_T *pStDateTime)
{   
#ifdef ANDROID
    SYS_COMMON_ERROR("no support stime in andriod \n");
    return ERROR;
#else
    INT iRet = ERROR;
    struct tm stTime= {0};
    time_t stCurrentTime;
    if (pStDateTime == NULL)
    {
        SYS_COMMON_ERROR("invaild parma \n");
        return iRet;
    }
    iRet = sys_time_convert_tm(pStDateTime,&stTime);
    if(iRet  < 0)
    {
        return iRet;
    }

    //将struct tm结构时间转换成GMT时间time_t
    stCurrentTime = mktime(&stTime);
    if(stCurrentTime < 0)
    {
        return iRet;
    }
    stime(&stCurrentTime);
    sys_time_set_hw(stCurrentTime);
    return OK;
#endif
}

/**@fn         sys_time_get_in_struct      
 * @brief      获取系统日期和时间
 * @param[out] pStDateTime  结构体指针
 * @return     成功返回 OK 错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT sys_time_get_in_struct(SYS_DATE_TIME_T *pStDateTime)
{
    time_t now;
    struct tm stTimeNow;
    if (pStDateTime == NULL)
    {
        return ERROR;
    }

    time(&now);
    if (localtime_r(&now, &stTimeNow) != NULL)
    {
        pStDateTime->uYear   = stTimeNow.tm_year + 1900;
        pStDateTime->uMonth  = stTimeNow.tm_mon + 1;
        pStDateTime->uDay    = stTimeNow.tm_mday;
        pStDateTime->uWeek   = stTimeNow.tm_wday;
        pStDateTime->uHour   = stTimeNow.tm_hour;
        pStDateTime->uMinute = stTimeNow.tm_min;
        pStDateTime->uSec    = stTimeNow.tm_sec;
    }
    else
    {
        SYS_COMMON_ERROR("get current time failed.\n");
        return ERROR;
    }
    //SYS_COMMON_INFO("%04d-%02d-%02d :%02d:%02d:%02d week:%d\n",pStDateTime->uYear,pStDateTime->uMonth,pStDateTime->uDay,pStDateTime->uHour,pStDateTime->uMinute,pStDateTime->uSec,pStDateTime->uWeek);
    return OK;
}
/**@fn         sys_time_convert_struct_to_value      
 * @brief      转换日期时间为从1970年1月1日0点到现在的秒数
 * @param[in]  pStDateTime 时间结构体指针
 * @param[out] uUtcTime     utc时间结构体指针
 * @return     成功返回 OK 错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT sys_time_convert_struct_to_value(const SYS_DATE_TIME_T *pStDateTime,UINT *uUtcTime)
{   
    struct tm stTime = {0};
    if(NULL == pStDateTime || uUtcTime == NULL)
    {
        SYS_COMMON_ERROR("param  is invaild \n");
        return ERROR;
    }
	
    stTime.tm_year=  pStDateTime->uYear - 1900; //注意struct tm结构中的年是从1900年开始的计数
    stTime.tm_mon =  pStDateTime->uMonth - 1; //注意struct tm结构中的月份是从0开始的
    stTime.tm_mday=  pStDateTime->uDay;
    stTime.tm_hour= pStDateTime->uHour;
    stTime.tm_min = pStDateTime->uMinute;
    stTime.tm_sec = pStDateTime->uSec;
    //将struct tm结构时间转换成GMT时间time_t
    *uUtcTime = mktime(&stTime);
	// SYS_COMMON_ERROR("%4d-%02d-%02d %02d:%02d:%02d convert :%d \n", pStDateTime->uYear, pStDateTime->uMonth ,pStDateTime->uDay, 
    //   pStDateTime->uHour, pStDateTime->uMinute, pStDateTime->uSec,*uUtcTime);
    return *uUtcTime;         /* finally seconds */
}

/**@fn         sys_time_convert_value_to_struct      
 * @brief      将从1970年1月1日0点到现在的秒转化成datetime结构
 * @param[in]  uUtcTime      1970年1月1日0点到现在的秒数
 * @param[in]  pStDateTime   utc时间结构体指针年月日时分秒
 * @return     成功返回 OK   错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT sys_time_convert_value_to_struct(const UINT uUtcTime, SYS_DATE_TIME_T *pStDateTime)
{
    struct tm stTimeNow;
    time_t uTimeTemp = 0;

    if (pStDateTime == NULL)
    {
        return ERROR;
    }
    uTimeTemp = uUtcTime;
    if (localtime_r(&uTimeTemp, &stTimeNow) != NULL)
    {
        pStDateTime->uYear   = stTimeNow.tm_year + 1900;
        pStDateTime->uMonth  = stTimeNow.tm_mon + 1;
        pStDateTime->uDay    = stTimeNow.tm_mday;
        pStDateTime->uWeek   = stTimeNow.tm_wday;
        pStDateTime->uHour   = stTimeNow.tm_hour;
        pStDateTime->uMinute = stTimeNow.tm_min;
        pStDateTime->uSec    = stTimeNow.tm_sec;
    }
    else
    {
        return ERROR;
    }
    //SYS_COMMON_INFO("uUtcTime:%lld :%lld %04d-%02d-%02d :%02d:%02d:%02d week:%d\n",uUtcTime,tz.tz_minuteswest,pStDateTime->uYear,pStDateTime->uMonth,pStDateTime->uDay,pStDateTime->uHour,pStDateTime->uMinute,pStDateTime->uSec,pStDateTime->uWeek);
    return OK;
}

/**@fn         sys_time_set_utc_value      
 * @brief      设置系统当前绝对时间
 * @param[in]  uUtcSec       UTC时间ms
 * @return     成功返回 ok  错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT sys_time_set_utc_msec(UINT64 uUtcSec)
{
#ifdef ANDROID
    SYS_COMMON_ERROR("no support stime in andriod \n");
    return ERROR;
#else
    INT iRet = ERROR;
    time_t uTime = 0;
    iRet = stime((time_t *)&uTime);
    uUtcSec = (UINT64)uTime;
    return iRet;
#endif
}

/**@fn         sys_time_convert_value_to_str      
 * @brief      将时间值转化为字符串
 * @param[in]  uTime   绝对时间，从1970年开始计算的秒数
 * @param[out] strData 存储时间的字符串
 * @param[in]  uSize   存储时间的字符串长度
 * @return     成功返回OK，失败返回ERROR
 */
INT sys_time_convert_value_to_str(UINT uTime, CHAR *strData, UINT uSize)
{
	SYS_DATE_TIME_T stDateTime = {0};
    if(NULL == strData || 0 == uSize)
    {
        return ERROR;
    }
	
    memset(strData, 0, uSize);
    if (OK != sys_time_convert_value_to_struct(uTime,&stDateTime))
    {
        return ERROR;
    }
    
    (void)snprintf(strData, uSize, "%04d-%02d-%02d %02d:%02d:%02d",
             stDateTime.uYear , stDateTime.uMonth, stDateTime.uDay,
             stDateTime.uHour , stDateTime.uMinute, stDateTime.uSec);
	
    strData[uSize - 1] = 0;
    return OK;
}

/**@fn         sys_time_convert_str_to_value      
 * @brief      将字符串转化为时间值
 * @param[in]  strData 存储时间的字符串
 * @param[out] uTime 绝对时间，从1970年开始计算的秒数
 * @return     成功返回OK，失败返回ERROR
 */
INT sys_time_convert_str_to_value(const CHAR *strData, UINT *uTime)
{   
    INT iRet = ERROR;
    UINT uYear = 0, uMon = 0, uDay = 0, uHour = 0, uMin = 0, uSec = 0;
    SYS_DATE_TIME_T stTime = {0};
    UINT uTimeValue = 0 ;

    if(NULL == strData  || uTime == NULL )
    {
        SYS_COMMON_ERROR("input param error.\n");
        return iRet;
    }

    if(sscanf(strData, "%04d-%02d-%02d %02d:%02d:%02d", &uYear, &uMon, &uDay, &uHour, &uMin, &uSec) != 6)
    {
        SYS_COMMON_ERROR("time str process error!\n");
        return iRet;
    }

    stTime.uYear   = uYear;
    stTime.uMonth  = uMon;
    stTime.uDay    = uDay;
    stTime.uHour   = uHour;
    stTime.uMinute = uMin;
    stTime.uSec    = uSec;
    iRet = sys_time_convert_struct_to_value(&stTime,&uTimeValue);
    if(iRet < 0)
    {
        SYS_COMMON_ERROR("date time convert error!\n");
        return ERROR;
    }
    *uTime = uTimeValue;
    return OK;
}  
/**@fn         sys_time_convert_str_to_datetime
 * @brief      将字符串转化为结构体
 * @param[in]  strData 存储时间的字符串
 * @param[out] pStDateTime 结构体指针
 * @return     成功返回OK，失败返回ERROR
 */
INT sys_time_convert_str_to_datetime(const CHAR *strData, SYS_DATE_TIME_T *pStDateTime)
{
    UINT uYear = 0, uMon = 0, uDay = 0, uHour = 0, uMin = 0, uSec = 0;
	
    if(NULL == strData || NULL == pStDateTime)
    {
        return ERROR;
    }
	
    if(sscanf(strData, "%04d-%02d-%02d %02d:%02d:%02d", &uYear, &uMon, &uDay, &uHour, &uMin, &uSec) != 6)
    {
        return ERROR;
    }
	
    pStDateTime->uYear   = uYear;
    pStDateTime->uMonth  = uMon;
    pStDateTime->uDay    = uDay;
    pStDateTime->uHour   = uHour;
    pStDateTime->uMinute = uMin;
    pStDateTime->uSec    = uSec;
	return OK;
}
  

/**@fn         sys_time_convert_struct_to_str
 * @brief      将SYS_DATE_TIME_T结构日期转换为字符串(形如2018-08-22 19:28:20)
 * @param[in]  pStDateTime  SYS_DATE_TIME_T结构
 * @param[out] strData      存储时间的字符串
 * @param[in]  uBufLen      字符串长度
 * @return     成功返回OK，失败返回ERROR
 */    
INT sys_time_convert_struct_to_str(const SYS_DATE_TIME_T *pStDateTime, CHAR *strData, UINT16 uBufLen)
{
    UINT uYear, uMon = 0, uDay = 0, uHour = 0, uMin = 0, uSec = 0;
    if(NULL == strData || NULL == pStDateTime)
    {
        return ERROR;
    }
	
    uYear = pStDateTime->uYear;
    uMon = pStDateTime->uMonth;
	
	
    uDay = pStDateTime->uDay;
    uHour = pStDateTime->uHour;
    uMin = pStDateTime->uMinute;
    uSec = pStDateTime->uSec;
	
    if(-1 != snprintf(strData,uBufLen-1,"%04d-%02d-%02d %02d:%02d:%02d", uYear, uMon, uDay, uHour, uMin, uSec))
    {
        return ERROR;
    }
	
    return OK;
}

/**@fn         sys_time_str_format_time
 * @brief      时间字符串格式化
 * @param[in]  strFormat     格式化字符串
 * @param[in]  pStDateTime   时间结构体指针
 * @param[out] strBuf        输出字符串缓冲区
 * @param[in]  uBufSize      缓冲区大小
 * @return     成功返回OK，失败返回ERROR
 */    
INT sys_time_str_format_time(const SYS_DATE_TIME_T *pStDateTime, const CHAR *strFromat, CHAR *strBuf,UINT16 uBufSize)
{
    INT iRet = ERROR;
    struct tm stTime = {0};
    if(!strBuf || !strFromat || !pStDateTime)
    {
        return iRet;
    }
    iRet = sys_time_convert_tm(pStDateTime,&stTime);
    if(iRet  < 0)
    {
        return iRet;
    }

    iRet  = strftime(strBuf, uBufSize - 1,strFromat,&stTime);
    return iRet;
}

/**@fn         sys_time_sleep_second
 * @brief      sleep秒
 * @param[in]  puSecond  SYS_DATE_TIME_T结构
 * @return     成功返回OK，失败返回ERROR
 */    
INT sys_time_sleep_second(UINT uSecond)
{
    INT iRet =  ERROR;
    iRet = sleep(uSecond);
    return iRet;
}


/**@fn         sys_time_sleep_ms
 * @brief      sleep毫秒
 * @param[in]  uMillSecond  SYS_DATE_TIME_T结构
 * @return     成功返回OK，失败返回ERROR
 */    
INT sys_time_sleep_ms(UINT64 uMillSecond)
{
    INT iRet =  ERROR;
    iRet =  usleep(uMillSecond * 1000);
    return iRet;
}

/**@fn         sys_time_sleep_us
 * @brief      sleep微秒
 * @param[in]  uMicroSecond
 * @return     成功返回OK，失败返回ERROR
 */    
INT sys_time_sleep_us(UINT uMicroSecond )
{
    INT iRet =  ERROR;
    iRet =  usleep(uMicroSecond);
    return iRet;
}

/**@fn         sys_time_rand_seed      
 * @brief      获取系统时间随机粽子信息
 * @return     成功返回 种子值
 */
INT sys_time_rand_seed(VOID)
{
    UINT64 uTime = 0;
    INT iRet = ERROR;
    iRet = sys_time_get_msec(&uTime);
    if(iRet < 0)
    {
        return iRet;
    } 
    srand(uTime);
    return rand();
}