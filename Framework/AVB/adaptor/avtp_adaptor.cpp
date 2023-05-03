/* 
 * @file    avtp_adaptor.c
 * @brief   avtp网络端点适配层
 * @author  alex_king
 * @date    2023-2-17
 * @version v1.0.0 
 */
#include "avtp_adaptor.h"

/**@fn         avtp_socket_address_setup   
 * @brief      socket setup
 * @param[in]  iSock 套接字句柄
 * @param[in]  pInterfaceName 网卡名称
 * @param[in]  pMacAddr Mac地址
 * @param[in]  iProtocol 协议类型
 * @param[in]  struct sockaddr_ll 结构体指针
 * @return     成功返回套接字 失败返回ERROR
 */
static INT32 avtp_socket_address_setup(INT32 iSock, CHAR *pInterfaceName, UINT8 *pMacAddr, INT32 iProtocol, struct sockaddr_ll *pSockAddr)
{
    INT32 iRet = ERROR;
    struct ifreq StFreq; //网络接口地址
    VOID *pMemset = NULL;
    VOID *pDest = NULL;

    if((NULL == pInterfaceName) || (NULL == pMacAddr) || (NULL == pSockAddr))
    {
        return iRet;
    }
    pMemset = memset(&StFreq, 0, sizeof(struct ifreq));
    if(NULL == pMemset)
    {
        return iRet;
    }
    iRet = snprintf(StFreq.ifr_name, sizeof(StFreq.ifr_name), "%s", pInterfaceName); //指定网卡名称
    if(iRet < 0)
    {
        return iRet;
    }
    iRet = ioctl(iSock, SIOCGIFINDEX, &StFreq);        /* 获取网卡的接口地址 */
	if (iRet < 0) 
    {
		AVTP_ERROR("Failed to get interface index\n");
		return iRet;
	}
	pSockAddr->sll_family = AF_PACKET;
	pSockAddr->sll_protocol = sys_htons(iProtocol);
	pSockAddr->sll_halen = ETH_ALEN;
	pSockAddr->sll_ifindex = StFreq.ifr_ifindex;
	pDest = memcpy(pSockAddr->sll_addr, pMacAddr, ETH_ALEN);
    if(NULL == pDest)
    {
        AVTP_ERROR("memcpy error\n");
        return ERROR;
    }
    return OK;
}

/**@fn         avtp_talker_socket_create   
 * @brief      avtp talker创建套接字
 * @param[in]  iPriority 优先级
 * @param[in]  pInterfaceName 网卡名称
 * @param[in]  pDestMacAddr 目的Mac地址
 * @param[in]  struct sockaddr_ll 结构体指针
 * @return     成功返回OK 失败返回ERROR
 */
INT32 avtp_talker_socket_create(INT32 iPriority, CHAR *pInterfaceName, UINT8 *pDestMacAddr, struct sockaddr_ll *pStSockAddr)
{
    INT32 iRet = ERROR;
    INT32 iSock = -1;
    if((NULL == pInterfaceName) || (NULL == pDestMacAddr) || (NULL == pStSockAddr) || (iPriority < 0))
    {
        return iRet;
    }
    LOGGER_INFO(">>>>>>>>>>>>>> iPriority:%d, pInterfaceName:%s\n", iPriority, pInterfaceName);
    iSock = socket(AF_PACKET, SOCK_DGRAM, (INT32)sys_htons(ETH_P_TSN));
    if(iSock < 0)
    {
        LOGGER_ERROR("Failed to open socket\n");
        return iRet;
    }
    iRet = setsockopt(iSock, SOL_SOCKET, SO_PRIORITY, &iPriority, sizeof(iPriority));
    if(iRet < 0)
    {
        LOGGER_ERROR("Failed to set priority\n");
        iRet = close(iSock);
        if(iRet < 0)
        {
            LOGGER_ERROR("close error\n");
        }
        return iRet;
    }
    iRet = avtp_socket_address_setup(iSock, pInterfaceName, pDestMacAddr, ETH_P_TSN, pStSockAddr);
    if (iRet < 0)
    {
        LOGGER_ERROR("setup socket address error\n");
        iRet = close(iSock);
        if(iRet < 0)
        {
            LOGGER_ERROR("close error\n");
        }
        return ERROR;
    }
    return iSock;
}

/**@fn         avtp_socket_send   
 * @brief      发送数据
 * @param[in]  iSock 套接字
 * @param[in]  pData 发送的数据
 * @param[in]  uDataLen 数据长度
 * @param[in]  struct sockaddr_ll 结构体指针
 * @return     成功返回发送的数据长度 失败返回ERROR
 */
INT32 avtp_socket_send(INT32 iSock, UINT8 *pData, UINT32 uDataLen, struct sockaddr_ll *pStSockAddr)
{
    if((NULL == pData) || (NULL == pStSockAddr))
    {
        return ERROR;
    }
    return sendto(iSock, pData, uDataLen, MSG_DONTWAIT, (struct sockaddr *)pStSockAddr, sizeof(struct sockaddr_ll));
}

/**@fn         avtp_socket_close   
 * @brief      关闭套接字
 * @param[in]  iSock 套接字
 * @param[in]  pBuf 接收缓冲区地址指针
 * @param[in]  iBufLen 接收缓冲区长度
 * @return     成功返回OK 失败返回ERROR
 */
INT32 avtp_socket_close(INT32 iSock)
{
    INT32 iRet = ERROR;
    
	if(iSock < 0)
	{
		return ERROR;
	}
	iRet = close(iSock);
    if(iRet < 0)
    {
        AVTP_ERROR("close error\n");
        return iRet;
    }
	return OK;
}