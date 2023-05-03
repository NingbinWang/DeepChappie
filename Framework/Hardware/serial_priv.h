#ifndef _SERIAL_PRIV_H_
#define _SERIAL_PRIV_H_

#include "Serial.h"
#include "sys_pthread.h"
#include "sys_mem.h"
#include "sys_timer.h"
#include "sys_posix.h"
#include "sys_mqueue.h"//信号量
#include "sys_time.h"
#include "logger.h"
#include <math.h>
#include "sys_serial.h"
#include "logger.h"

#define UART_DEV_PATH "/dev/ttyS"
#define UART_TIOCGICOUNT   0x545D  //监测uart状态IO

/**
 * @struct    SERIAL_UART_PRIV_DATA_T
 * @brief     UART私有数据结构
*/
typedef struct SERIAL_UART_PRIV_DATA_T
{
    INT32 iFd;            // 操作的文件描述符 
    INT32 iIndex;         // 串口序号 
    CHAR  sDevName[128];       //
}SERIAL_UART_PRIV_DATA_T;

/**
 * @struct    SERIAL_UART_BASE_T
 * @brief     UART基础数据结构
*/
typedef struct SERIAL_UART_BASE_T
{
    ISerial          stInterface; /* 串口操作接口 */
    SERIAL_UART_PRIV_DATA_T stPrivData;  /* 私有数据 */
}SERIAL_UART_BASE_T;


#endif