#include"sys_socket_priv.h"

/**@fn         sys_socket_create
 * @brief      创建socket
 * @param[in]  iDomain 协议族, 参考man 2 socket
 * @param[in]  iType 套接字类型, 参考man 2 socket
 * @param[in]  iProtocol 协议类型, 参考man 2 socket
 * @return     成功返回 套接字句柄 错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT32 sys_socket_create(INT32 iDomain, INT32 iType, INT32 iProtocol)
{
    INT32 iSockFd = -1;

    if(SYS_AF_INET6 != iDomain && SYS_AF_INET != iDomain)
    {
        SYS_COMMON_ERROR("socket iDomain is no support iDomain:%d \n",iDomain);
        return ERROR;
    }

    iSockFd = socket(iDomain, iType, iProtocol);
    if (iSockFd < 0)
    {
        SYS_COMMON_ERROR("socket create error \n");
        return ERROR;
    }

    return iSockFd;
}

/**@fn         sys_socket_bind      
 * @brief      绑定socket
 * @param[in]  iSockFd  创建的套接字
 * @param[in]  iDomain 协议族, 参考man 2 socket
 * @param[in]  strIPAddr 要绑定的地址, 为NULL则代表监听所有地址
 * @param[in]  uPort 要绑定的端口
 * @return     成功返回 套接字句柄 错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT32 sys_socket_bind(INT32 iSockFd, INT32 iDomain, const CHAR *strIPAddr, UINT16 uPort)
{
    INT32 iRet = -1;
    INT32 bReuse = 1;
    struct sys_address_t stServerAddr;

    if(iSockFd < 0)
    {
        SYS_COMMON_ERROR("param is wrong!\n");
    	return ERROR;
    }
    
    if(iDomain != AF_INET6 && iDomain != AF_INET)
    {
        SYS_COMMON_ERROR("socket iDomain is no support iDomain:%d \n",iDomain);
        return ERROR;
    }
    memset(&stServerAddr,0,sizeof(stServerAddr));

    //端口复用
	if (ERROR == setsockopt(iSockFd, SOL_SOCKET, SO_REUSEADDR, (CHAR*)&bReuse, sizeof(INT32)))
    {
        SYS_COMMON_ERROR("bReuse address failed!\n");
    	return ERROR;
    }

    if( iDomain == AF_INET)
    {
         stServerAddr.size = sizeof(struct sockaddr_in);
         stServerAddr.addr.sin.sin_family = iDomain;
         stServerAddr.addr.sin.sin_port =  htons(uPort);
         if (strIPAddr == NULL)
         {
             stServerAddr.addr.sin.sin_addr.s_addr = INADDR_ANY;
         }
         else
         {
             iRet= inet_pton(AF_INET, strIPAddr, (void *)&stServerAddr.addr.sin.sin_addr.s_addr);
         }
    }
    else
    {

         stServerAddr.size = sizeof(struct sys_sockaddr_in6);
         stServerAddr.addr.sin6.sin6_family = iDomain;
         stServerAddr.addr.sin6.sin6_port =  htons(uPort);
         if (strIPAddr == NULL)
         {
             memset(&stServerAddr.addr.sin6.sin6_addr.sys_s6_addr, 0, sizeof(stServerAddr.addr.sin6.sin6_addr.sys_s6_addr));
         }
         else
         {
             iRet = inet_pton(AF_INET6, strIPAddr, (void *)&stServerAddr.addr.sin6.sin6_addr.sys_s6_addr);
         }
    }

    iRet = bind(iSockFd, (struct sockaddr *)&stServerAddr.addr, stServerAddr.size);
    if( iRet < 0 )
    {
       SYS_COMMON_ERROR("Bind() error:%s :%d, size: %u, error: %d:%s\n",strIPAddr,uPort, stServerAddr.size, errno, strerror(errno));
       return ERROR;
    }

	return iRet;
}
/**@fn        sys_socket_listen
 * @brief     在socket上建立监听      
 * @param[in] iSockFd 创建的套接字描述符
 * @param[in] iMaxConNum 在套接字上排队的最大连接数
 * @return    成功返回 套接字句柄  错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT32 sys_socket_listen(INT32 iSockFd, INT32 iMaxConNum)
{
    if (listen(iSockFd, iMaxConNum) < 0)
    {
        SYS_COMMON_ERROR("socket_listen failed \n");
        return ERROR;
    }
    return OK;
}

/**@fn          sys_socket_accept
 * @brief       从已完成的连接队列中获取一个已完成的队列      
 * @param[in]   iSockFd   监听套接字
 * @param[out]  strClientIP 接收到的对端地址，可为空
 * @param[out]  uAddrLen, strClientIP 的长度，若strClientIP为空则填0
 * @param[out]  puClientPort 接收到的对端端口，可为空
 * @return      成功返回 套接字句柄 错误返回 ERROR, 支持通过get_last_errno获取错误码
 * @note        puClientPort的存储是本机序，调用者无需再做ntoh转换
 */
INT32 sys_socket_accept(INT32 iSockFd, CHAR *strClientIP, UINT16 uAddrLen, UINT16 *puClientPort)
{   
    INT32 iConnFd = -1;
    struct sys_address_t stClientAddr;
  
    if((NULL == puClientPort) || (NULL == strClientIP))
    {
        SYS_COMMON_ERROR("accept invalid param  puClientPort:%p\n",puClientPort);
        return ERROR;
    }
    
	/* 最大长度默认使用ipv6结构长度 */
    memset(&stClientAddr,0,sizeof(stClientAddr));
    stClientAddr.size = sizeof(struct sys_sockaddr_in6);
    if((iConnFd = accept(iSockFd, (struct sockaddr*)&stClientAddr.addr.sa, (socklen_t *)&stClientAddr.size)) < 0)
    {
    	/* SYS_COMMON_ERROR("socket_accept failed \n"); */
        return ERROR;
    }

    
    if(stClientAddr.addr.sa.sa_family == AF_INET)
    {
        inet_ntop(AF_INET,&stClientAddr.addr.sin.sin_addr.s_addr,strClientIP,uAddrLen);
        *puClientPort = htons(stClientAddr.addr.sin.sin_port); 
    }
    else if(stClientAddr.addr.sa.sa_family == AF_INET6)
    {
        inet_ntop(AF_INET6, &stClientAddr.addr.sin6.sin6_addr,strClientIP,uAddrLen);
        *puClientPort = htons(stClientAddr.addr.sin6.sin6_port); 
    }
    else
    {
        SYS_COMMON_ERROR("socket iDomain is no support iDomain:%d \n", stClientAddr.addr.sa.sa_family);
        return ERROR;
    }
    
    return iConnFd;   
}

/**@fn         sys_socket_connect
 * @brief      连接服务器
 * @param[in]  iSockFd  创建的套接字
 * @param[in]  iDomain 协议族, 参考man 2 socket
 * @param[in]  strIPAddr 要连接的地址
 * @param[in]  uPort 要连接的端口
 * @return      OK/ERROR, 支持通过get_last_errno获取错误码
 */
INT32 sys_socket_connect(INT32 iSockFd, INT32 iDomain, const CHAR *strIPAddr, UINT16 uPort)
{
    INT32 iRet = ERROR;
    struct sys_address_t stClientAddr;

    if (NULL == strIPAddr)
    {
        return ERROR;
    }
   
    if( iDomain != AF_INET && iDomain != AF_INET6)
    {
        SYS_COMMON_ERROR("unkown iDomain: %d \n" ,iDomain);
        return ERROR;
    }
    memset(&stClientAddr, 0, sizeof(stClientAddr));

    if(iDomain == AF_INET)
    {
        stClientAddr.size = sizeof(struct sockaddr_in);
        stClientAddr.addr.sin.sin_family = AF_INET;
        stClientAddr.addr.sin.sin_port = htons(uPort);
        iRet = inet_pton(AF_INET, strIPAddr, (void *)&stClientAddr.addr.sin.sin_addr.s_addr);
    }
    else
    {
        stClientAddr.size = sizeof(struct sys_sockaddr_in6);
        stClientAddr.addr.sin6.sin6_family = AF_INET6;
        stClientAddr.addr.sin6.sin6_port = htons(uPort);
        iRet = inet_pton(AF_INET6, strIPAddr, (void *)&stClientAddr.addr.sin6.sin6_addr.sys_s6_addr);
    }
    
    if( iRet < 0 )
    {
        SYS_COMMON_ERROR("inet_pton addr error \n");
        return ERROR;
    }

    if (connect(iSockFd, (struct sockaddr*)&stClientAddr.addr.sa, stClientAddr.size) < 0)
    {
        SYS_COMMON_ERROR("connect  error, strIPAddr %s, uPort: %u, size: %u, errno: %d: %s\n", strIPAddr, uPort, stClientAddr.size, errno,strerror(errno));
        return ERROR;
    }
    
    return OK;
}

/**@fn         sys_socket_send_to
 * @brief      将数据发送到指定地址上 
 * @param[in]  iSockFd 建立的socket
 * @param[in]  pBuf  要发送的数据
 * @param[in]  iLen 数据长度
 * @param[in]  iDomain 协议族, 参考man 2 socket
 * @param[in]  strIPAddr  目的ip地址
 * @param[in]  uPort 目的端口
 * @return     成功发送的字节数 错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT32 sys_socket_send_to(INT32 iSockFd, void *pBuf, INT32 iLen, INT32 iDomain, const CHAR *strIPAddr, UINT16 uPort)
{   
    INT32 iRet = ERROR;
    INT32 iSendLen = 0 ;
    struct sys_address_t stDstAddr;
    memset(&stDstAddr, 0, sizeof(stDstAddr));

    if( iDomain == AF_INET)
    {
        stDstAddr.size = sizeof(struct sys_sockaddr_in);
        stDstAddr.addr.sin.sin_family = iDomain;
        stDstAddr.addr.sin.sin_port = htons(uPort);
        iRet = inet_pton(AF_INET, strIPAddr, (void *)&stDstAddr.addr.sin.sin_addr);
    }
    else if(iDomain == AF_INET6)
    {
        stDstAddr.size = sizeof(struct sys_sockaddr_in6);
        stDstAddr.addr.sin6.sin6_family = iDomain;
        stDstAddr.addr.sin6.sin6_port = htons(uPort);
        iRet = inet_pton(AF_INET6, strIPAddr, (void *)&stDstAddr.addr.sin6.sin6_addr);
    }
    if( iRet < 0 )
    {
        SYS_COMMON_ERROR("inet_pton addr error \n");
        return ERROR;
    }
	//SYS_COMMON_ERROR("sendto-->iSockFd:%d,buf:%d iLen:%d",iSockFd,strIPAddr,iLen);			
    iSendLen = sendto(iSockFd, pBuf, iLen, 0, (struct sockaddr*)&stDstAddr.addr.sa,stDstAddr.size);
    if (iSendLen < 0)
    {
        SYS_COMMON_ERROR("send to strIPAddr:%s port:%d error:%d :%s \n",strIPAddr,uPort, errno,strerror(errno));
        return ERROR;
    }
    return iSendLen;
}

/**@fn          sys_socket_recvfrom
 * @brief      从对端接收数据      
 * @param[in]  iSockFd  套接字句柄
 * @param[in]  pBuf 接收缓冲区
 * @param[in]  iBufLen 接受缓冲区的长度
 * @param[out] strClientIP 接收到的对端地址
 * @param[out] uAddrLen, strClientIP 的长度
 * @param[out] puClientPort 接收到的对端端口
 * @return     成功接收的字节数 错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT32 sys_socket_recvfrom(INT32 iSockFd, VOID *pBuf, INT32 iBufLen, CHAR *strClientIP, UINT16 uAddrLen, UINT16 *puClientPort)
{   
    INT32 iRecvLen = 0;
    struct sys_address_t stSrcAddr;
    if( iSockFd < 0  ||  pBuf == NULL  || iBufLen == 0  || strClientIP == NULL || puClientPort == NULL )
    {
        SYS_COMMON_ERROR("recvfrom  param  error :%d :%p :%d %p :%p\n",iSockFd,pBuf,iBufLen,strClientIP,puClientPort);
        return ERROR;
    }
    
    memset(&stSrcAddr,0,sizeof(stSrcAddr));
    stSrcAddr.size = sizeof(struct sys_sockaddr_in6);
    iRecvLen = recvfrom(iSockFd, pBuf, iBufLen,0,(struct sockaddr*)&stSrcAddr.addr.sa, (socklen_t *)&stSrcAddr.size);
    if( iRecvLen < 0)
    {
        //SYS_COMMON_ERROR("recv data error \n");
        return iRecvLen;
    }

     if(stSrcAddr.addr.sa.sa_family == AF_INET )
    {
        inet_ntop(AF_INET,&stSrcAddr.addr.sin.sin_addr.s_addr,strClientIP,uAddrLen);
        *puClientPort = htons(stSrcAddr.addr.sin.sin_port); 

    }
    else if(stSrcAddr.addr.sa.sa_family == AF_INET6)
    {
        inet_ntop(AF_INET6,&stSrcAddr.addr.sin6.sin6_addr, strClientIP, uAddrLen);
        *puClientPort = htons(stSrcAddr.addr.sin6.sin6_port); 
    }
//    SYS_COMMON_DEBUG("strClientIP :%s  puClientPort:%d  \n",strClientIP,*puClientPort);
    return iRecvLen;
}

/**@fn         sys_socket_recv
 * @brief      从对端接收数据      
 * @param[in]  iSockFd  套接字句柄
 * @param[in]  pBuf 接收缓冲区
 * @param[in]  iLen 接受缓冲区的长度
 * @return     成功接收的字节数 错误返回 ERROR 返回0代表对端关闭, 支持通过get_last_errno获取错误码
 */
INT32 sys_socket_recv(INT32 iSockFd, VOID *pBuf, INT32 iLen)
{   
    INT32 recv_num = -1;
    if (NULL == pBuf || 0 == iLen || iSockFd < 0)
    {
        SYS_COMMON_ERROR("recv param invaild \n");
        return ERROR;
    }
	
    recv_num = recv(iSockFd, pBuf, iLen, 0);
    return recv_num;
}

/**@fn         sys_socket_send
 * @brief      向对端发送数据      
 * @param[in]  iSockFd  套接字句柄
 * @param[in]  pBuf 发送缓冲区
 * @param[in]  iLen 发送缓冲区的长度
 * @return     成功发送的字节数 错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT32 sys_socket_send(INT32 iSockFd, VOID *pBuf, INT32 iLen)
{   
    INT32 iSendLen = 0 ;
    if( iSockFd < 0 || pBuf == NULL || iLen == 0 )
    {
        SYS_COMMON_ERROR("param invaild \n");
        return ERROR;
    }
    iSendLen = send(iSockFd,pBuf,iLen,0);
    return iSendLen;
}

/**@fn         sys_socket_close
 * @brief      关闭socket      
 * @param[in]  iSockFd  套接字句柄
 * @return     成功返回 0 错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT32 sys_socket_close(INT32 iSockFd)
{
    if(close(iSockFd) < 0)
    {
        SYS_COMMON_ERROR("close socket error \n");
        return ERROR;
    }
    return OK;
}


/* @fn	      sys_socket_convert_optlevel_and_name	  
 * @brief	  转换枚举参数
 * @param[in] iDomain 协议族, 参考man 2 socket
 * @param[in] iType 套接字类型, 参考man 2 socket
 * @param[in] iProtocol 协议类型, 参考man 2 socket
 * @return    成功返回 套接字句柄
 * @return    错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT32 sys_socket_convert_optlevel_and_name(INT32 iOptLevel,INT32 iOptName,INT32 *pConvertOptName,INT32 *pConvertName)
{
    INT32 iRet = ERROR;
    INT32 iAdapteroptlevel = 0;
    INT32 iAdapterOptname = 0;
    if(!pConvertOptName  || !pConvertName)
    {
        return iRet;
    }
    switch(iOptLevel) 
    {
        case SYS_IPPROTO_IP:
        iAdapteroptlevel = IPPROTO_IP;
        break;
        case SYS_SOL_SOCKET:
        iAdapteroptlevel = SOL_SOCKET;
        break;
        default:
        iAdapteroptlevel = iOptLevel;
        /* SYS_COMMON_WARN("no support ,use input :%d \n",iOptLevel); */
        break;
    }

    switch(iOptName) 
    {
        case SYS_IP_MULTICAST_IF:
        iAdapterOptname = IP_MULTICAST_IF;
        break;
        case SYS_IP_MULTICAST_TTL:
        iAdapterOptname = IP_MULTICAST_TTL;
        break;
        case SYS_IP_MULTICAST_LOOP:
        iAdapterOptname = IP_MULTICAST_LOOP;
        break;
        case SYS_IP_ADD_MEMBERSHIP:
        iAdapterOptname = IP_ADD_MEMBERSHIP;
        break;
        case SYS_IP_DROP_MEMBERSHIP:
        iAdapterOptname = IP_DROP_MEMBERSHIP;
        break;
        case SYS_SO_BROADCAST:
        iAdapterOptname = SO_BROADCAST;
        break;   
        case SYS_SO_RCVTIMEO:
        iAdapterOptname = SO_RCVTIMEO;
        break;   
        case SYS_SO_LINGER:
        iAdapterOptname = SO_LINGER;  
        break;
        case SYS_SO_NO_CHECK:
        iAdapterOptname = SO_NO_CHECK;  
        break;
        case SYS_TCP_MAXSEG:
        iAdapterOptname = TCP_MAXSEG;  
        break;
        case SYS_SO_REUSEADDR:
        iAdapterOptname = SO_REUSEADDR;
        break;
        default:
        iAdapterOptname = iOptName;
        /* SYS_COMMON_WARN("no support ,use input :%d \n",iAdapterOptname); */
        break;
    }
    if(iAdapterOptname != iOptName || iOptLevel != iAdapteroptlevel)
    {
        /* SYS_COMMON_WARN("convert iOptName :%d iOptLevel :%d \n",iOptName,iOptLevel); */
    }
    *pConvertOptName =iAdapteroptlevel;
    *pConvertName = iAdapterOptname;
    return OK;
}

/**@fn         sys_socket_set_sock_opt
 * @brief      设置套接字属性
 * @param[in]  iSockFd  套接字句柄
 * @param[in]  iOptLevel  操作层面，比如是套接字层面(SOL_SOCKET)还是协议层面
 * @param[in]  iOptName 操作名称
 * @param[in]  pOptValue  option_name对应的取值
 * @param[in]  iOptLength option_value的长度
 * @return     成功，返回OK；失败参考ERROR_ID_E, 支持通过get_last_errno获取错误码
 */
INT32 sys_socket_set_sock_opt(INT32 iSockFd, INT32 iOptLevel, INT32 iOptName, const VOID *pOptValue, INT32 iOptLength)
{   
    INT32 iRet = ERROR;
    INT32 iAdapteOptLevel = ERROR;
    INT32 iAdapteOptName  = ERROR;
    if (NULL == pOptValue || 0 == iOptLength)
    {
        SYS_COMMON_ERROR("param invaild \n");
        return ERROR;
    }
    iRet = sys_socket_convert_optlevel_and_name(iOptLevel,iOptName,&iAdapteOptLevel,&iAdapteOptName);
    if(iRet < 0)
    {
        /* SYS_COMMON_ERROR("sys_socket_convert_optlevel_and_name error iSockFd:%d iOptLevel:%d iOptName:%d \n",iSockFd, iOptLevel, iOptName); */
        return ERROR;
    }

	iRet = setsockopt(iSockFd, iAdapteOptLevel, iAdapteOptName, pOptValue,(socklen_t)iOptLength);
    if (iRet < 0)
    {
        /* SYS_COMMON_ERROR("setsockopt error iSockFd:%d iOptLevel:%d iOptName:%d pOptValue:%p errno:%d :%s \n",iSockFd, iAdapteOptLevel, iAdapteOptName, pOptValue,errno,strerror(errno)); */
        return ERROR;
    }
    
    return OK;
}


/**@fn         sys_socket_get_sock_opt
 * @brief      获取套接字属性
 * @param[in]  iSockFd  套接字句柄
 * @param[in]  iOptLevel  操作层面，比如是套接字层面(SOL_SOCKET)还是协议层面
 * @param[in]  iOptName 操作名称
 * @param[in]  pOptValue  option_name对应的取值
 * @param[in/out]  iOptLength 调用者填写option_value的长度, 返回时代表实际长度
 * @return     成功，返回OK；失败参考ERROR_ID_E, 支持通过get_last_errno获取错误码
 */
INT32 sys_socket_get_sock_opt(INT32 iSockFd, INT32 iOptLevel, INT32 iOptName, VOID *pOptValue, INT32 *iOptLength)
{
    if(NULL == pOptValue || NULL == iOptLength || 0 == *iOptLength)
    {
        SYS_COMMON_ERROR("param invaild \n");
        return ERROR;
    }
	
    if (getsockopt(iSockFd, iOptLevel, iOptName, pOptValue, (UINT32 *)iOptLength) < 0)
    {
        SYS_COMMON_ERROR("getsockopt invaild iOptName:%d \n",iOptName);
        return ERROR;
    }
    
    return OK;
}

/**@fn         sys_socket_shutdown
 * @brief      shutdown socket      
 * @param[in]  iSockFd  套接字句柄
 * @param[in]  iHow 选项
 * @return     成功返回 0错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT32 sys_socket_shutdown(INT32 iSockFd, INT32 iHow)
{
    INT32 iHowShutdown = 0;
	
	if(iSockFd < 0)
    {
        return ERROR;
    }

    switch(iHow)
    {
        case SYS_SOCKET_SHUTDONW_TYPE_INPUT:
            iHowShutdown = SHUT_RD;
            break;
        case SYS_SOCKET_SHUTDONW_TYPE_OUTPUT:
            iHowShutdown = SHUT_WR;
            break;
        case SYS_SOCKET_SHUTDONW_TYPE_BOTH:
            iHowShutdown = SHUT_RDWR;
            break;
        default:
            SYS_COMMON_ERROR("unknown iHow:%d\n", iHow);
            return ERROR;
    }
    
    if(shutdown(iSockFd, iHowShutdown) < 0)
    {
        SYS_COMMON_ERROR("shutdown failed \n");
        return ERROR;
    }
    return OK;
}

/**@fn         sys_socket_get_local_addr      
 * @brief      获取套接字本地地址
 * @param[in]  iSockFd 套接字
 * @param[out] pStLocalAddr 本地套接字结构体指针
 * @param[out] pUAddrLength 结构体大小
 * @return     成功返回 套接字句柄 错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT32 sys_socket_get_local_addr(INT32 iSockFd, struct sys_sockaddr *pStLocalAddr, socklen_t *pUAddrLength)
{
    INT32 iRet = ERROR; 
    if(iSockFd < 0)
    {
        return ERROR;
    }
    if( pStLocalAddr == NULL || pUAddrLength == NULL )
    {
        SYS_COMMON_ERROR("param invalid \n");
        return ERROR;
    }
    iRet = getsockname(iSockFd,(struct sockaddr*) pStLocalAddr, (socklen_t *)pUAddrLength);
    return iRet;
}

/**@fn         sys_socket_get_peer_addr      
 * @brief      获取套接字对端地址
 * @param[in]  iSockFd 套接字
 * @param[out] pStRemoteAddr 远程套接字结构体指针
 * @param[out] pUAddrLength  结构体大小
 * @return     成功返回 套接字句柄 错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT32 sys_socket_get_peer_addr(INT32 iSockFd, struct sys_sockaddr *pStRemoteAddr, socklen_t *pUAddrLength)
{
    INT32 iRet = ERROR; 
    if(iSockFd < 0)
    {
        return ERROR;
    }
    if( pStRemoteAddr == NULL || pUAddrLength == NULL )
    {
        SYS_COMMON_ERROR("param invalid \n");
        return ERROR;
    }
    iRet = getpeername(iSockFd, (struct sockaddr*)pStRemoteAddr, (socklen_t *)pUAddrLength);
    return iRet;
}

/**@fn         sys_socket_inet_ntop      
 * @brief      转换网络二进制结构到ASCII类型的地址
 * @param[in]  iAf  地址协议族 iAf_INET、iAf_INET6
 * @param[in]  pSrcAddr    地址结构体指针,iAf_INET时地址结构是in_addr、iAf_INET6时指向in6_addr结构体指针
 * @param[out] strDstAddr  网络序表示的32位IP地址
 * @param[out] uSize       网络序表示的32位IP地址
 * @return     成功,返回非NULL字符串；失败返回NULL,错误见ERRNO
 */
const CHAR *sys_socket_inet_ntop(INT32 iAf, const VOID *pSrcAddr, CHAR *strDstAddr, socklen_t uSize)
{
    if(pSrcAddr == NULL || strDstAddr == NULL || uSize == 0 )
    {
        return NULL;
    } 
 
    return inet_ntop(((iAf == SYS_AF_INET) ? AF_INET : AF_INET6),pSrcAddr,strDstAddr,uSize);
}

/**@fn         sys_socket_inet_pton      
 * @brief      转换ASCII类型的地址到网络十进制结构
 * @param[in]  iAf  地址协议族 iAf_INET、iAf_INET6
 * @param[in]  pSrcAddr    地址结构体指针,iAf_INET时地址结构是in_addr、iAf_INET6时指向in6_addr结构体指针
 * @param[out] strDstAddr  网络序表示的32位IPV4地址或者128位的IPV6地址
 * @return     成功返回OK 失败返回ERROR
 */
INT32 sys_socket_inet_pton(INT32 iAf, const CHAR *pSrcAddr, VOID *strDstAddr)
{
    if((pSrcAddr == NULL) || (strDstAddr == NULL))
    {
        return ERROR;
    } 
 
    return inet_pton(((iAf == SYS_AF_INET) ? AF_INET : AF_INET6), pSrcAddr, strDstAddr);
}

/**@fn         sys_socket_inet_addr
 * @brief      将一个点分十进制的IP转换成一个长整数型数（u_long类型）
 * @param[in]  strIP  地址字符串 
 * @param[out] pUAdddr 
 * @return     成功则将字符串转换为32位二进制网络字节序的IPV4地址，否则为INADDR_NONE
 */
INT32 sys_socket_inet_addr(const CHAR *strIP,ULONG *pUAdddr)
{   
    INT32 iRet = ERROR;
    in_addr_t staddr = -1;
    if( strIP == NULL || pUAdddr == NULL)
    {
        SYS_COMMON_ERROR("invaild param error\n");
        return iRet;
    }
    staddr = inet_addr(strIP);
    *pUAdddr = staddr;
    return OK;
}

/**@fn         sys_socket_inet_ntoa
 * @brief      将一个十进制网络字节序转换为点分十进制IP格式的字符串
 * @param[in]  in  32位的IPv4地址
 * @return     成功返回一个字符指针，指向一块存储着点分格式IP地址的静态缓冲区（同一线程内共享此内存)错误，返回NULL。
 */
CHAR *sys_socket_inet_ntoa(struct sys_in_addr in)
{
    struct in_addr strInaddr = {0};
    if(sizeof(struct in_addr) != sizeof(struct sys_in_addr))
    {
        return NULL;
    }
    memcpy(&strInaddr,&in,sizeof(strInaddr));
    return inet_ntoa(strInaddr);
}

/**@fn         sys_socket_inet_aton
 * @brief      将一个点分十进制IP格式的字符串转换为十进制网络字节序
 * @param[in]  strIp  十进制IP格式的字符串
 * @return     成功返回0错误，返回 -1。
 */
INT32 sys_socket_inet_aton(const CHAR *strIp, struct sys_in_addr *pStInp)
{
    INT32 iRet = -1 ;
    if( strIp == NULL || pStInp == NULL )
    {
        SYS_COMMON_ERROR("param is null \n");
        return iRet;
    }
    iRet = inet_aton(strIp,(struct in_addr *)pStInp);
    return iRet;
}

/**@fn          sys_socket_get_tcp_state      
 * @brief      获取tcp状态
 * @param[in]  iSockFd 套接字
 * @return     成功返回 tcp链接状态 错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
INT32 sys_socket_get_tcp_state(int iSockFd)
{
    INT32 iRet = ERROR;
    socklen_t iLen =  0 ;
    struct tcp_info stInfo;
    memset(&stInfo, 0, sizeof(stInfo));
    iLen = sizeof(stInfo);
    iRet = sys_socket_get_sock_opt(iSockFd, SYS_IPPROTO_TCP, TCP_INFO, &stInfo, (INT32 *)&iLen);
    if(iRet < 0)
    {
        return iRet;
    }
    return stInfo.tcpi_state;
}

/**@fn         sys_socket_get_last_errno      
 * @brief      获取套接字错误码
 * @param[in]  无
 * @return     详见errno 
 */
INT32 sys_socket_get_last_errno()
{
    return errno;
}
