#include "Notification_priv.h"


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
 * @brief      进行订阅
 * @param[in]  pINotification   pINotification对象操作指针
 * @return     成功返回OK     失败返回错误码
 */
static INT32 notification_subscribe(INotification *pINotification,const char* pubid)
{
    NOTIFICATION_PRIV_DATA_T *pStPrivData = NULL;
    Broker_Node *pub = NULL;
    if(NULL == pINotification)
    {
        LOGGER_ERROR("invaild param error :%p \n",pINotification);
        return ERROR;
    }
    pStPrivData = notification_get_priv_data(pINotification);
    if(NULL == pStPrivData) 
    {
        LOGGER_ERROR("get priv data error \n");
        return ERROR;
    }
    //检查pubid
    /* Not allowed to subscribe to yourself */
    if (strcmp(pubid,pStPrivData->publishers->name) == 0)
    {
        LOGGER_ERROR("try to subscribe to it itself name %s", pubid);
        return ERROR;
    }
    // Whether the account is created
     pub=notifybroker_find(pStPrivData->publishers,pubid);
     if(pub != NULL){
        LOGGER_ERROR("Multi subscribe pub[%s]", pubid);
        return ERROR;
     }
    //Let the publisher add this subscriber
    notifybroker_pushback(pStPrivData->publishers,pubid);
    //Let the publisher add this subscriber


    return OK;
}

/**@fn         notification_unsubscribe  
 * @brief      初始化参数
 * @param[in]  pINotification   pINotification对象操作指针
 * @return     成功返回OK     失败返回错误码
 */
static INT32 notification_unsubscribe(INotification *pINotification,const char* pubid)
{
    INT32 iRet = ERROR;
    NOTIFICATION_PRIV_DATA_T *pStPrivData = NULL;
    Broker_Node *pub = NULL;
    if(NULL == pINotification)
    {
        LOGGER_ERROR("invaild param error :%p \n",pINotification);
        return ERROR;
    }
    pStPrivData = notification_get_priv_data(pINotification);
    if(NULL == pStPrivData) 
    {
        LOGGER_ERROR("get priv data error \n");
        return ERROR;
    }
  /* Whether to subscribe to the publisher */
    pub=notifybroker_find(pStPrivData->publishers,pubid);
     if(pub == NULL){
        LOGGER_ERROR("no subscribe pub[%s]\n", pubid);
        return ERROR;
     }

     iRet = notifybroker_remove(pStPrivData->publishers,pubid);
     if(iRet != OK)
     {
        LOGGER_ERROR("remove subscribe pub[%s] error\n", pubid);
         return ERROR;
     }

    return OK;
}


/**@fn         notification_commit  
 * @brief      初始化参数
 * @param[in]  pINotification   pINotification对象操作指针
 * @return     成功返回OK     失败返回错误码
 */
static INT32 notification_commit(INotification *pINotification, const char* pubid,const void* data,unsigned int size)
{
    NOTIFICATION_PRIV_DATA_T *pStPrivData = NULL;
    Broker_Node *pub = NULL;
    void* wBuf;
    if(NULL == pINotification || !size )
    {
        LOGGER_ERROR("invaild param error :%p \n",pINotification);
        return ERROR;
    }
    pStPrivData = notification_get_priv_data(pINotification);
    if(NULL == pStPrivData) 
    {
        LOGGER_ERROR("get priv data error \n");
        return ERROR;
    }
    if(pStPrivData->publishers == NULL)
    {
        LOGGER_ERROR("no publiisher \n");
        return ERROR;
    }
    pub=notifybroker_find(pStPrivData->publishers,pubid);
     if(pub == NULL){
        LOGGER_ERROR("no subscribe pub[%s]\n", pubid);
        return ERROR;
     }
  
    PingPongBuffer_GetWriteBuf(&pub->BufferManager, &wBuf);
    sys_mem_copy(wBuf, (void*)data, size);
    PingPongBuffer_SetWriteDone(&pub->BufferManager);
    LOGGER_INFO("pub[%s] commit data(0x%p)[%d] >> data(0x%p)[%d] done\n",
                pubid, data, size, wBuf, size);
    return OK;
}


/**@fn         notification_publish 
 * @brief      初始化参数
 * @param[in]  pINotification   pINotification对象操作指针
 * @return     成功返回OK     失败返回错误码
 */
static INT32 notification_publish(INotification *pINotification, const char* pubid)
{
    NOTIFICATION_PRIV_DATA_T *pStPrivData = NULL;
    void* rBuf;
    EventParam_t param;
    Broker_Node *pub=NULL;
    if(NULL == pINotification)
    {
        LOGGER_ERROR("invaild param error :%p \n",pINotification);
        return ERROR;
    }
    pStPrivData = notification_get_priv_data(pINotification);
    if(NULL == pStPrivData) 
    {
        LOGGER_ERROR("get priv data error \n");
        return ERROR;
    }
    pub=notifybroker_find(pStPrivData->publishers,pubid);
     if(pub == NULL){
        LOGGER_ERROR("no subscribe pub[%s]\n", pubid);
        return ERROR;
     }
  
    
    if (!PingPongBuffer_GetReadBuf(&pub->BufferManager, &rBuf))
    {
        LOGGER_WARN("pub[%s] data was not commit",pStPrivData->publishers->name);
        return ERROR_NO_COMMITED;
    }
    param.event = EVENT_PUB_PUBLISH;
    param.pData = rBuf;
    param.size = pStPrivData->publishers->BufferSize;


    PingPongBuffer_SetReadDone(&pub->BufferManager);
    return OK;
}


/**@fn         notification_pull  
 * @brief      初始化参数
 * @param[in]  pINotification   pINotification对象操作指针
 * @return     成功返回OK     失败返回错误码
 */
static INT32 notification_pull(INotification *pINotificaion, const char* pubid,void* data,unsigned int size)
{
    return OK;
}



static INT32 notification_notify(INotification *pINotificaion, const char* pubid,void* data,unsigned int size)
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
    pStPrivData->subscribers =notifybroker_init("SUB",pStPrivData->BufferSize);
    if(pStPrivData->subscribers == NULL)
    {
      LOGGER_ERROR("init subscribers error\n");
      return ERROR;
    }
    pStPrivData->publishers =notifybroker_init("PUB",pStPrivData->BufferSize);
    if(pStPrivData->publishers == NULL)
    {
      LOGGER_ERROR("init subscribers error\n");
      return ERROR;
    }
    return OK;
}


/**@fn	       notification_init_priv_data	  
 * @brief	   初始化私有数据
 * @param[in]  pStPrivData          私有数据指针
 * @return	   无
 */
static INT32 notification_priv_data(NOTIFICATION_PRIV_DATA_T *pStPrivData,UINT32 pingpongbuffsize)
{
    if((!pStPrivData) && (pingpongbuffsize == 0))
    {
        LOGGER_ERROR("init pPriv Data error\n");
        return ERROR;
    }
    pStPrivData->BufferSize= pingpongbuffsize;
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
    pINotification->Pull = notification_pull;
    pINotification->Publish = notification_publish;
    pINotification->Notify = notification_notify;
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