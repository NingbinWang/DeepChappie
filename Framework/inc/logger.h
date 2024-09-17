#ifndef _LOGGER_H_
#define _LOGGER_H_
#include <string.h>
#include <errno.h>
#include"sys_time.h"
#include"sys_common.h"

#define LOGGER_ERROR(fmt, arg...) \
    do{ \
        INT iMsecond = 0;\
        UINT64 uTimeStamp = 0;\
        SYS_DATE_TIME_T stDataTime = {0};\
        sys_time_get_in_struct(&stDataTime);\
        sys_time_get_utc_msec(&uTimeStamp);\
        iMsecond = uTimeStamp % 1000;\
        printf("[%02d-%02d %02d:%02d:%02d:%03d][\033[1;31;40mERROR\033[0m][SYS][%s:%d]" fmt , stDataTime.uMonth, stDataTime.uDay, stDataTime.uHour, stDataTime.uMinute, stDataTime.uSec, iMsecond,  __FUNCTION__, __LINE__, ##arg);\
    } while(0)

#define LOGGER_INFO(fmt, arg...) \
    do{ \
        INT iMsecond = 0;\
        UINT64 uTimeStamp = 0;\
        SYS_DATE_TIME_T stDataTime = {0};\
        sys_time_get_in_struct(&stDataTime);\
        sys_time_get_utc_msec(&uTimeStamp);\
        iMsecond = uTimeStamp % 1000;\
        printf("[%02d-%02d %02d:%02d:%02d:%03d][INFO ][SYS][%s:%d]" fmt , stDataTime.uMonth, stDataTime.uDay, stDataTime.uHour, stDataTime.uMinute, stDataTime.uSec, iMsecond,  __FUNCTION__, __LINE__, ##arg);\
    } while(0)

#define LOGGER_WARN(fmt, arg...) \
    do{ \
        INT iMsecond = 0;\
        UINT64 uTimeStamp = 0;\
        SYS_DATE_TIME_T stDataTime = {0};\
        sys_time_get_in_struct(&stDataTime);\
        sys_time_get_utc_msec(&uTimeStamp);\
        iMsecond = uTimeStamp % 1000;\
        printf("[%02d-%02d %02d:%02d:%02d:%03d][WARN ][SYS][%s:%d]" fmt , stDataTime.uMonth, stDataTime.uDay, stDataTime.uHour, stDataTime.uMinute, stDataTime.uSec, iMsecond,  __FUNCTION__, __LINE__, ##arg);\
    } while(0)

#define LOGGER_DEBUG(fmt, arg...) \
    do{ \
        INT iMsecond = 0;\
        UINT64 uTimeStamp = 0;\
        SYS_DATE_TIME_T stDataTime = {0};\
        sys_time_get_in_struct(&stDataTime);\
        sys_time_get_utc_msec(&uTimeStamp);\
        iMsecond = uTimeStamp % 1000;\
        printf("[%02d-%02d %02d:%02d:%02d:%03d][DEBUG][SYS][%s:%d]" fmt , stDataTime.uMonth, stDataTime.uDay, stDataTime.uHour, stDataTime.uMinute, stDataTime.uSec, iMsecond,  __FUNCTION__, __LINE__, ##arg);\
    } while(0)


#endif