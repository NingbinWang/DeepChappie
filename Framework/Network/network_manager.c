#include "network_priv.h"

/**@fn         network_manage_get_priv_data
 * @brief      获取私有信息指针
 * @param[in]  pINetwork_manager 
 * @return    成功返回GSENSOR_MANAGER_PRIV_DATA_T类型指针,失败返回NULL
 */
static NETWORK_PRIV_DATA_T *network_manage_get_priv_data(INetwork *pINetwork_manager)
{
    NETWORK_BASE_T *pStBase = NULL;
    pStBase = (NETWORK_BASE_T*)pINetwork_manager;
    if(NULL == pStBase)
    {
        LOGGER_ERROR("param error\n");
        return NULL;
    }
    return &pStBase->stPrivData;
}


/**@fn         network_manager_init    
 * @brief      初始化构造对外接口
 * @param[in]  INetwork network的接口
 * @return     成功返回OK  失败返回错误码
 */
static INT network_manager_init(INetwork *pINetwork_manager,const CHAR *strIP,const CHAR *strNetMask, const CHAR *strGateWay)
{
    NETWORK_PRIV_DATA_T *pStPrivData = NULL;
    INT iRet = 0;
    pStPrivData = network_manage_get_priv_data(pINetwork_manager);
    if((NULL == pStPrivData))
    {
        LOGGER_ERROR("param error\n");
        return iRet;
    }
    iRet = sys_net_ifconfig(pStPrivData->sDevName,strIP,strNetMask,strGateWay);
    if(iRet == ERROR)
    {
        LOGGER_ERROR("ifconfig %s error ip:%s netmask:%s gateway:%s\n",pStPrivData->sDevName,strIP,strNetMask,strGateWay);
        return ERROR;
    }
    return OK;
}

/**@fn         network_manager_up    
 * @brief      初始化构造对外接口
 * @param[in]   INetwork network的接口
 * @return     成功返回OK  失败返回错误码
 */
static INT network_manager_up(INetwork *pINetwork_manager)
{
    NETWORK_PRIV_DATA_T *pStPrivData = NULL;
    INT iRet = 0;
    pStPrivData = network_manage_get_priv_data(pINetwork_manager);
    if((NULL == pStPrivData))
    {
        LOGGER_ERROR("param error\n");
        return iRet;
    }
    iRet = sys_net_ifconfig_up(pStPrivData->sDevName);
    if(iRet == ERROR)
    {
        LOGGER_ERROR("ifconfig  %s up error \n",pStPrivData->sDevName);
        return ERROR;
    }
    return OK;
}
/**@fn         network_manager_down
 * @param[in]   INetwork network的接口
 * @return     成功返回OK  失败返回错误码
 */
static INT network_manager_down(INetwork *pINetwork_manager)
{
    NETWORK_PRIV_DATA_T *pStPrivData = NULL;
    INT iRet = ERROR;
    pStPrivData = network_manage_get_priv_data(pINetwork_manager);
    if((NULL == pStPrivData))
    {
        LOGGER_ERROR("param error\n");
        return iRet;
    }
    iRet = sys_net_ifconfig_down(pStPrivData->sDevName);
    if(iRet == ERROR)
    {
        LOGGER_ERROR("ifconfig %s down error\n",pStPrivData->sDevName);
        return ERROR;
    }
    return OK;
}



/**@fn         network_manager_release   
 * @brief      释放数据
 * @param[in]  INetwork network的接口
 * @return     成功返回OK     失败返回错误码
 */
static INT network_manager_release(INetwork *pINetwork_manager)
{
    NETWORK_BASE_T *pStBase = NULL;
    pStBase = (NETWORK_BASE_T*)pINetwork_manager;
    sys_mem_free(pStBase);
    return OK;
}


/**@fn         network_priv_data    
 * @brief      初始化构造私有数据
 * @param[in]  NETWORK_PRIV_DATA_T    network的私有结构体
 * @param[in]  sDevName               网络名称
 * @return     成功返回OK  失败返回错误码
 */
static INT network_priv_data(NETWORK_PRIV_DATA_T *pStPrivData,CHAR* sDevName)
{
     INT iRet = 0;
     sys_mem_copy(pStPrivData->sDevName,sDevName,strlen(sDevName));
     iRet = sys_net_dev_node_check(pStPrivData->sDevName);
     if(iRet == ERROR)
    {
           LOGGER_ERROR("can't find devname %s\n",pStPrivData->sDevName);
           return ERROR;
    }
    
    return OK;
}


/**@fn         network_interface    
 * @brief      初始化构造对外接口
 * @param[in]  pINetwork_manager  Network对象接口
 * @return     成功返回OK  失败返回错误码
 */
static INT network_interface(INetwork *pINetwork_manager)
{
    if(NULL == pINetwork_manager)
    {
        printf("init interface func error\n");
        return ERROR;
    }
    pINetwork_manager->Init = network_manager_init;
    pINetwork_manager->Up = network_manager_up;
    pINetwork_manager->Down = network_manager_down;
    pINetwork_manager->Release = network_manager_release;
    return OK;
}

/**@fn	       network_init_instance	  
 * @brief	   初始化构造单例
 * @param[in]  iIndex uart序号
 * @return	   成功返回对象指针,失败返回NULL
 */
INetwork *network_init_instance(char* sDevname)
{   
    INT iRet = ERROR;
    NETWORK_BASE_T *pBase = NULL;
    pBase =(NETWORK_BASE_T *) sys_mem_malloc(sizeof(NETWORK_BASE_T));
    if(!pBase)
    {
        LOGGER_ERROR("malloc errror \n");
        return NULL;
    }
    memset(pBase,0,sizeof(NETWORK_BASE_T));
    //初始化对外接口
    iRet = network_interface(&pBase->stInterface);
    if(iRet < 0)
    {   
        LOGGER_ERROR("init interface error \n");
        sys_mem_free(pBase);
        pBase = NULL;
        return NULL;
    }

    //初始化私有数据
    iRet = network_priv_data(&pBase->stPrivData,sDevname);
    if(iRet < 0)
    {   
        LOGGER_ERROR("init priv data error \n");
        sys_mem_free(pBase);
        pBase = NULL;
        return NULL;
    }
    return &pBase->stInterface;
}

void init_network_component(char* sDevname)
{
    network = network_init_instance(sDevname);
    if(network== NULL)
    {
        LOGGER_ERROR("init_network_component error \n");
    }
}

INetwork *network_get_instance(void)
{
    return network;
}