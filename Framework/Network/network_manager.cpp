#include "network_priv.h"



/**@fn         gsensor_manager_init_priv_data    
 * @brief      初始化构造对外接口
 * @param[in]  GSENSOR_MANAGER_PRIV_DATA_T  AVBTalker对象接口
 * @return     成功返回OK  失败返回错误码
 */
static INT32 network_priv_data(NETWORK_PRIV_DATA_T *pStPrivData,CHAR* sDevName)
{
     return OK;
}


/**@fn         network_interface    
 * @brief      初始化构造对外接口
 * @param[in]  pINetwork_manager  Network对象接口
 * @return     成功返回OK  失败返回错误码
 */
static INT32 network_interface(INetwork *pINetwork_manager)
{
   
    return OK;
}

/**@fn	       network_init_instance	  
 * @brief	   初始化构造单例
 * @param[in]  iIndex uart序号
 * @return	   成功返回对象指针,失败返回NULL
 */
INetwork *network_init_instance(char* sDevname)
{   
    INT32 iRet = ERROR;
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