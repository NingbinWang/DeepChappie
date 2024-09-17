#ifndef _SERIAL_H_
#define _SERIAL_H_

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

typedef struct 
{
    int  enable;
    char sDevName[128];
    int iIndex;
}Serial_Info_t;

/**@struct SERIAL_ATTRIBUTE_UART_T 
 * @brief  定义UART的参数设置
 */
typedef struct
{
    int iBaudRate;    /*波特率*/
    int iDataBits;    /*数据位*/
    int iStopBits;    /*停止位*/
    int iParity;      /*校验位*/
}SERIAL_ATTRIBUTE_UART_T;


/**@struct SERIAL_UART_IO_COUNTER_T 
 * @brief  底层监控结构体监控
 */
typedef struct 
{
	int iRx;          /* 接收数据计数 */   
    int iTx;          /* 发送数据计算 */
    int iParity;      /* 计数校验失败计数 */
    int iFrame;       /* 错误frame统计 */
    int iBrk;         /* 帧错误计数 */
    int iOverrun;     /* 硬件256字节FIFO溢出计数 */
	int iBufOverrun;  /* 驱动640KB缓存溢出计数 */
	int iRes[13];     /* 预留13 INT大小，用于后续扩展使用 */
}SERIAL_COUNTER_INFO_T;

typedef struct ISerial ISerial;

struct ISerial
{
    /**@fn	       Open	  
     * @brief	   开启
     * @param[in]  pISerial 串口对象指针
     * @return	   成功返回OK   失败返回错误码
     */
    int (*Open)(ISerial *pISerial);

    /**@fn	       SetAttribute	  
     * @brief	   设置参数
     * @param[in]  pISerialCom 串口对象指针
     * @param[in]  pStInfo     串口属性信息结构体指针
     * @return	   成功返回OK   失败返回错误码
     */
    int (*SetAttribute)(ISerial *pISerial,SERIAL_ATTRIBUTE_UART_T *pStInfo);

    /**@fn	       Write	  
     * @brief	   写数据
     * @param[in]  pISerial 串口对象指针
     * @param[in]  pData       数据指针
     * @param[in]  uLength     数据长度
     * @return	   成功返回写数据长度   失败返回错误码
     */
    int (*Write)(ISerial *pISerial,const unsigned char *pData,unsigned int uLength);

    /**@fn	       Read	  
     * @brief	   读数据
     * @param[in]  pISerial 串口对象指针
     * @param[out] pData       数据指针
     * @param[in]  uLength     缓冲区长度
     * @return	   成功返回读数据长度   失败返回错误码
     */
    int (*Read)(ISerial *pISerial,unsigned char *pData,unsigned int uLength);
    
    /**@fn	       Close	  
     * @brief	   关闭uart
     * @param[in]  pISerial 串口对象指针
     * @return	   成功返回OK   失败返回错误码
     */
    int (*Close)(ISerial *pISerial);

    /**@fn	       GetInfo	  
     * @brief	   获取统计信息
     * @param[in]  pISerial      串口对象指针
     * @param[in]  pStCountInfo     串口统计信息
     * @return	   成功返回OK 失败返回ERROR
     */
    int (*GetInfo)(ISerial *pISerial, SERIAL_COUNTER_INFO_T *pStCountInfo);
};

/**@fn	       serial_com_init_instance	  
 * @brief	   创建串口对象
 * @param[in]  sDevName  串口类型
 * @param[in]  iIndex 串口索引
 * @return	   成功 串口对象操作指针 失败 NULL
 */
ISerial *serial_init_instance(char* sDevName,int iIndex);





#ifdef __cplusplus
}
#endif/*__cplusplus*/


#endif

