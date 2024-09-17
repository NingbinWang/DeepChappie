#include "devmonitor_priv.h"


 /**@fn	       sys_dev_monitor_get_priv_data	  
 * @brief	   获取串口对象私有数据 
 * @param[in]  pIDevMonitor      串口对象操作指针
 * @return	   成功 私有数据指针 失败 NULL
 */
static DEV_MONITOR_PRIV_DATA_T *devmonitor_get_priv_data(IDevMonitor *pIDevMonitor)
{
    DEV_MONITOR_BASE_T *pBase =NULL;
    if(!pIDevMonitor)
    {
        LOGGER_ERROR("invaild param \n");
        return NULL;
    }
    pBase = (DEV_MONITOR_BASE_T*)pIDevMonitor;
    return &pBase->stPrivData;
}

 /**@fn	       sys_dev_monitor_init_hotplug_sock	  
 * @brief	   创建netlink 套接字 
 * @param[in]  无
 * @return	   成功域套接字接口,失败ERROR
 */
INT devmonitor_init_hotplug_sock(VOID) 
{ 
    INT iRet = ERROR; 
    INT iSockfd = ERROR;
    struct sockaddr_nl stSockNl; 
    bzero(&stSockNl, sizeof(struct sockaddr_nl)); 
    stSockNl.nl_family = AF_NETLINK; 
    stSockNl.nl_pid = getpid(); 
    stSockNl.nl_groups = 3; 

    iSockfd = socket(AF_NETLINK, SOCK_RAW, NETLINK_KOBJECT_UEVENT); 
    if (iSockfd < 0)  
    { 
        LOGGER_ERROR("socket create error \n"); 
        return iRet; 
    } 

    iRet = bind(iSockfd, (struct sockaddr *)&stSockNl, sizeof(struct sockaddr_nl)); 
    if (iRet < 0)  
    { 
        LOGGER_ERROR("socket bind error \n"); 
        close(iSockfd); 
        return iRet; 
    } 

    return iSockfd; 
} 

/**@fn	       dev_monitor_netlink_data_handle	  
 * @brief	   NetLink数据解析
 * @param[in]  pStPrivData          私有数据指针
 * @param[in]  strBuf               缓冲区指针
 * @param[in]  uLength              缓冲区长度
 * @return	   无
 */
void devmonitor_netlink_data_handle(DEV_MONITOR_PRIV_DATA_T *pStPrivData, CHAR *strBuf, UINT uLength)
{       
    INT iRet = -1;
    INT iMediumNo = 0;
    INT iMediumPartNo = 1;
    DEV_MONITOR_ACTION_E eAction   = DEV_MONITOR_ACTION_INVAILD;
    DEV_MONITOR_TYPE_E eType = DEV_MONITOR_TYPE_INVAILD;
    CHAR strBlockPath[32]  = {0};
    CHAR *strBlock = NULL;
    CHAR strDriver = 0;
    if((NULL == pStPrivData) || (NULL == strBuf))
    {
        LOGGER_ERROR("invaild param \n");
        return ;
    }
    
    if(strstr(strBuf, DEV_MONITOR_ACTION_SYMBOL_ADD) != NULL)
    {
        eAction = DEV_MONITOR_ACTION_ADD;
    }
    else if(strstr(strBuf, DEV_MONITOR_ACTION_SYMBOL_REMOVE) != NULL)
    {
        eAction = DEV_MONITOR_ACTION_REMOVE;
    }else if(strstr(strBuf,DEV_MONITOR_ACTION_SYMBOL_CHANGE) != NULL)
    {
        eAction = DEV_MONITOR_ACTION_CHANGE;
    }
    else
    {   
        LOGGER_INFO("ignore netlink event info :%s \n",strBuf);
        return;
    }
    strBlock = strstr(strBuf,"block");
    if(NULL == strBlock)
    {
        return ;
    }

    //parse medium  no  action
    iRet = sscanf(strBlock,"block/%s",strBlockPath);
    if(iRet < 0)
    {
        LOGGER_ERROR("%s\n", strBuf);
        return ;
    }
    
    if(strlen(strBlockPath) == 0)
    {   
        LOGGER_ERROR("ignore netlink event info :%s \n",strBuf);
        return;
    }
    if(strstr(strBlockPath,"/") != NULL)
    {
        LOGGER_INFO("ignore netlink event info :%s \n",strBuf);
        return;
    }
    if(NULL == pStPrivData->pCallBack)
    {   
        LOGGER_ERROR("no medium callback \n");
        return ;
    }
    if(strstr(strBlockPath, "mmc") != NULL)
    {
        eType = DEV_MONITOR_TYPE_TF;
        sscanf(strBlockPath,"%*[^0-9]%1d",&iMediumNo);
    }
    else if(strstr(strBlockPath, "sd") != NULL)
    {
        eType = DEV_MONITOR_TYPE_HARD_DISK;
        sscanf(strBlockPath,"sd%c", &strDriver);
        iMediumNo = strDriver - 'a';
    }
    else
    {   
        LOGGER_WARN("invaild type :%s \n",strBlockPath);
        return ;
    }
    LOGGER_INFO(" iMediumNo:%d :%d medium action:%d\n", iMediumNo, eType, eAction);
    pStPrivData->pCallBack(eType, iMediumNo, iMediumPartNo, eAction);
}

/**@fn	       sys_dev_monitor_thread_loop	  
 * @brief	   设备监听线程
 * @param[in]  pStPrivData          私有数据指针
 * @return	   无
 */
VOID devmonitor_thread_work(DEV_MONITOR_PRIV_DATA_T *pStPrivData)
{       
    INT iRet = -1;
    if((NULL == pStPrivData) || (pStPrivData->iFd < 0))
    {
        LOGGER_ERROR("sys_dev_monitor_init_hotplug_sock error \n");
        return;
    }

    while(1)
    {   
        if(pStPrivData->bExist == TRUE)
        {
            break;
        }
        memset(pStPrivData->strBuf, 0, sizeof(pStPrivData->strBuf));
        iRet = recv(pStPrivData->iFd, pStPrivData->strBuf, sizeof(pStPrivData->strBuf) - 1, 0);
        if(iRet < 0)
        {
            sys_time_sleep_ms(2);
            continue;
        }
		pStPrivData->strBuf[iRet] = '\0';
        LOGGER_INFO("recv:%s\n",pStPrivData->strBuf);
        devmonitor_netlink_data_handle(pStPrivData, pStPrivData->strBuf, iRet);
    }
    
    close(pStPrivData->iFd);
}

/**@fn         devmonitor_register_action_callback      
 * @brief      注册设备监听回调函数
 * @param[in]  pIDevMonitor  设备监听对象
 * @param[in]  eType         设备监听类型
 * @param[in]  pCallBack     回调函数
 * @return     无
 */
VOID devmonitor_register_action_callback(IDevMonitor *pIDevMonitor, DEV_MONITOR_TYPE_E eType, DevActionCallBack pCallBack)
{
    DEV_MONITOR_PRIV_DATA_T *pStPrivData = NULL;
    if(NULL == pIDevMonitor || (NULL == pCallBack))
    {
        LOGGER_ERROR("invaild param error :%p \n",pIDevMonitor);
        return ;
    }
    pStPrivData = devmonitor_get_priv_data(pIDevMonitor);
    if(NULL == pStPrivData) 
    {
        LOGGER_ERROR("get priv data error \n");
        return ;
    }
    if(NULL != pStPrivData->pCallBack)
    {
        LOGGER_ERROR("no suppor now \n");
        return ;
    }
    pStPrivData->pCallBack = pCallBack;
}

/**@fn         devmonitor_check_dev      
 * @brief      设备节点检测
 * @param[in]  pIDevMonitor  设备监听对象
 * @param[in]  eType         设备监听类型
 * @param[in]  iDevIndex     设备节点索引
 * @return     无
 */
static INT devmonitor_check_dev(IDevMonitor *pIDevMonitor,DEV_MONITOR_TYPE_E eType, INT iDevIndex)
{
    DEV_MONITOR_PRIV_DATA_T *pStPrivData = NULL;
    if(NULL == pIDevMonitor)
    {
        LOGGER_ERROR("invaild param error :%p \n",pIDevMonitor);
        return -1;
    }

    pStPrivData = devmonitor_get_priv_data(pIDevMonitor);
    if( NULL == pStPrivData)
    {
        LOGGER_ERROR("get priv data error \n");
        return -1;
    }
    

    return OK;
}

/**@fn	       devmonitor_init_priv_data	  
 * @brief	   初始化私有数据
 * @param[in]  pStPrivData          私有数据指针
 * @return	   无
 */
static INT devmonitor_init_priv_data(DEV_MONITOR_PRIV_DATA_T *pStPrivData)
{
    if(!pStPrivData)
    {
        LOGGER_ERROR("init pPriv Data error\n");
        return ERROR;
    }

    return OK;
}

/**@fn	       devmonitor_init	  
 * @brief	   初始化模块
 * @param[in]  无
 * @return	   成功返回IDevMonitor操作指针  失败返回NULL;
 */
static INT devmonitor_init(IDevMonitor *pIDevMonitor)
{   
    INT iRet = ERROR;
    DEV_MONITOR_PRIV_DATA_T *pStPrivData = NULL;
    if(NULL == pIDevMonitor)
    {
        LOGGER_ERROR("init interface func error\n");
        return iRet;
    }
    pStPrivData = devmonitor_get_priv_data(pIDevMonitor);
    if( NULL == pStPrivData)
    {
        LOGGER_ERROR("get priv data error \n");
        return -1;
    }
    iRet = devmonitor_init_hotplug_sock();
    if(iRet < 0)
    {
        return ERROR;
    }
    pStPrivData->iFd = iRet;
    iRet = sys_pthread_create(NULL,"dev_monitor", TASK_PRIORITY_6, SIZE_64KB, (FUNCPTR)devmonitor_thread_work, 1, pStPrivData);
    if(iRet != OK)
    {
        LOGGER_ERROR("thread create strtask_name mcu_app_recv error \n");
    } 
    return OK;
}

/**@fn	       devmonitor_init_interface	  
 * @brief	   初始化对外接口
 * @param[in]  无
 * @return	   成功返回IDevMonitor操作指针  失败返回NULL;
 */
static INT devmonitor_init_interface(IDevMonitor *pIDevMonitor)
{   
    INT iRet = ERROR;
    if(NULL == pIDevMonitor)
    {
        LOGGER_ERROR("init interface func error\n");
        return iRet;
    }
    pIDevMonitor->Init = devmonitor_init;
    pIDevMonitor->RegisterActionCallBack = devmonitor_register_action_callback;
    pIDevMonitor->CheckDev = devmonitor_check_dev;
    return OK;
}

/**@fn	       devmonitor_init_instance	  
 * @brief	   初始化构造单例
 * @param[in]  无
 * @return	   成功返回对象指针,失败返回NULL
 */
IDevMonitor *devmonitor_init_instance(VOID)
{   
    INT iRet = ERROR;
    DEV_MONITOR_BASE_T *pBase = NULL;
    pBase = (DEV_MONITOR_BASE_T *)sys_mem_malloc(sizeof(DEV_MONITOR_BASE_T));
    if(!pBase)
    {
        LOGGER_ERROR("malloc errror \n");
        return NULL;
    }
    memset(pBase,0,sizeof(DEV_MONITOR_BASE_T));
    //初始化对外接口
    iRet = devmonitor_init_interface(&pBase->stInterface);
    if(iRet < 0)
    {   
        LOGGER_ERROR("init interface error \n");
        sys_mem_free(pBase);
        pBase = NULL;
        return NULL;
    }

    //初始化私有数据
    iRet = devmonitor_init_priv_data(&pBase->stPrivData);
    if(iRet < 0)
    {   
        LOGGER_ERROR("init priv data error \n");
        sys_mem_free(pBase);
        pBase = NULL;
        return NULL;
    }
    return &pBase->stInterface;
}

void init_devmonitor_component(void)
{
    devmonitor = devmonitor_init_instance();
    if(devmonitor == NULL)
    {
        LOGGER_ERROR("notification_init_instance error \n");
    }
}

IDevMonitor *devmonitor_get_instance(void)
{
    return devmonitor;
}