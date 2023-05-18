#include "notification_priv.h"


/**@fn         notification_get_priv_data
 * @brief      获取私有信息指针
 * @param[in]  pINotification 
 * @return    成功返回NOTIFICATION_PRIV_DATA_T类型指针,失败返回NULL
 */
static NOTIFICATION_PRIV_DATA_T *notification_get_priv_data(INotification *pINotification)
{
    NOTIFICATION_BASE_T *pStBase = NULL;
    pStBase = (NOTIFICATION_BASE_T*)pINotification;
    if(NULL == pStBase)
    {
        LOGGER_ERROR("param error\n");
        return NULL;
    }
    return &pStBase->stPrivData;
}


/**@fn         notification_subscribe  
 * @brief      初始化参数
 * @param[in]  pINotification   pINotification对象操作指针
 * @return     成功返回OK     失败返回错误码
 */
static INT32 notification_subscribe(INotification *pINotification,const char* pubid)
{
    // Whether to subscribe repeatedly
    
    // Whether the account is created

    //Add the publisher to the subscription list

    //Let the publisher add this subscriber



    return OK;
}

/**@fn         notification_subscribe  
 * @brief      初始化参数
 * @param[in]  pINotification   pINotification对象操作指针
 * @return     成功返回OK     失败返回错误码
 */
static INT32 notification_unsubscribe(INotification *pINotification,const char* pubid)
{

    return OK;
}


/**@fn         notification_commit  
 * @brief      初始化参数
 * @param[in]  pINotification   pINotification对象操作指针
 * @return     成功返回OK     失败返回错误码
 */
static INT32 notification_commit(INotification *pINotificaion, const void* data,unsigned int size)
{

    return OK;
}





/**@fn         notification_init   
 * @brief      初始化参数
 * @param[in]  pINotification   pINotification对象操作指针
 * @return     成功返回OK     失败返回错误码
 */
static int notification_init(INotification *pINotification)
{
    NOTIFICATION_PRIV_DATA_T *pStPrivData = NULL;
    INT32 iRet = 0;
    pStPrivData = notification_get_priv_data(pINotification);
    if((NULL == pStPrivData))
    {
        LOGGER_ERROR("param error\n");
        return iRet;
    }
    if(pStPrivData->BufferSize != 0)
    {
        UINT8* buf0 = NULL;
        UINT8* buf1 = NULL;
        UINT8 *buffer = (UINT8*) sys_mem_malloc((pStPrivData->BufferSize) * sizeof(UINT8) * 2);
        if (!buffer)
        {
            LOGGER_ERROR("buffer malloc failed!");
            return ERROR;
        }
        memset(buffer, 0, (pStPrivData->BufferSize) * sizeof(UINT8) * 2);
        buf0 = buffer;
        buf1 = buffer + (pStPrivData->BufferSize);
        PingPongBuffer_Init(&pStPrivData->BufferManager, buf0, buf1);
    }

}


/**@fn	       notification_init_priv_data	  
 * @brief	   初始化私有数据
 * @param[in]  pStPrivData          私有数据指针
 * @return	   无
 */
static INT32 notification_priv_data(NOTIFICATION_PRIV_DATA_T *pStPrivData,UINT32 pingpongbuffsize)
{
    if(!pStPrivData)
    {
        LOGGER_ERROR("init pPriv Data error\n");
        return ERROR;
    }
    pStPrivData->BufferSize=pingpongbuffsize;

    return OK;
}


/**@fn	       notification_init_interface	  
 * @brief	   初始化对外接口
 * @param[in]  无
 * @return	   成功返回IDevMonitor操作指针  失败返回NULL;
 */
static INT32 notification_interface(INotification *pINotification)
{   
    INT32 iRet = ERROR;
    if(NULL == pINotification)
    {
        LOGGER_ERROR("init interface func error\n");
        return iRet;
    }
    pINotification->Init = notification_init;
    pINotification->Subscribe = notification_subscribe;
    pINotification->Unsubscribe = notification_unsubscribe;
    pINotification->Commit = notification_commit;
    return OK;
}




/**@fn	       notification_init_instance	  
 * @brief	   初始化构造单例
 * @param[in]  无
 * @return	   成功返回对象指针,失败返回NULL
 */
INotification *notification_init_instance(unsigned int pingpongbuffsize)
{   
    INT32 iRet = ERROR;
    NOTIFICATION_BASE_T *pBase = NULL;
    pBase = (NOTIFICATION_BASE_T *)sys_mem_malloc(sizeof(NOTIFICATION_BASE_T));
    if(!pBase)
    {
        LOGGER_ERROR("malloc errror \n");
        return NULL;
    }
    memset(pBase,0,sizeof(NOTIFICATION_BASE_T));
    //初始化对外接口
    iRet = notification_interface(&pBase->stInterface);
    if(iRet < 0)
    {   
        LOGGER_ERROR("init interface error \n");
        sys_mem_free(pBase);
        pBase = NULL;
        return NULL;
    }

    //初始化私有数据
    iRet = notification_priv_data(&pBase->stPrivData,pingpongbuffsize);
    if(iRet < 0)
    {   
        LOGGER_ERROR("init priv data error \n");
        sys_mem_free(pBase);
        pBase = NULL;
        return NULL;
    }
    return &pBase->stInterface;
}