#include "Notification_priv.h"

INotification *notifybroker = NULL;

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


/**@fn	       notification_find_broker_node	  
 * @brief	   获取broker信息节点
 * @param[in]  pStPrivData     私有数据结构体指针
 * @param[in]  ID               id
 * @return	   成功返回节点数据指针 失败返回NULL
 */
BROKER_NODE_T *notification_find_broker_node(NOTIFICATION_PRIV_DATA_T *pStPrivData,const char* ID)
{
    BROKER_NODE_T *pStNode = NULL;
    if(!pStPrivData)
    {
        LOGGER_ERROR("input prt null\n");
        return NULL;
    }
    LIST_FOR_EACH(BROKER_NODE_T, pStNode, &pStPrivData->publishers)
    {
        if(strncmp(ID,pStNode->ID,strlen(pStNode->ID)) != 0)
        {
            continue;
        }
        break;
    }
    return pStNode;
}



/**@fn         notification_subscribe  
 * @brief      进行订阅
 * @param[in]  pINotification   pINotification对象操作指针
 * @return     成功返回OK     失败返回错误码
 */
static ERRORCODE_E notification_subscribe(INotification *pINotification,const char* pubid,const void* data,unsigned int size)
{
    NOTIFICATION_PRIV_DATA_T *pStPrivData = NULL;
    BROKER_NODE_T *pBrokernode = NULL;
    if(NULL == pINotification)
    {
        LOGGER_ERROR("invaild param error :%p \n",pINotification);
        return ERROR_UNKNOW;
    }
    pStPrivData = notification_get_priv_data(pINotification);
    if(NULL == pStPrivData) 
    {
        LOGGER_ERROR("get priv data error \n");
        return ERROR_UNKNOW;
    }
    //检查pubid
    /* Not allowed to subscribe to yourself */
    pBrokernode = notification_find_broker_node(pStPrivData,pubid);
    if(pBrokernode != NULL)
    {
         LOGGER_ERROR("Multi subscribe pub[%s]", pubid);
        return ERROR_UNKNOW;
     }
  
    //Let the publisher add this subscriber
    notifybroker_pushback(pStPrivData,pubid,(void *)data,size);
    return ERROR_NONE;
}

/**@fn         notification_unsubscribe  
 * @brief      初始化参数
 * @param[in]  pINotification   pINotification对象操作指针
 * @return     成功返回OK     失败返回错误码
 */
static ERRORCODE_E notification_unsubscribe(INotification *pINotification,const char* pubid)
{
    INT32 iRet = ERROR;
    NOTIFICATION_PRIV_DATA_T *pStPrivData = NULL;
    BROKER_NODE_T *pBrokernode = NULL;
    if(NULL == pINotification)
    {
        LOGGER_ERROR("invaild param error :%p \n",pINotification);
        return ERROR_UNKNOW;
    }
    pStPrivData = notification_get_priv_data(pINotification);
    if(NULL == pStPrivData) 
    {
        LOGGER_ERROR("get priv data error \n");
        return ERROR_UNKNOW;
    }
  /* Whether to subscribe to the publisher */
    pBrokernode = notification_find_broker_node(pStPrivData,pubid);
    if(pBrokernode == NULL)
    {
        LOGGER_ERROR("no subscribe pub[%s]", pubid);
        return ERROR_UNKNOW;
    }
 
    iRet = notifybroker_remove(&(pStPrivData->publishers),pBrokernode);
     if(iRet != OK)
     {
        LOGGER_ERROR("remove subscribe pub[%s] error\n", pubid);
         return ERROR_UNKNOW;
     }
    return ERROR_NONE;
}


/**@fn         notification_commit  
 * @brief      初始化参数
 * @param[in]  pINotification   pINotification对象操作指针
 * @return     成功返回OK     失败返回错误码
 */
static ERRORCODE_E notification_commit(INotification *pINotification, const char* pubid,const void* data,unsigned int size)
{
    NOTIFICATION_PRIV_DATA_T *pStPrivData = NULL;
    BROKER_NODE_T *pBrokernode = NULL;
    void* wBuf;
    if(NULL == pINotification || !size )
    {
        LOGGER_ERROR("invaild param error :%p \n",pINotification);
        return ERROR_UNKNOW;
    }
    pStPrivData = notification_get_priv_data(pINotification);
    if(NULL == pStPrivData) 
    {
        LOGGER_ERROR("get priv data error \n");
        return ERROR_UNKNOW;
    }
    pBrokernode = notification_find_broker_node(pStPrivData,pubid);
    if(pBrokernode == NULL)
    {
        LOGGER_ERROR("no subscribe pub[%s]", pubid);
        return ERROR_UNKNOW;
    }
    PingPongBuffer_GetWriteBuf(&pBrokernode->BufferManager, &wBuf);
    sys_mem_copy(wBuf, (void*)data, size);
    PingPongBuffer_SetWriteDone(&pBrokernode->BufferManager);
    LOGGER_INFO("pub[%s] commit data(0x%p)[%d] >> data(0x%p)[%d] done\n",
                pubid, data, size, wBuf, size);
    return ERROR_NONE;
}


/**@fn         notification_publish 
 * @brief      初始化参数
 * @param[in]  pINotification   pINotification对象操作指针
 * @return     成功返回OK     失败返回错误码
 */
static ERRORCODE_E notification_publish(INotification *pINotification, const char* pubid)
{
    NOTIFICATION_PRIV_DATA_T *pStPrivData = NULL;
    BROKER_NODE_T *pBrokernode = NULL;
    EventCallback_t callback = NULL;
    void* rBuf;
    EVENTPARAM_T param;
    ERRORCODE_E iRet = ERROR_UNKNOW;
    if(NULL == pINotification)
    {
        LOGGER_ERROR("invaild param error :%p \n",pINotification);
        return ERROR_UNKNOW;
    }
    pStPrivData = notification_get_priv_data(pINotification);
    if(NULL == pStPrivData) 
    {
        LOGGER_ERROR("get priv data error \n");
        return ERROR_UNKNOW;
    }
    pBrokernode = notification_find_broker_node(pStPrivData,pubid);
    if(pBrokernode == NULL)
    {
        LOGGER_ERROR("no subscribe pub[%s]", pubid);
        return ERROR_UNKNOW;
    }
  
    if (!PingPongBuffer_GetReadBuf(&pBrokernode->BufferManager, &rBuf))
    {
        LOGGER_WARN("pub[%s] data was not commit",pubid);
        return ERROR_NO_COMMITED;
    }

    param.event = EVENT_PUB_PUBLISH;
    param.pData = rBuf;
    param.size = pBrokernode->BufferSize;
    callback = pBrokernode->eventCallback;
    if(callback != NULL)
    {
        iRet = callback(&param);
    }else{
        LOGGER_WARN("pub[%s] not register callback",pubid);
        iRet = ERROR_NONE;
    }
    PingPongBuffer_SetReadDone(&pBrokernode->BufferManager);
    return iRet;
}


/**@fn         notification_pull  
 * @brief      初始化参数
 * @param[in]  pINotification   pINotification对象操作指针
 * @return     成功返回OK     失败返回错误码
 */
static ERRORCODE_E notification_pull(INotification *pINotification, const char* pubid,void* data,unsigned int size)
{
    NOTIFICATION_PRIV_DATA_T *pStPrivData = NULL;
    BROKER_NODE_T *pBrokernode = NULL;
    EventCallback_t callback = NULL;
    EVENTPARAM_T param;
    ERRORCODE_E iRet = ERROR_UNKNOW;
    if(NULL == pINotification)
    {
        LOGGER_ERROR("invaild param error :%p \n",pINotification);
        return ERROR_UNKNOW;
    }
    pStPrivData = notification_get_priv_data(pINotification);
    if(NULL == pStPrivData) 
    {
        LOGGER_ERROR("get priv data error \n");
        return ERROR_UNKNOW;
    }
    pBrokernode = notification_find_broker_node(pStPrivData,pubid);
    if(pBrokernode == NULL)
    {
        LOGGER_ERROR("no subscribe pub[%s]", pubid);
        return ERROR_UNKNOW;
    }
    callback = pBrokernode->eventCallback;
    if(callback != NULL)
    {
        param.event = EVENT_SUB_PULL;
        param.pData = data;
        param.size = size;
        iRet = callback(&param);
    }else{
        if(pBrokernode->BufferSize == size)
        {
            void* rBuf;
            if (PingPongBuffer_GetReadBuf(&pBrokernode->BufferManager, &rBuf))
            {
                memcpy(data, rBuf, size);
                PingPongBuffer_SetReadDone(&pBrokernode->BufferManager);
                LOGGER_INFO("read done");
                 iRet = ERROR_NONE;
            }
            else
            {
                LOGGER_WARN("pub[%s] data was not commit!", pubid);
            }
        }else{
            LOGGER_ERROR(
                "Data size pub[%s]:%d != sub[%s]:%d",
                pubid,
                pBrokernode->BufferSize,
                pBrokernode->ID,
                size
            );
        }
    }
    return iRet;
}



static ERRORCODE_E notification_notify(INotification *pINotification, const char* pubid,void* data,unsigned int size)
{
    NOTIFICATION_PRIV_DATA_T *pStPrivData = NULL;
    BROKER_NODE_T *pBrokernode = NULL;
    EventCallback_t callback = NULL;
    EVENTPARAM_T param;
    ERRORCODE_E iRet = ERROR_UNKNOW;
    if(NULL == pINotification)
    {
        LOGGER_ERROR("invaild param error :%p \n",pINotification);
        return ERROR_UNKNOW;
    }
    pStPrivData = notification_get_priv_data(pINotification);
    if(NULL == pStPrivData) 
    {
        LOGGER_ERROR("get priv data error \n");
        return ERROR_UNKNOW;
    }
    pBrokernode = notification_find_broker_node(pStPrivData,pubid);
    if(pBrokernode == NULL)
    {
        LOGGER_ERROR("no subscribe pub[%s]", pubid);
        return ERROR_UNKNOW;
    }
    callback = pBrokernode->eventCallback;
    if(callback != NULL)
    {
        param.event= EVENT_NOTIFY;
        param.pData = data;
        param.size = size;
        iRet = callback(&param);
    }else{
        LOGGER_WARN("pub[%s] not register callback",pubid);
        iRet = ERROR_NO_CALLBACK;
    }
    return iRet;
}

/**@fn         notification_seteventcallback   
 * @brief      初始化参数
 * @param[in]  pINotification   pINotification对象操作指针
 * @return     成功返回OK     失败返回错误码
 */
static ERRORCODE_E notification_seteventcallback(INotification *pINotification, const char* pubid,EventCallback_t callback)
{
    NOTIFICATION_PRIV_DATA_T *pStPrivData = NULL;
    BROKER_NODE_T *pBrokernode = NULL;
    if(NULL == pINotification)
    {
        LOGGER_ERROR("invaild param error :%p \n",pINotification);
        return ERROR_UNKNOW;
    }
    pStPrivData = notification_get_priv_data(pINotification);
    if(NULL == pStPrivData) 
    {
        LOGGER_ERROR("get priv data error \n");
        return ERROR_UNKNOW;
    }
    pBrokernode = notification_find_broker_node(pStPrivData,pubid);
    if(pBrokernode == NULL)
    {
        LOGGER_ERROR("no subscribe pub[%s]", pubid);
        return ERROR_UNKNOW;
    }
    pBrokernode->eventCallback = callback;

    return ERROR_NONE;
}

/**@fn         notification_init   
 * @brief      初始化参数
 * @param[in]  pINotification   pINotification对象操作指针
 * @return     成功返回OK     失败返回错误码
 */
static ERRORCODE_E notification_init(INotification *pINotification)
{
    NOTIFICATION_PRIV_DATA_T *pStPrivData = NULL;
    ERRORCODE_E iRet = ERROR_UNKNOW;
    pStPrivData = notification_get_priv_data(pINotification);
    if((NULL == pStPrivData))
    {
        LOGGER_ERROR("param error\n");
        return iRet;
    }
   // list_init(&pStPrivData->subscribers);
    list_init(&pStPrivData->publishers);
    sys_mutex_create(&pStPrivData->brokerMutex, MUTEX_NORMAL);
    return ERROR_NONE;
}

/**@fn         notification_release   
 * @brief      初始化参数
 * @param[in]  pINotification   pINotification对象操作指针
 * @return     成功返回OK     失败返回错误码
 */
static ERRORCODE_E notification_release(INotification *pINotification)
{
    NOTIFICATION_PRIV_DATA_T *pStPrivData = NULL;
    BROKER_NODE_T *pStNode = NULL;
    if(NULL == pINotification)
    {
        LOGGER_ERROR("invaild param error :%p \n",pINotification);
        return ERROR_UNKNOW;
    }
    pStPrivData = notification_get_priv_data(pINotification);
    if(NULL == pStPrivData) 
    {
        LOGGER_ERROR("get priv data error \n");
        return ERROR_UNKNOW;
    }
    LIST_FOR_EACH(BROKER_NODE_T, pStNode, &pStPrivData->publishers)
    {
       list_delete(&pStPrivData->publishers,&pStNode->node);
    }
    sys_mem_free(pINotification);
    return ERROR_NONE;
}

/**@fn	       notification_init_priv_data	  
 * @brief	   初始化私有数据
 * @param[in]  pStPrivData          私有数据指针
 * @return	   无
 */
static INT32 notification_priv_data(NOTIFICATION_PRIV_DATA_T *pStPrivData)
{
    if(!pStPrivData)
    {
        LOGGER_ERROR("init pPriv Data error\n");
        return ERROR;
    }
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
    pINotification->SetEventCallback = notification_seteventcallback;
    pINotification->Release = notification_release;

    return OK;
}




/**@fn	       notification_init_instance	  
 * @brief	   初始化构造单例
 * @param[in]  无
 * @return	   成功返回对象指针,失败返回NULL
 */
INotification *notification_init_instance()
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
    iRet = notification_priv_data(&pBase->stPrivData);
    if(iRet < 0)
    {   
        LOGGER_ERROR("init priv data error \n");
        sys_mem_free(pBase);
        pBase = NULL;
        return NULL;
    }
    return &pBase->stInterface;
}


void init_notification_component(void)
{
    notifybroker = notification_init_instance();
    if(notifybroker == NULL)
    {
        LOGGER_ERROR("notification_init_instance error \n");
    }
}

INotification *notification_get_notifybroker(void)
{
    return notifybroker;
}