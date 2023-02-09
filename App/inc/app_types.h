      
/**@file app_types.h
 * @note TUUBU System Technology Co., Ltd. All Right Reserved.
 * @brief  
 * 
 * @author   alex
 * @dappe     2022-08-30
 * @version  1.0
 * 
 * @note ///Description here 
 * @note History:        
 * @note     <author>   <time>    <version >   <desc>
 * @note    alex       2022-08-30  V1.0    Creappe
 * @warning  
 */

#ifndef _APP_TYPES_H_
#define _APP_TYPES_H_

#ifdef  __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include <linux/types.h>

typedef void                    app_void;
typedef char*                   app_pstr;
typedef /*signed*/ char         app_s8;
typedef unsigned char           app_u8;
typedef /*signed*/ short        app_s16;
typedef unsigned short          app_u16;
typedef /*signed*/ int          app_s32;
typedef unsigned int            app_u32;
#if defined(__GNUC__)
typedef /*signed*/ long long    app_s64;
typedef unsigned long long      app_u64;
#else /* _MSC_VER || __BORLANDC__ */
typedef   signed __int64        app_s64;
typedef unsigned __int64        app_u64;
#endif
typedef unsigned long           app_addr;
typedef app_u64                 app_offset;


typedef struct app_timeval
{
    app_u32 sec;     /* seconds */
    app_u32 usec;    /* microseconds */
}app_timeval_t;

typedef struct app_timespec
{
    app_u32 sec;     /* seconds */
    app_u32 nsec;    /* nanoseconds */
}app_timespec_t;

typedef app_u32                  app_sec_t;       /* seconds */
typedef app_u64                  app_msec_t;      /* milliseconds */
typedef app_u64                  app_usec_t;      /* microseconds */
typedef app_u64                  app_nsec_t;      /* nanoseconds */

typedef app_s8                   app_int8;
typedef app_u8                   app_uint8;
typedef app_s16                  app_int16;
typedef app_u16                  app_uint16;
typedef app_s32                  app_int32;
typedef app_u32                  app_uint32;


#define APP_TRUE                 (1)
#define APP_FALSE                (0)
#define APP_NULL                 (NULL)

#define APP_INVALID              (~0)

#define APP_OK                   (0)
#define APP_ERROR                (1)


#ifdef  __cplusplus
}
#endif  /* __cplusplus */

#endif  /*_app_TYPES_H_*/


