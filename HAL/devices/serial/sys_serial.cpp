/*
 * @file sys_serial.cpp
 * @brief 
 *
 * @author BSP  
 * @date 2023/04/07 
 *
 * @note 下面的note和warning为可选项目  
 * @note 这里填写本文件的详细功能描述和注解  
 * @note 历史记录：  
 * @note V1.0.0 创建  
 *
 * @warning 这里填写本文件相关的警告信息  
 * encoding UTF-8  
 */

#include "sys_serial_priv.h"
/**@fn        sys_serial_open    
 * @brief     打开serial     
 * @return    成功返回 serial的文件描述符 错误返回 ERROR
 */
INT32 sys_serial_open(char* strDevName)
{
   INT32 fd = -1;
   fd = open(strDevName,O_RDWR | O_NOCTTY);
   if(fd < 0)
   {
     SYS_COMMON_ERROR("open serial %s failure.fd = %d \n",strDevName,fd);
     return ERROR;
   }
   return fd;
}

/**@fn         sys_serial_close
 * @brief      关闭serial的节点
 * @param[in]  serial的文件描述符
 * @return     成功返回 OK 错误返回 ERROR
 */
INT32 sys_serial_close(const INT32 fd)
{
    INT32 ret = -1;
    if(fd < 0)
    {
      SYS_COMMON_ERROR("serial fd  error \n");
      return ERROR;
    }
    ret  = close(fd);
    if(ret < 0)
    {
      SYS_COMMON_ERROR("close serial failure.fd = %d \n",fd);
      return ERROR;
    }
    return OK;
}

 /**@fn	       usys_serial_baudrate	  
 * @brief	   设置串口属性 波特率
 * @param[in]  iFd         文件描述符
 * @param[in]  iBaudRate   波特率
 * @return	   成功 0 失败 -1
 */
INT32 sys_serial_baudrate(const INT32 iFd,INT32 iBaudRate)
{
    INT32 iRet = -1;
    INT32 iIndex = 0 ;
    struct termios stTerMios;
    memset(&stTerMios,0,sizeof(stTerMios));
    iRet = tcgetattr(iFd, &stTerMios);
    if(iRet < 0)
    {
        SYS_COMMON_ERROR("get attribute error \n");
        return -1;
    }

    for(iIndex = 0; iIndex < sizeof(g_ArrayBaudRatePair)/sizeof(BAUDRATE_PARI_T); iIndex++)
    {
        if(iBaudRate != g_ArrayBaudRatePair[iIndex].iBaudRate)
        {
            continue;
        }
        tcflush(iFd, TCIOFLUSH);       
        cfsetispeed(&stTerMios, g_ArrayBaudRatePair[iIndex].iEBaudRate);    
        cfsetospeed(&stTerMios, g_ArrayBaudRatePair[iIndex].iEBaudRate);     
        iRet  = tcsetattr(iFd, TCSANOW, &stTerMios);
        if (iRet != 0) 
        {          
            SYS_COMMON_ERROR("tcsetattr fd :%d error\n",iFd); 
            return -1;       
        }      
        tcflush(iFd,TCIOFLUSH);     
        break;
    }
    return iRet;
} 


 /**@fn	       sys_serial_set_databits 
 * @brief	   设置串口属性 数据位
 * @param[in]  pTermios    终端参数指针
 * @param[in]  iDataBit    数据位 取值7 、8
 * @return	   成功 0 失败 -1
 */
INT32 sys_serial_set_databits(struct termios *pTermios,INT32 iDataBit)
{   
    if(!pTermios)
    {
        SYS_COMMON_ERROR("invaild param \n");
        return -1;
    }
    switch(iDataBit)
    {
        case UART_BIT_DATA_7:
            pTermios->c_cflag |= CS7;
            break;
        case UART_BIT_DATA_8:
            pTermios->c_cflag |= CS8;
            break;
        default:
            SYS_COMMON_ERROR("set data bit :%d error ,no support\n",iDataBit);
            return -1;
    }
    return 0;
}


/**@fn	       sys_serial_set_parity	  
 * @brief	   设置串口属性 数据位
 * @param[in]  pTermios    终端参数指针
 * @param[in]  iParity     校验类型 取值为N无校验,E偶校验,O奇校验,S Space校验  
 * @return	   成功 0 失败 -1
 */
 INT32 sys_serial_set_parity(struct termios *pTermios, INT32 iParity)
{
    if(!pTermios)
    {
        SYS_COMMON_ERROR("invaild param \n");
        return -1;
    }
    switch (iParity)   
    {     
    case 'n':  
    case 'N':      
        pTermios->c_cflag &= ~PARENB;   /* Clear parity enable */  
        pTermios->c_iflag &= ~INPCK;     /* Enable parity checking */   
        break;    
    case 'o':     
    case 'O':       
        pTermios->c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/    
        pTermios->c_iflag |= INPCK;             /* Disnable parity checking */   
        break;    
    case 'e':    
    case 'E':     
        pTermios->c_cflag |= PARENB;     /* Enable parity */      
        pTermios->c_cflag &= ~PARODD;   /* 转换为偶效验*/       
        pTermios->c_iflag |= INPCK;       /* Disnable parity checking */  
        break;  
    case 'S':   
    case 's':  /*as no parity*/     
        pTermios->c_cflag &= ~PARENB;  
        pTermios->c_cflag &= ~CSTOPB;
        break;    
    default:     
        SYS_COMMON_ERROR("Unsupported parity :%d\n",iParity);
        return -1;
    }    
    pTermios->c_iflag &= ~(BRKINT |ICRNL| ISTRIP | IXON);
    return 0;
}


/**@fn	      sys_serial_set_stopbit	  
 * @brief	   设置串口属性 停止位
 * @param[in]  pTermios    终端参数指针
 * @param[in]  iStopBit    停止位  取值为 1 或者2 
 * @return	   成功 0 失败 -1
 */
INT32 sys_serial_set_stopbit(struct termios *pTermios,INT32 iStopBit)
{   
    if(!pTermios)
    {
        return -1;
    }
    /* 设置停止位*/    
    switch(iStopBit)  
    {
        case 1:
            pTermios->c_cflag &= ~CSTOPB;
            break;
        case 2:
            pTermios->c_cflag |= CSTOPB;
            break;
        default:
            SYS_COMMON_ERROR("Unsupported stop bits :%d \n",iStopBit);
            return -1;  
    }
    return 0;
}


/**@fn         sys_serial_tx
 * @brief      写传输
 * @param[in]  串口的文件描述符
 * @return     成功返回 OK 错误返回 ERROR
 */

INT32 sys_serial_tx(const INT32 iFd,void *pData,size_t iLength)
{
   INT32 iRet = -1;
   if(iFd < 0)
   {
      SYS_COMMON_ERROR("gsensor fd  error \n");
      return ERROR;
   }
   iRet = write(iFd,pData, iLength);
   if(iRet != iLength)
   {
		  SYS_COMMON_ERROR("write data error failure.need write %d, return %d\n",iLength,iRet);
      return ERROR;
   }
   return OK;
}

/**@fn         sys_serial_rx
 * @brief      写传输
 * @param[in]  串口的文件描述符
 * @return     成功返回 OK 错误返回 ERROR
 */

INT32 sys_serial_rx(const INT32 iFd,void *pData,size_t iLength)
{
   INT32 iRet = -1;
   if(iFd < 0)
   {
      SYS_COMMON_ERROR("gsensor fd  error \n");
      return ERROR;
   }
   iRet = read(iFd,pData, iLength);
   if(iRet != iLength)
   {
		  SYS_COMMON_ERROR("read data error failure.need read %d, return %d\n",iLength,iRet);
      return ERROR;
   }
   return OK;
}
