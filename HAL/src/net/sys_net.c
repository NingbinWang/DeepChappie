#include"sys_net_priv.h"

/* @fn	      sys_net_set_ip	  
 * @brief	  配置IP
 * @param[in] strDevName 设备网卡名
 * @param[in] iDomain    协议族
 * @param[in] strIP      IP地址
 * @return 成功返回0错误，返回 -1
 */
INT sys_net_set_ip(const CHAR *strDevName, INT iDomain, const CHAR *strIP)
{   
    INT iFd = -1;
    struct ifreq stIfreq;
	struct sockaddr_in stIPaddr;
    if(NULL == strDevName || NULL == strIP)
    {
        return ERROR;
    }
    if((iFd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        SYS_COMMON_ERROR("create socket failed!\n");
        return ERROR;
    }

    snprintf(stIfreq.ifr_name, sizeof(stIfreq.ifr_name), "%s", strDevName);

    memset(&stIPaddr, 0, sizeof(struct sockaddr_in));
    stIPaddr.sin_family = PF_INET;
	inet_aton(strIP, &stIPaddr.sin_addr);

    memcpy(&stIfreq.ifr_ifru.ifru_addr, &stIPaddr, sizeof(struct sockaddr_in));
    if(ioctl(iFd, SIOCSIFADDR, &stIfreq) < 0)
   	{
		SYS_COMMON_ERROR("IP_set ioctl failed. error: %s!\n", strerror(errno));
		close(iFd);
		return ERROR;
	}

    close(iFd);

	SYS_COMMON_INFO("== IP set OK!!! if: %s, ip: %s ==\n", strDevName, strIP);
    return OK;
}

/**@fn         sys_net_get_ip
 * @brief      获取设备IP地址
 * @param[in]  strEthName  网卡名
 * @param[in]  iDomain     协议族
 * @param[in]  strIpAddr   缓冲区地址指针
 * @param[in]  uLength     缓冲区大小长度
 * @return 成功返回0错误，返回 -1
 */
INT sys_net_get_ip(const CHAR* strEthName, INT iDomain, CHAR *strIpAddr, UINT16 uLength)
{
    INT iFd = -1;
    struct ifreq stIfreq;
	struct sockaddr_in *pstIPaddr;
    FILE *pFileId;
    CHAR strDevName[20] = {0};
    CHAR strIpV6[8][5];
    INT iIndex = 0;
    INT iPrefix = 0;
    INT iScope = 0;
    INT iFlags = 0;
    
    if (NULL == strEthName || NULL == strIpAddr)
    {
        return ERROR;
    }
    
	if(PF_INET6 == iDomain)
	{
		pFileId = fopen(SYS_NET_IFINET6_PATH, "r");
		if (NULL == pFileId)
        {
            return ERROR;
        }     
		while (fscanf(pFileId, "%4s%4s%4s%4s%4s%4s%4s%4s %02x %02x %02x %02x %20s\n",
			strIpV6[0], strIpV6[1], strIpV6[2], strIpV6[3],
			strIpV6[4], strIpV6[5], strIpV6[6], strIpV6[7],
			 &iIndex, &iPrefix, &iScope, &iFlags, strDevName) != EOF) {
			if (!strcmp(strDevName, strEthName))
            {
				sprintf(strIpAddr, "%s:%s:%s:%s:%s:%s:%s:%s",
					strIpV6[0], strIpV6[1], strIpV6[2], strIpV6[3],
					strIpV6[4], strIpV6[5], strIpV6[6], strIpV6[7]);
				fclose(pFileId);
		        return OK;
			}
		}	
		fclose(pFileId);
		return ERROR;
	}

    if((iFd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        SYS_COMMON_ERROR("create socket failed!\n");
        return ERROR;
    }
    snprintf(stIfreq.ifr_name, sizeof(stIfreq.ifr_name), "%s", strEthName);
    if(ioctl(iFd, SIOCGIFADDR, &stIfreq) < 0)
    {
        SYS_COMMON_ERROR("get_IP:%s : ioctl failed! error: %s\n",strEthName,strerror(errno));
        close(iFd);
        return ERROR;
	}
    pstIPaddr = (struct sockaddr_in *)&stIfreq.ifr_addr;
    snprintf(strIpAddr, uLength, "%s", inet_ntoa(pstIPaddr->sin_addr));
    close(iFd); 
    return OK;
}
/**@fn         sys_net_set_mac_addr
 * @brief      设置设备mac地址
 * @param[in]  strEthName  网卡名
 * @param[in]  pStMacAddr  缓冲区地址指针
 * @param[in]  uLength     缓冲区大小长度
 * @return 成功返回0错误，返回 -1
 */
INT sys_net_set_mac_addr(const CHAR* strEthName,UINT8 *pStMacAddr,UINT16 uLength)
{

    INT iRet = -1;
    INT iFd = -1;
    struct ifreq stIfreq;
    if (NULL == pStMacAddr || NULL == strEthName ) 
    {
        return iRet;
    }
    snprintf(stIfreq.ifr_name,sizeof(stIfreq.ifr_name),"%s",strEthName);
	memcpy(stIfreq.ifr_hwaddr.sa_data,pStMacAddr,uLength);
    if ((iFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        return iRet;
    }
    iRet = ioctl(iFd, SIOCSIFHWADDR, &stIfreq, sizeof(stIfreq));
    if(iRet < 0)
    {
        close(iFd);
        return iRet;
    }
    
    close(iFd);
    return iRet;
}
/**@fn         sys_net_get_mac_addr
 * @brief      获取设备mac地址
 * @param[in]  strEthName  网卡名
 * @param[in]  pStMacAddr  缓冲区地址指针
 * @param[in]  uLength     缓冲区大小长度
 * @return 成功返回0错误，返回 -1
 */
INT sys_net_get_mac_addr(const CHAR* strEthName,UINT8 *pStMacAddr,UINT16 uLength)
{
    INT iRet = -1;
    INT iFd = -1;
    struct ifreq stIfreq;
    if (NULL == pStMacAddr || NULL == strEthName ) 
    {
        return iRet;
    }
    snprintf(stIfreq.ifr_name,sizeof(stIfreq.ifr_name),"%s",strEthName);
    if ((iFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        return iRet;
    }
    iRet = ioctl(iFd, SIOCGIFHWADDR, &stIfreq, sizeof(stIfreq));
    if(iRet < 0)
    {
        close(iFd);
        return iRet;
    }
    memcpy(pStMacAddr,stIfreq.ifr_hwaddr.sa_data,uLength);
    close(iFd);
    return iRet;
}
/**@fn         sys_net_set_netmask
 * @brief      设置掩码
 * @param[in]  strEthName      网卡名
 * @param[in]  pStAddr         缓冲区地址指针
 * @param[in]  uLength         缓冲区大小长度
 * @return 成功返回0错误，返回 -1
 */
INT sys_net_set_netmask(const CHAR* strEthName,UINT8 *pStAddr,UINT16 uLength)
{

    return ERROR;
}
/**@fn         sys_net_get_netmask
 * @brief      获取掩码
 * @param[in]  strEthName      网卡名
 * @param[in]  pStAddr         缓冲区地址指针
 * @param[in]  uLength         缓冲区大小长度
 * @return 成功返回0错误，返回 -1
 */
INT sys_net_get_netmask(const CHAR* strEthName,UINT8 *pStAddr,UINT16 uLength)
{

    return ERROR;
}
/**@fn         sys_net_set_gateway
 * @brief      设置网关
 * @param[in]  strEthName      网卡名
 * @param[in]  pStAddr         缓冲区地址指针
 * @param[in]  uLength         缓冲区大小长度
 * @return 成功返回0错误，返回 -1
 */
INT sys_net_set_gateway(const CHAR* strEthName,UINT8 *pStAddr,UINT16 uLength)
{

    return ERROR;
}

/**@fn         sys_net_get_gateway
 * @brief      获取网关
 * @param[in]  strEthName      网卡名
 * @param[in]  pStAddr         缓冲区地址指针
 * @param[in]  uLength         缓冲区大小长度
 * @return 成功返回0错误，返回 -1
 */
INT sys_net_get_gateway(const CHAR* strEthName,UINT8 *pStAddr,UINT16 uLength)
{

    return ERROR;
}

/**@fn         sys_net_get_ip_mac
 * @brief      获取每个IP的mac地址
 * @param[in]  strIpAddr       IP地址
 * @param[in]  strNetCartName  网卡名
 * @param[out] strMac          mac地址
 * @param[in]  uLength         缓冲区长度
 * @return 成功返回0错误，返回 -1
 */
INT sys_net_get_ip_mac(CHAR *strIpAddr, const CHAR *strNetCartName, UINT8 *strMac, UINT8 uLength)
{
    INT               iSockfd = -1;
    UINT8              *ptr     = NULL;
    struct arpreq       arpreq;
    struct sockaddr_in *sin = NULL;

    memset(&arpreq, 0x00, sizeof(arpreq));

    if (!strIpAddr || !strMac || !strNetCartName)
    {
        printf("invaild param \n");
        return -1;
    }

    iSockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (iSockfd < 0)
    {
        perror("socket error");
        return -1;
    }

    sin                  = (struct sockaddr_in *)&arpreq.arp_pa;
    sin->sin_family      = AF_INET;
    sin->sin_addr.s_addr = inet_addr(strIpAddr);
    strncpy((char *)arpreq.arp_dev, (char *)strNetCartName, sizeof(arpreq.arp_dev) - 1);
    arpreq.arp_ha.sa_family = AF_UNSPEC;
    if (ioctl(iSockfd, SIOCGARP, &arpreq) < 0)
    {
        close(iSockfd);
        printf("ioctl SIOCGARP :%s :%s %d\n", strIpAddr, strNetCartName, errno);
        return -1;
    }
    ptr = (UINT8 *)arpreq.arp_ha.sa_data;
    memcpy(strMac, ptr, 6);
    SYS_COMMON_INFO("ip :%s device:%s %02x:%02x:%02x:%02x:%02x:%02x \n", strIpAddr, strNetCartName, strMac[0], strMac[1],
                    strMac[2], strMac[3], strMac[4], strMac[5]);
    close(iSockfd);

    return 0;
}

/* @brief	  检测网络设备接口结点是否存在
 * @param[in] sDevName 设备名称
 * @return 成功返回 OK
 * @return 错误返回 ERROR, 不支持通过get_last_errno获取错误码
 */
INT sys_net_dev_node_check(const CHAR *strDevName)
{
    INT iRet = ERROR;
    INT iSocketFd = -1;
	struct ifreq ifr;
    if(!strDevName)
    {
        return iRet;
    }
    iSocketFd = socket(PF_INET, SOCK_STREAM, 0);
	if(iSocketFd < 0)
	{
		SYS_COMMON_ERROR("create socket failed!\n");
		return ERROR;
	}
	snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "%s", strDevName);
    if(ioctl(iSocketFd, SIOCGIFFLAGS, &ifr) < 0)
    {
        SYS_COMMON_ERROR("SIOCGIFFLAGS failed. dev may not exist!\n");
        close(iSocketFd);
        return ERROR;
    }

    if(!(ifr.ifr_ifru.ifru_flags&(IFF_UP)))
    {
        SYS_COMMON_INFO("%s is down!\n", strDevName);
        close(iSocketFd);
        return OK;
    }
    close(iSocketFd);
    iRet = 1;
    return iRet;
}

/**@fn        sys_net_ifconfig_up
 * @brief	  启动网卡
 * @param[in] sDevName 网卡名
 * @return 成功返回0错误，返回 -1
 */
INT sys_net_ifconfig_up(const CHAR *strName)
{
    int fd, rtn;
    struct ifreq ifr;        

    if (!strName) {
        return -1;
    }

    fd = socket(AF_INET, SOCK_DGRAM, 0 );
    if ( fd < 0 ) {
        perror("socket");
        return -1;
    }
    
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, (const char *)strName, IFNAMSIZ - 1 );

    if ( (rtn = ioctl(fd, SIOCGIFFLAGS, &ifr) ) == 0 ) 
    { 
        ifr.ifr_flags |= IFF_UP;
    }

    if ( (rtn = ioctl(fd, SIOCSIFFLAGS, &ifr) ) != 0) {
        perror("SIOCSIFFLAGS");
    }

    close(fd);

    return rtn;
}
/**@fn        sys_net_ifconfig_down
 * @brief	  关闭网卡
 * @param[in] sDevName 网卡名
 * @return 成功返回0错误，返回 -1
 */
INT sys_net_ifconfig_down(const CHAR *strName)
{
    int fd, rtn;
    struct ifreq ifr;        

    if (!strName) {
        return -1;
    }

    fd = socket(AF_INET, SOCK_DGRAM, 0 );
    if ( fd < 0 ) {
        perror("socket");
        return -1;
    }
    
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, (const char *)strName, IFNAMSIZ - 1 );

    if ( (rtn = ioctl(fd, SIOCGIFFLAGS, &ifr) ) == 0 ) 
    {
        ifr.ifr_flags &= ~IFF_UP;
    }

    if ( (rtn = ioctl(fd, SIOCSIFFLAGS, &ifr) ) != 0) 
    {
        perror("SIOCSIFFLAGS");
    }

    close(fd);

    return rtn;
}

/* @fn	      sys_net_ifconfig	  
 * @brief	  配置网卡IP、网关、掩码
 * @param[in] strDevName 设备网卡名
 * @param[in] strIP      IP地址
 * @param[in] strNetMask 掩码
 * @param[in]strGateWay  网关
 * @return 成功返回0错误，返回 -1
 */
INT sys_net_ifconfig(const CHAR *strDevName, const CHAR *strIP,const CHAR *strNetMask, const CHAR *strGateWay)
{
    /*1 set ip*/
    INT iFd = -1;
    struct ifreq stIfreq;
    struct sockaddr_in staddr;
	struct rtentry rt;
    if(!strDevName || !strIP || !strNetMask || !strGateWay)
    {
        SYS_COMMON_ERROR("param error\n");
	    return ERROR;
    }
    if((iFd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {
	    SYS_COMMON_ERROR("create socket failed!\n");
	    return ERROR;
    }
    snprintf(stIfreq.ifr_name, sizeof(stIfreq.ifr_name), "%s", strDevName);
    memset(&staddr, 0, sizeof(struct sockaddr_in));
    staddr.sin_family = PF_INET;
    if(inet_aton(strIP, &staddr.sin_addr)<0)
    {
        SYS_COMMON_ERROR("inet_aton error\n" );
        close(iFd);
        return ERROR;
    }
    memcpy(&stIfreq.ifr_ifru.ifru_addr, &staddr, sizeof(struct sockaddr_in));
    if(ioctl(iFd, SIOCSIFADDR, &stIfreq) < 0)
    {
	   SYS_COMMON_ERROR("IP_set ioctl failed. error: %s!\n", strerror(errno));
	   close(iFd);
	   return ERROR;
    }
	
    /*2 set netmask*/
    memset(&staddr, 0, sizeof(struct sockaddr_in));
    staddr.sin_family = PF_INET;
	if(inet_aton(strNetMask, &staddr.sin_addr)<0)
    {
        SYS_COMMON_ERROR("inet_aton error\n" );
        close(iFd);
        return ERROR;
    }
    memcpy(&stIfreq.ifr_ifru.ifru_addr, &staddr, sizeof(struct sockaddr_in));
    if(ioctl(iFd, SIOCSIFNETMASK, &stIfreq) < 0)
    {
	   SYS_COMMON_ERROR("IP_set ioctl failed. error: %s!\n", strerror(errno));
	   close(iFd);
	   return ERROR;
    }
	
    /*3 set gateway*/
	memset(&rt, 0, sizeof(struct rtentry));
    memset(&staddr, 0, sizeof(struct sockaddr_in));
    staddr.sin_family = PF_INET;
    staddr.sin_port = 0;
    if(inet_aton(strGateWay, &staddr.sin_addr)<0)
    {
        SYS_COMMON_ERROR("inet_aton error\n" );
        close(iFd);
        return ERROR;
    }
    memcpy(&rt.rt_gateway, &staddr, sizeof(struct sockaddr_in));
    ((struct sockaddr_in *)&rt.rt_dst)->sin_family=AF_INET;
    ((struct sockaddr_in *)&rt.rt_genmask)->sin_family=AF_INET;
    rt.rt_flags = RTF_GATEWAY;
    if (ioctl(iFd, SIOCADDRT, &rt)<0)
    {
        SYS_COMMON_ERROR("ioctl(SIOCADDRT) error in set_default_route\n");
        close(iFd);
        return ERROR;
    }
    close(iFd);

    SYS_COMMON_INFO("== set OK!!! if: %s, ip: %s mask: %s ip: %s==\n", strDevName,strIP,strNetMask,strGateWay);
    return OK;
}

/* @fn	      sys_net_get_eth_list	  
 * @brief	  获取网卡列表
 * @param[out] astrEhtName 设备网卡列表，二维数组,strEth[Index][EthNameBuffer]
 * @param[in] uNameSize   网卡名称Buffer大小
 * @param[in] uNumMax     获取最大网卡数
 * @return 成功返回0;错误，返回 -1
 */
INT sys_net_get_eth_list(CHAR **astrEhtName, UINT8 uNameSize, UINT8 uNumMax)
{
    INT iIndex = 0;
    INT iEthNum = 0;
    INT iSocket = 0;
    INT iRet = ERROR;
    struct ifconf ifconf;
    if(astrEhtName == NULL)
    {
        return ERROR;
    }
    struct ifreq interfaces[uNumMax];
    // ioctl 调用需要socket 对象
    iSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if(iSocket < 0)
    {
	    SYS_COMMON_ERROR("create socket failed!\n");
	    return ERROR;
    }
    // get if vector
    ifconf.ifc_len = sizeof(interfaces);
	ifconf.ifc_req = interfaces;
	//获取所有网络接口信息
	iRet = ioctl(iSocket, SIOCGIFCONF, &ifconf);
    if(iRet < 0)
    {
        SYS_COMMON_ERROR("call ioctl SIOCGIFCONF error\n");
        close(iSocket);
        return iRet;
    }
	iEthNum = ifconf.ifc_len/sizeof(struct ifreq);
	SYS_COMMON_INFO("interfaces nums = %d\n",iEthNum);
	struct ifreq *ifa = interfaces;
	for (iIndex=0; iIndex<iEthNum; iIndex++,ifa++) {
        strncpy(astrEhtName[iIndex],ifa->ifr_name,sizeof(ifa->ifr_name));
    }
    close(iSocket);
	return OK;
}

