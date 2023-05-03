#include "serial_priv.h"



/*-----------------------------------------*/

 /**@fn	       uart_get_priv_data	  
 * @brief	   获取串口对象私有数据 
 * @param[in]  pISerial      串口对象操作指针
 * @return	   成功 私有数据指针 失败 NULL
 */
static SERIAL_UART_PRIV_DATA_T *uart_get_priv_data(ISerial *pISerial)
{
    SERIAL_UART_BASE_T *pBase =NULL;
    if(!pISerial)
    {
        LOGGER_ERROR("invaild param \n");
        return NULL;
    }
    pBase = (SERIAL_UART_BASE_T*)pISerial;

    return &pBase->stPrivData;
}

/**@fn        uart_print_debug_info      
* @brief      获取底层uart信息统计
* @param[in]  pISerial     串口对象操作指针
* @return     成功返回OK 失败返回ERROR
*/
INT32 uart_get_debug_info(ISerial *pISerial, SERIAL_COUNTER_INFO_T *pStUartInfo)
{
    INT32 iRet = ERROR;
    struct serial_icounter_struct stCounter;
    SERIAL_UART_PRIV_DATA_T *pStPrivData = NULL;

    if(NULL == pISerial || NULL == pStUartInfo)
    {
        LOGGER_ERROR("invalid param input\n");
        return iRet;
    }

    pStPrivData = uart_get_priv_data(pISerial);
    if(NULL == pStPrivData)
    {
        LOGGER_ERROR("get uart priv data error\n");
        return iRet;
    }

    if(-1 == pStPrivData->iFd)
    {
        LOGGER_ERROR("uart fd error\n");
        return iRet;
    }

    memset(&stCounter, 0x0, sizeof(stCounter));
    iRet = ioctl(pStPrivData->iFd, UART_TIOCGICOUNT, &stCounter);
    if(iRet < 0)
    {
        LOGGER_ERROR("ioctl iFd %d failed! Error:%s\n",pStPrivData->iFd,strerror(errno));
        return iRet;
    }
    pStUartInfo->iRx         = stCounter.rx;
    pStUartInfo->iTx         = stCounter.tx;
    pStUartInfo->iParity     = stCounter.parity;
    pStUartInfo->iFrame      = stCounter.frame;
    pStUartInfo->iBrk        = stCounter.brk;
    pStUartInfo->iOverrun    = stCounter.overrun;
    pStUartInfo->iBufOverrun = stCounter.buf_overrun;

    return OK;
}

 /**@fn	       uart_open	  
 * @brief	   打开串口 
 * @param[in]  pISerial      串口对象操作指针
 * @return	   成功 0 失败 -1
 */
INT32 uart_open(ISerial *pISerial)
{   
    INT32 iFd = -1;
    char strDevName[32] = {0};
    SERIAL_UART_PRIV_DATA_T *pStPrivData = NULL;
    if(NULL == pISerial)
    {
        LOGGER_ERROR("input error \n");
        return -1;
    }
    
    pStPrivData = uart_get_priv_data(pISerial);
    if(!pStPrivData)
    {
        LOGGER_ERROR("get priv data error \n");
        return -1;
    }
    LOGGER_INFO("uart iIndex[%d]\n", pStPrivData->iIndex);
    if(pStPrivData->sDevName[0] != 0){
       snprintf(strDevName,sizeof(strDevName),"%s%d",pStPrivData->sDevName,pStPrivData->iIndex);
    }else{
       snprintf(strDevName,sizeof(strDevName),"%s%d",UART_DEV_PATH,pStPrivData->iIndex);
    }
    iFd =sys_serial_open(strDevName);
    if(iFd < 0)
    {
        LOGGER_ERROR("open dev fd error :%s \n",strDevName);
        return -1;
    }
    pStPrivData->iFd = iFd;    
    return 0;
}

/**@fn	       uart_release	  
 * @brief	   串口资源释放接口
 * @param[in]  pISerial          串口对象接口指针
 * @return	   
 */
static INT32 uart_release(ISerial *pISerial)
{
    INT32 iRet = ERROR;
    SERIAL_UART_BASE_T *pBase = NULL;
    if(NULL == pISerial)
    {
        LOGGER_ERROR("Realse error \n");
        return iRet;
    }
    pBase = (SERIAL_UART_BASE_T*)pISerial;
    sys_mem_free(pBase);
    pBase = NULL;
    return OK;
}

 /**@fn	       uart_close	  
 * @brief	   串口关闭 
 * @param[in]  pISerial     串口对象操作指针
 * @return	   成功 0 失败 -1
 */
INT32 uart_close(ISerial *pISerial)
{
    INT32 iRet = ERROR;
    SERIAL_UART_PRIV_DATA_T *pStPrivData = NULL;
    if(NULL == pISerial)
    {
        LOGGER_ERROR("input error \n");
        return iRet;
    }
    pStPrivData = uart_get_priv_data(pISerial);
    if(NULL == pStPrivData)
    {
        LOGGER_ERROR("get priv data error \n");
        return iRet;
    }
    iRet = sys_serial_close(pStPrivData->iFd);
    {
        LOGGER_ERROR("close error \n");
        return iRet;
    }
    iRet = uart_release(pISerial);
    if(OK != iRet)
    {
        LOGGER_ERROR("call uart_release error\n");
    }
    return iRet;
}

 /**@fn	       uart_write	  
 * @brief	   串口写数据 
 * @param[in]  pISerialCom      串口对象操作指针
 * @param[in]  pData            写入数据指针
 * @param[in]  uLength          数据长度
 * @return	   成功 写字节长度 失败 -1
 */
INT32 uart_write(ISerial *pISerial, const UINT8 *pData,UINT32 uLength)
{
    INT32 iFd  = -1;
    INT32 iRet = -1;
    SERIAL_UART_PRIV_DATA_T *pStPrivData = NULL;
    if(!pData || uLength == 0 || !pISerial)
    {
        LOGGER_ERROR("invaild param error :%p :%p %d \n", pISerial, pData,uLength);
        return -1;
    }
    pStPrivData = uart_get_priv_data(pISerial);
    if(!pStPrivData)
    {
        LOGGER_ERROR("get priv data error \n");
        return -1;
    }
    iFd = pStPrivData->iFd;
    iRet = sys_serial_tx(iFd,(void *)pData,(size_t)uLength);
    if(iRet < 0)
    {
         LOGGER_ERROR("tx data error \n");
        return -1;
    }
    return uLength;
}

 /**@fn	       uart_read	  
 * @brief	   串口读数据 
 * @param[in]  pISerialCom      串口对象操作指针
 * @param[in]  pData            读数据指针
 * @param[in]  uLength          读数据buffer长度
 * @return	   成功 读到数据字节长度 失败 -1
 */
INT32 uart_read(ISerial *pISerial, UINT8 *pData, UINT32 uLength)
{
    INT32 iFd  = -1;
    INT32 iRet = -1;
    SERIAL_UART_PRIV_DATA_T *pStPrivData = NULL;
    if(!pData || uLength == 0 || !pISerial)
    {
        LOGGER_ERROR("invaild param error :%p :%p %d \n", pISerial, pData, uLength);
        return -1;
    }
    pStPrivData = uart_get_priv_data(pISerial);
    if(!pStPrivData)
    {
        LOGGER_ERROR("get priv data error \n");
        return -1;
    }
    iFd = pStPrivData->iFd;
    iRet = sys_serial_rx(iFd,(void *)pData,(size_t)uLength);
    if(iRet < 0)
    {
         LOGGER_ERROR("rx data error \n");
        return -1;
    }
    return uLength;
}

 /**@fn	       uart_set_attribute	  
 * @brief	   设置串口属性 波特率 数据位 校验位 停止位
 * @param[in]  pISerial      类指针
 * @param[in]  pStInfo          串口设置信息
 * @return	   成功 0 失败 -1
 */
INT32 uart_set_attribute(ISerial *pISerial, SERIAL_ATTRIBUTE_UART_T *pStInfo)
{
    INT32 iRet =-1;
    INT32 iFd = -1;
    struct termios stTerMios;
    SERIAL_UART_PRIV_DATA_T *pStPrivData = NULL;

    if(NULL == pISerial || NULL == pStInfo)
    {
        LOGGER_ERROR("input error \n");
        return -1;
    }
    memset(&stTerMios,0,sizeof(stTerMios));
    pStPrivData = uart_get_priv_data(pISerial);
    if(!pStPrivData)
    {
        LOGGER_ERROR("get priv data error \n");
        return -1;
    }
    iFd = pStPrivData->iFd;
    if(iFd < 0)
    {
        LOGGER_ERROR("fd error \n");
        return -1;
    }
    //波特率设置
    iRet = sys_serial_baudrate(iFd, pStInfo->iBaudRate);
    if(iRet)
    {
        LOGGER_ERROR("set iBaudRate :%d\n", pStInfo->iBaudRate);
        return -1;
    }
    
    //get current fd info 
    iRet = tcgetattr(iFd, &stTerMios);
    if(iRet < 0)
    {
        LOGGER_ERROR("tcgetattr error fd:%d \n",iFd);
        return -1;
    }

    //数据位
    if(sys_serial_set_databits(&stTerMios, pStInfo->iDataBits) < 0)
    {
        LOGGER_ERROR("set_databits error :%d\n", pStInfo->iDataBits);
        return -1;
    }
    //校验类型
    if(sys_serial_set_parity(&stTerMios, pStInfo->iParity) < 0)
    {
        LOGGER_ERROR("set_parity error :%d\n", pStInfo->iParity);
        return -1;
    }
    //停止位
    if(sys_serial_set_stopbit(&stTerMios, pStInfo->iStopBits) < 0)
    {
        LOGGER_ERROR("set_stopbit error :%d\n", pStInfo->iStopBits);
        return -1;
    }
    //set only transport
    stTerMios.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/
    stTerMios.c_oflag  &= ~OPOST;   /*Output*/

    stTerMios.c_cc[VTIME] = 150; /* 设置超时15 seconds*/
    stTerMios.c_cc[VMIN] = 0;    /* Update the options and do it NOW */

    //set  attribute 
    tcflush(iFd,TCIOFLUSH); 

    if(tcsetattr(iFd, TCSANOW, &stTerMios)) 
    {          
        LOGGER_ERROR("tcsetattr fd :%d error\n",iFd); 
        return -1;       
    }      
    tcflush(iFd,TCIOFLUSH);     
    return 0;
}

/**@fn	       uart_init_priv_data	  
 * @brief	   初始化私有数据
 * @param[in]  pStPrivData          私有数据指针
 * @param[in]  iIndex   串口索引
 * @return	   
 */
static INT32 uart_init_priv_data(SERIAL_UART_PRIV_DATA_T *pStPrivData,CHAR* sDevName, INT32 iIndex)
{
    INT32 iRet = ERROR;
    if(NULL == pStPrivData)
    {
        LOGGER_ERROR("init pPriv Data error\n");
        return iRet;
    }
    pStPrivData->iFd = -1;
    pStPrivData->iIndex = iIndex;
    sys_mem_copy((void *)pStPrivData->sDevName,(void *)sDevName,strlen(sDevName));
    return OK;
}

/**@fn	       uart_init_interface	  
 * @brief	   初始化APP_SDK接口
 * @param[in]  无
 * @return	   成功返回ISerialCom操作指针  失败返回NULL;
 */
static INT32 uart_init_interface(ISerial *pISerial)
{   
    INT32 iRet = ERROR;
    if(NULL == pISerial)
    {
        LOGGER_ERROR("init interface func error\n");
        return iRet;
    }
    pISerial->Open           = uart_open;
    pISerial->Close          = uart_close;
    pISerial->SetAttribute   = uart_set_attribute;
    pISerial->Write          = uart_write;
    pISerial->Read           = uart_read;
    pISerial->GetInfo        = uart_get_debug_info;
    return OK;
}

/**@fn	       serial_init_instance	  
 * @brief	   初始化构造单例
 * @param[in]  iIndex uart序号
 * @return	   成功返回对象指针,失败返回NULL
 */
ISerial *serial_init_instance(char* sDevname,int iIndex)
{   
    INT32 iRet = ERROR;
    SERIAL_UART_BASE_T *pBase = NULL;
    pBase =(SERIAL_UART_BASE_T *) sys_mem_malloc(sizeof(SERIAL_UART_BASE_T));
    if(!pBase)
    {
        LOGGER_ERROR("malloc errror \n");
        return NULL;
    }
    memset(pBase,0,sizeof(SERIAL_UART_BASE_T));
    //初始化对外接口
    iRet = uart_init_interface(&pBase->stInterface);
    if(iRet < 0)
    {   
        LOGGER_ERROR("init interface error \n");
        sys_mem_free(pBase);
        pBase = NULL;
        return NULL;
    }

    //初始化私有数据
    iRet = uart_init_priv_data(&pBase->stPrivData,sDevname, iIndex);
    if(iRet < 0)
    {   
        LOGGER_ERROR("init priv data error \n");
        sys_mem_free(pBase);
        pBase = NULL;
        return NULL;
    }
    return &pBase->stInterface;
}