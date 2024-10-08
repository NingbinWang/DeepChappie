#ifndef _SYS_SERIAL_H_
#define _SYS_SERIAL_H_

#include "sys_common.h"
#include<termios.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<linux/serial.h>
/**
 * @struct    BAUDRATE_PARI_T
 * @brief     波特率对照表结构
*/
typedef struct  BAUDRATE_PARI_T
{
    INT iBaudRate;   /* 波特率 */
    INT iEBaudRate;  /* 波特率设置参数 */
}BAUDRATE_PARI_T;

//波特率MAP,转换成波特率枚举
BAUDRATE_PARI_T g_ArrayBaudRatePair[] = 
{
    {115200 , B115200},
    {57600 , B57600},
    {38400 , B38400},
    {19200 , B19200},
    {9600 , B9600},
    {4800 , B4800},
    {2400 , B2400},
    {1200 , B1200},
    {300,B300},
};

/**
 * @enum      UART_BIT_DATA_E
 * @brief     数据位枚举
*/
typedef enum UART_BIT_DATA_E
{
    UART_BIT_DATA_7 = 0x07,  /* 7位 */
    UART_BIT_DATA_8          /* 8位 */
}UART_BIT_DATA_E;

INT sys_serial_open(char* strDevName);

INT sys_serial_close(const INT fd);

INT sys_serial_baudrate(const INT iFd,INT iBaudRate);

INT sys_serial_set_databits(struct termios *pTermios,INT iDataBit);

 INT sys_serial_set_parity(struct termios *pTermios, INT iParity);

 INT sys_serial_set_stopbit(struct termios *pTermios,INT iStopBit);

 INT sys_serial_tx(const INT iFd,void *pData,size_t iLength);

 INT sys_serial_rx(const INT iFd,void *pData,size_t iLength);


#endif