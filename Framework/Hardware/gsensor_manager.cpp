#include "gsensor_priv.h"

/**@fn         gsensor_manager_init_resource_release   
 * @brief      释放单例初始化过程中申请的资源
 * @param[in]  pStPrivData   私有数据指针
 * @return     VOID
 */
static VOID gsensor_manager_init_resource_release(GSENSOR_MANAGER_PRIV_DATA_T *pStPrivData)
{
    INT32 iRet = ERROR;
    if(NULL == pStPrivData)
    {
        printf("param error\n");
        return;
    }

    if(0 != pStPrivData->stMsgID)
    {
        iRet = sys_mqueue_close(&pStPrivData->stMsgID);
        if(OK == iRet)
        {
            pStPrivData->stMsgID = 0;
        }
    }
    if(0 != pStPrivData->stThreadID)
    {
        iRet = sys_pthread_cancel(pStPrivData->stThreadID);
        if(OK == iRet)
        {
            pStPrivData->stThreadID = 0;
        }
    }
    return;
}


/**@fn         gsensor_manager_send_ctrl_msg        
 * @brief      发送消息          
 * @param[in]  pStPrivData私有数据结构体指针
 * @param[in]  eStates    RTSP会话状态类型
 * @return     成功返回OK  失败返回错误码
 */
static INT32 gsensor_manager_send_ctrl_msg(GSENSOR_MANAGER_PRIV_DATA_T *pStPrivData, GSENSOR_MANAGER_CMD_E eCmd)
{
    INT32 iRet = ERROR;
    UINT32 uWaitMsec = 2000;
    GSENSOR_MANAGER_MSG_T stCtrlMsg;

    if(NULL == pStPrivData)
    {
        printf("param error\n");
        return iRet;
    }
    stCtrlMsg.eCmd = eCmd;
    iRet = sys_mqueue_send(&pStPrivData->stMsgID, (CHAR *)&stCtrlMsg, sizeof(GSENSOR_MANAGER_MSG_T), NO_WAIT);
    if(iRet < 0)
    {
        printf("sys_mqueue_send start msg error \n");
        return iRet;
    }
    iRet = sys_mqueue_recv(&pStPrivData->stMsgID, (CHAR *)&stCtrlMsg, sizeof(GSENSOR_MANAGER_MSG_T),uWaitMsec);
    if(iRet < 0)
    {
        printf("eCmd %d recv result time out\n", eCmd);
        return iRet;
    }
    iRet = (stCtrlMsg.eResult == GSENSOR_MANAGER_RESULT_SUCCESS) ? OK : ERROR;
    if(iRet < 0)
    {
        printf("control cmd %d error\n", eCmd);
        return iRet;
    }
    return iRet;
}

/**@fn	       gsensor_manager_send_msg_response	  
 * @brief	   发送消息响应
 * @param[in]  pStSegMsg   消息指针
 * @param[in]  pStPrivData 私有数据指针
 * @return	   无
 */
static VOID gsensor_manager_send_msg_response(GSENSOR_MANAGER_PRIV_DATA_T *pStPrivData, GSENSOR_MANAGER_MSG_T *pStMsg, GSENSOR_MANAGER_RESULT_E eResult)
{
    INT32 iRet = ERROR;
    GSENSOR_MANAGER_MSG_T stSegMsg ;

    if((NULL == pStPrivData) || (NULL == pStMsg))
    {
        printf("param error\n");
        return;
    }
    stSegMsg.eCmd    = pStMsg->eCmd;
    stSegMsg.eResult = eResult;
    iRet = sys_mqueue_send(&pStPrivData->stMsgID, (CHAR *)&stSegMsg, sizeof(GSENSOR_MANAGER_MSG_T), NO_WAIT);
    if(iRet < 0)
    {
        printf("sys_mqueue_send start msg error \n");
        return;
    }
    return;
}

/**@fn	       gsensor_manager_recv_msg_handle	  
 * @brief	   消息处理接口
 * @param[in]  pStPrivData 私有数据指针
 * @param[in]  pStMsg   消息指针
 * @return	  成功返回OK 其它返回ERROR
 */
static INT32 gsensor_manager_recv_msg_handle(GSENSOR_MANAGER_PRIV_DATA_T *pStPrivData, GSENSOR_MANAGER_MSG_T *pStMsg)
{
    INT32 iRet = ERROR;
    if((NULL == pStPrivData) || (NULL == pStMsg))
    {
        printf("param error\n");
        return iRet;
    }
    switch(pStMsg->eCmd)
    {
        case GSENSOR_MANAGER_CMD_START:
            if(GSENSOR_MANAGER_CMD_START == pStPrivData->eState)
            {
                printf("gsensor already start\n");
                iRet = OK;
                break;
            }
            pStPrivData->eState = GSENSOR_MANAGER_CMD_START;
            iRet = OK;
            break;
        case GSENSOR_MANAGER_CMD_STOP:
            if(GSENSOR_MANAGER_CMD_STOP != pStPrivData->eState)
            {
                printf("avb talker not in start state\n");
                break;
            }
            pStPrivData->iWaitTime = WAIT_FOREVER;
            pStPrivData->eState = GSENSOR_MANAGER_CMD_STOP;
            pStPrivData->iFlag = 0;
            iRet = OK;
            break;
        default:
            printf("ctrl cmd %d not support\n", pStMsg->eCmd);
            iRet = ERROR;
            break;
    }
    
    //发送本次操作的结果 成功返回OK  失败返回ERROR
    if(iRet < 0)
    {   
        gsensor_manager_send_msg_response(pStPrivData,pStMsg,GSENSOR_MANAGER_RESULT_FAILED);
        return iRet;
    }
    gsensor_manager_send_msg_response(pStPrivData,pStMsg,GSENSOR_MANAGER_RESULT_SUCCESS);
    return iRet;
}

static INT32 gsensor_manager_maxval(const UINT16 validnum)
{
    return ((1<<(validnum- 1))-1);
}

static INT32 gsensor_manager_convert(const INT32 maxval,INT32 data)
{
    if(data < maxval){
        return data;
    }else{
        return -(data-maxval);
    }
}

static VOID gsensor_manager_calculate(GSENSOR_MANAGER_PRIV_DATA_T *pStPrivData,const sensor_t stRawData)
{
    INT32 accmaxval = 0;
    INT32 gyromaxval = 0;
    if(NULL == pStPrivData)
    {
        printf("param error\n");
        return;
    }
    accmaxval = gsensor_manager_maxval(pStPrivData->info.accvalidnum);
    pStPrivData->data.accx = gsensor_manager_convert(accmaxval,stRawData.accx)*(pStPrivData->info.acccoef);
    pStPrivData->data.accy = gsensor_manager_convert(accmaxval,stRawData.accy)*(pStPrivData->info.acccoef);
    pStPrivData->data.accz = gsensor_manager_convert(accmaxval,stRawData.accz)*(pStPrivData->info.acccoef);
    gyromaxval = gsensor_manager_maxval(pStPrivData->info.gyrovalidnum);
    pStPrivData->data.gyrox = gsensor_manager_convert(gyromaxval,stRawData.gyrox)*(pStPrivData->info.gyrocoef);
    pStPrivData->data.gyroy = gsensor_manager_convert(gyromaxval,stRawData.gyroy)*(pStPrivData->info.gyrocoef);
    pStPrivData->data.gyroz = gsensor_manager_convert(gyromaxval,stRawData.gyroz)*(pStPrivData->info.gyrocoef);

}
/**@fn          gsensor_manager_work_thread    
 * @brief       gsensor_manager工作线程
 * @param[in]   pStPrivData  私有数据结构指针
 * @return      VOID
 */
static VOID gsensor_manager_work_thread(GSENSOR_MANAGER_PRIV_DATA_T *pStPrivData)
{
    sensor_t stData;
    INT32 iFd = 0;
    INT32 iRet = ERROR;
    GSENSOR_MANAGER_MSG_T  stCtrlMsg;
    fd_set read_fd;
    if(NULL == pStPrivData)
    {
        printf("param error\n");
        return;
    }
    iFd = pStPrivData->iFd;
    pStPrivData->iFlag = 0;
    FD_ZERO(&read_fd);
    while(TRUE)
    {
        if(0 == pStPrivData->iFlag%20)
        {
            iRet = sys_mqueue_recv(&pStPrivData->stMsgID, (CHAR *)&stCtrlMsg, sizeof(stCtrlMsg), pStPrivData->iWaitTime);
            if (iRet >= 0)
            {   
                
                iRet = gsensor_manager_recv_msg_handle(pStPrivData,&stCtrlMsg);
                if((iRet < 0) || (GSENSOR_MANAGER_CMD_STOP == pStPrivData->eState))
                {
                    pStPrivData->iWaitTime = WAIT_FOREVER;
                    continue;
                }
            }
        }
        FD_SET(iFd, &read_fd);
		select(iFd + 1, &read_fd, NULL, NULL, NULL);
	    if(FD_ISSET(iFd, &read_fd))
		{
				sys_gsensor_getdata(iFd,&stData);
				gsensor_manager_calculate(pStPrivData,stData);		
		}
    }
}



/**@fn         gsensor_manager_get_priv_data
 * @brief      获取私有信息指针
 * @param[in]  pIGsensor_manager  IGsensor_manager
 * @return    成功返回GSENSOR_MANAGER_PRIV_DATA_T类型指针,失败返回NULL
 */
static GSENSOR_MANAGER_PRIV_DATA_T *gsensor_manager_get_priv_data(IGsensor_manager *pIGsensor_manager)
{
    GSENSOR_MANAGER_BASE_T *pStBase = NULL;
    pStBase = (GSENSOR_MANAGER_BASE_T*)pIGsensor_manager;
    if(NULL == pStBase)
    {
        printf("param error\n");
        return NULL;
    }
    return &pStBase->stPrivData;
}

/**@fn         gsensor_manager_init   
 * @brief      初始化参数
 * @param[in]  pIGsensor_manager   IGsensor_manager对象操作指针
 * @return     成功返回OK     失败返回错误码
 */
static INT32 gsensor_manager_init(IGsensor_manager *pIGsensor_manager)
{
    INT32 iRet = ERROR;
    CHAR strMsgName[64] = {0};
    CHAR strThreadName[64] = {0}; 
    GSENSOR_MANAGER_PRIV_DATA_T *pStPrivData = NULL;
    pStPrivData = gsensor_manager_get_priv_data(pIGsensor_manager);
    if((NULL == pStPrivData))
    {
        printf("param error\n");
        return iRet;
    }
    do
    {
        iRet = snprintf(strMsgName,sizeof(strMsgName),"Gsensor_%s", pStPrivData->cGyroname);
        if(iRet < 0)
        {
            printf("snprintf error\n");
            break;
        }
        iRet = sys_mqueue_create(&pStPrivData->stMsgID, strMsgName, 8, sizeof(GSENSOR_MANAGER_MSG_T));
        if(iRet < 0)
        {
            printf("sys_mqueue_create fail\n");
            break;
        }
        iRet = snprintf(strThreadName,sizeof(strThreadName),"Gsensor_%dAxis",pStPrivData->uAxis);
        if(iRet < 0)
        {
            printf("snprintf error!\n");
            break;
        }
        iRet = sys_pthread_create(&pStPrivData->stThreadID, strThreadName, TASK_PRIORITY_5, SIZE_32KB, (FUNCPTR)gsensor_manager_work_thread, 1, pStPrivData);
        if(iRet != OK)
        {
            printf("create gsensor task failed!\n");
            break;
        }
    } while (FALSE);

    if(iRet < 0)
    {
        printf("gsensor_manager_init failed!\n");
        gsensor_manager_init_resource_release(pStPrivData);
        return iRet;
    }
    

    return OK;
}

/**@fn         gsensor_manager_start      
 * @brief      开启服务
 * @param[in]  IGsensor_manager   IGsensor_manager对象操作指针
 * @return     成功返回OK     失败返回错误码
 */
static INT32 gsensor_manager_start(IGsensor_manager *pIGsensor_manager)
{
    INT32 iRet = ERROR;
    GSENSOR_MANAGER_PRIV_DATA_T *pStPrivData = NULL;

    pStPrivData = gsensor_manager_get_priv_data(pIGsensor_manager);
    if(NULL == pStPrivData)
    {
        printf("param error\n");
        return iRet;
    }
    
    return gsensor_manager_send_ctrl_msg(pStPrivData, GSENSOR_MANAGER_CMD_START);
}

/**@fn         gsensor_manager_stop   
 * @brief      关闭服务
 * @param[in]  IGsensor_manager   IGsensor_manager对象操作指针
 * @return     成功返回OK     失败返回错误码
 */
static INT32 gsensor_manager_stop(IGsensor_manager *pIGsensor_manager)
{
     INT32 iRet = ERROR;
    GSENSOR_MANAGER_PRIV_DATA_T *pStPrivData = NULL;

    pStPrivData = gsensor_manager_get_priv_data(pIGsensor_manager);
    if(NULL == pStPrivData)
    {
        printf("param error\n");
        return iRet;
    }
    if(GSENSOR_MANAGER_CMD_START != pStPrivData->eState)
    {
        printf("avb talker not in start state\n");
        return iRet;
    }
    return gsensor_manager_send_ctrl_msg(pStPrivData, GSENSOR_MANAGER_CMD_STOP);
}

/**@fn         gsensor_manager_get_state   
 * @brief      获取指定通道推流状态
 * @param[in]  IGsensor_manager  对象操作指针
 * @param[in]  pState 待获取推流状态
 * @return     成功返回OK     失败返回错误码
 */
static INT32 gsensor_manager_get_state(IGsensor_manager *pIGsensor_manager, GSENSOR_MANAGER_STATE_E *pState)
{
    INT32 iRet = ERROR;
    GSENSOR_MANAGER_PRIV_DATA_T *pStPrivData = NULL;

    pStPrivData = gsensor_manager_get_priv_data(pIGsensor_manager);
    if((NULL == pStPrivData) || (NULL == pState))
    {
        printf("param error\n");
        return iRet;
    }
    switch(pStPrivData->eState)
    {
        case GSENSOR_MANAGER_CMD_START:
            *pState = GSENSOR_MANAGER_START;
            break;
        case GSENSOR_MANAGER_CMD_STOP:
            *pState = GSENSOR_MANAGER_STOP;
            break;
        default:
            *pState = GSENSOR_MANAGER_INIT;
            break;
    }
    return OK;
}


/**@fn         gsensor_manager_release   
 * @brief      初始化参数
 * @param[in]  pIGsensor_manager   IGsensor_manager对象操作指针
 * @return     成功返回OK     失败返回错误码
 */
static INT32 gsensor_manager_release(IGsensor_manager *pIGsensor_manager)
{
     INT32 iRet = ERROR;
     GSENSOR_MANAGER_PRIV_DATA_T *pStPrivData = NULL;
     pStPrivData = gsensor_manager_get_priv_data(pIGsensor_manager);
    if((NULL == pStPrivData))
    {
        printf("param error\n");
        return iRet;
    }
    gsensor_manager_init_resource_release(pStPrivData);

    iRet = sys_gsensor_close(pStPrivData->iFd);
    if(iRet < 0)
    {
        printf("sys_gsensor_close error\n");
        return iRet;
    }
    sys_mem_free(pIGsensor_manager);
    return OK;
}

/**@fn         gsensor_manager_interface    
 * @brief      初始化构造对外接口
 * @param[in]  pIAVBTalker  AVBTalker对象接口
 * @return     成功返回OK  失败返回错误码
 */
static INT32 gsensor_manager_interface(IGsensor_manager *pIGsensor_manager)
{
    if(NULL == pIGsensor_manager)
    {
        printf("param error\n");
        return ERROR;
    }
    pIGsensor_manager->Init = gsensor_manager_init;
    pIGsensor_manager->Start = gsensor_manager_start;
    pIGsensor_manager->Stop = gsensor_manager_stop;
    pIGsensor_manager->Release = gsensor_manager_release;
    pIGsensor_manager->GetState = gsensor_manager_get_state;
    return OK;
}
/**@fn         gsensor_manager_init_priv_data    
 * @brief      初始化构造对外接口
 * @param[in]  GSENSOR_MANAGER_PRIV_DATA_T  AVBTalker对象接口
 * @return     成功返回OK  失败返回错误码
 */
static INT32 gsensor_manager_init_priv_data(GSENSOR_MANAGER_PRIV_DATA_T *pStPrivData)
{
    sensorinfo_t *pinfo;
    INT32 iRet = ERROR;
     INT32 iFd = 0;
    pinfo = (sensorinfo_t *)sys_mem_malloc(sizeof(sensorinfo_t));
    if(NULL == pinfo)
    {
        printf("sys_mem_malloc failed\n");
        return ERROR;
    }
    pStPrivData->iFd = sys_gsensor_open();
    iFd = pStPrivData->iFd;
    if(iFd < 0)
    {
        printf("id < 0\n");
        sys_mem_free(pinfo);
        return ERROR;
    }
    iRet = sys_gsensor_getinfo(iFd,pinfo);
    if(iRet == ERROR)
    {
        printf("sys_gsensor_getinfo failed\n");
        sys_mem_free(pinfo);
        return ERROR;
    }
    pStPrivData->info.uAxis = pinfo->AXIS;
   
    sys_mem_copy(pStPrivData->cGyroname,pinfo->gyroname,strlen(pinfo->gyroname));
    iRet = sys_gsensor_getconfig(iFd,pinfo);
    if(iRet == ERROR)
    {
        printf("sys_gsensor_getinfo failed\n");
        sys_mem_free(pinfo);
        return ERROR;
    }
    pStPrivData->info.uAccrng = pinfo->accrng;
    pStPrivData->info.uGyrorng = pinfo->gyrorng;
    pStPrivData->eMode = NORMALE_MODE;
    iRet = sys_gsensor_normalpower(iFd);
    if(iRet == ERROR)
    {
        printf("sys_gsensor_normalpower failed\n");
        sys_mem_free(pinfo);
        return ERROR;
    }
    iRet = sys_gsensor_getparam(iFd,pinfo);
    if(iRet == ERROR)
    {
        printf("sys_gsensor_getparam failed\n");
        sys_mem_free(pinfo);
        return ERROR;
    }
    pStPrivData->info.accvalidnum=pinfo->accvalidnum;
    pStPrivData->info.acccoef=pinfo->acccoef;
    pStPrivData->info.gyrovalidnum=pinfo->gyrovalidnum;
    pStPrivData->info.gyrocoef=pinfo->gyrorng;
    pStPrivData->eMode = HIGHRESO_MODE;
    pStPrivData->iWaitTime = WAIT_FOREVER;
    sys_mem_free(pinfo);
    return OK;
}

/**@fn         gsensor_manager_init_instance
 * @brief      创建gsensor_manager实例
 * @param[in]  pStInitParam 初始参数
 * @return     成功返回ISENSOR_MANAGER对象指针，  失败返回NULL
 */

IGsensor_manager *gsensor_manager_init_instance(void)
{
   INT32 iRet = ERROR;
   GSENSOR_MANAGER_BASE_T *pStBase = NULL;
   pStBase = (GSENSOR_MANAGER_BASE_T *)sys_mem_malloc(sizeof(GSENSOR_MANAGER_BASE_T));
    if(NULL == pStBase)
    {
        printf("sys_mem_malloc failed\n");
        return NULL;
    }

    iRet = gsensor_manager_interface(&pStBase->stInterface); 
    if(iRet < 0)
    {   
        printf("gsensor_manager_interface failed\n");
        sys_mem_free(pStBase);
        return NULL;
    }

    iRet = gsensor_manager_init_priv_data(&pStBase->stPrivData);
    if(iRet < 0)
    {
        printf("gsensor_manager_init_priv_data failed\n");
        sys_mem_free(pStBase);
        return NULL;
    }
    return &pStBase->stInterface;    
}
