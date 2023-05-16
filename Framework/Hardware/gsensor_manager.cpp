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
        LOGGER_ERROR("param error\n");
        return;
    }

    if(0 != pStPrivData->stReqMsgID)
    {
        iRet = sys_mqueue_close(&pStPrivData->stReqMsgID);
        if(OK == iRet)
        {
            pStPrivData->stReqMsgID = 0;
        }
    }
    if(0 != pStPrivData->stRespMsgID)
    {
        iRet = sys_mqueue_close(&pStPrivData->stRespMsgID);
        if(OK == iRet)
        {
            pStPrivData->stRespMsgID = 0;
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
 * @return     成功返回OK  失败返回错误码
 */
static INT32 gsensor_manager_send_ctrl_msg(GSENSOR_MANAGER_PRIV_DATA_T *pStPrivData, GSENSOR_MANAGER_CMD_E eCmd)
{
    INT32 iRet = ERROR;
    UINT32 uWaitMsec = 2000;
    GSENSOR_MANAGER_MSG_T stCtrlMsg;

    if(NULL == pStPrivData)
    {
        LOGGER_ERROR("param error\n");
        return iRet;
    }
    stCtrlMsg.eCmd = eCmd;
    iRet = sys_mqueue_send(&pStPrivData->stReqMsgID, (CHAR *)&stCtrlMsg, sizeof(GSENSOR_MANAGER_MSG_T), NO_WAIT);
    if(iRet < 0)
    {
        LOGGER_ERROR("sys_mqueue_send start msg error \n");
        return iRet;
    }
    iRet = sys_mqueue_recv(&pStPrivData->stRespMsgID, (CHAR *)&stCtrlMsg, sizeof(GSENSOR_MANAGER_MSG_T),uWaitMsec);
    if(iRet < 0)
    {
        LOGGER_ERROR("eCmd %d recv result time out\n", eCmd);
        return iRet;
    }
    iRet = (stCtrlMsg.eResult == GSENSOR_MANAGER_RESULT_SUCCESS) ? OK : ERROR;
    if(iRet < 0)
    {
        LOGGER_ERROR("control cmd %d error\n", eCmd);
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
        LOGGER_ERROR("param error\n");
        return;
    }
    stSegMsg.eCmd    = pStMsg->eCmd;
    stSegMsg.eResult = eResult;
    iRet = sys_mqueue_send(&pStPrivData->stRespMsgID, (CHAR *)&stSegMsg, sizeof(GSENSOR_MANAGER_MSG_T), NO_WAIT);
    if(iRet < 0)
    {
        LOGGER_ERROR("sys_mqueue_send start msg error \n");
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
        LOGGER_ERROR("param error\n");
        return iRet;
    }
    switch(pStMsg->eCmd)
    {
        case GSENSOR_MANAGER_CMD_START:
            if(GSENSOR_MANAGER_CMD_START == pStPrivData->eState)
            {
                LOGGER_INFO("gsensor already start\n");
                iRet = OK;
                break;
            }
            pStPrivData->eState = GSENSOR_MANAGER_CMD_START;
            pStPrivData->iWaitTime = NO_WAIT;
            iRet = OK;
            break;
        case GSENSOR_MANAGER_CMD_STOP:
            if(GSENSOR_MANAGER_CMD_STOP == pStPrivData->eState)
            {
                LOGGER_INFO("gsesor not in start state\n");
                break;
            }
            pStPrivData->iWaitTime = WAIT_FOREVER;
            pStPrivData->eState = GSENSOR_MANAGER_CMD_STOP;
            iRet = OK;
            break;
        default:
            LOGGER_INFO("ctrl cmd %d not support\n", pStMsg->eCmd);
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



/**@fn	       gsensor_manager_maxval	  
 * @brief	   获取极大值
 * @param[in]  validnum 有效位数
 * @return	   返回极大值
 */
static INT32 gsensor_manager_maxval(const UINT16 validnum)
{
    return ((1<<(validnum- 1))-1);
}

/**@fn	       gsensor_manager_convert	  
 * @brief	   获取正负数
 * @param[in]  maxval 极大值
 * @param[in]  data 原始数据
 * @return	   返回正负数
 */
static INT32 gsensor_manager_convert(const INT32 maxval,INT32 data)
{
    if(data < maxval){
        return data;
    }else{
        return -(data-maxval);
    }
}

/**@fn	       gsensor_manager_calculate	  
 * @brief	   进行数据换算获取mg值
 * @param[out]  StPrivData  私有数据结构指针
 * @param[in]  stRawData 原始数据
 * @return	   
 */
static VOID gsensor_manager_calculate(GSENSOR_MANAGER_PRIV_DATA_T *pStPrivData,const sensor_t stRawData)
{
    FLOAT32 tmpax,tmpay,tmpaz;
    if(NULL == pStPrivData)
    {
        LOGGER_ERROR("param error\n");
        return;
    }
    if(0 == strncmp(pStPrivData->cGyroname,"GYRO_LIS2DH12",strlen("GYRO_LIS2DH12"))){
      if((pStPrivData->info.accvalidnum != 0) ){
        INT32 accmaxval = 0;
        accmaxval = gsensor_manager_maxval(pStPrivData->info.accvalidnum);
        tmpax = stRawData.accx;
        tmpay = stRawData.accy;
        tmpaz = stRawData.accz;
        pStPrivData->data.accx = gsensor_manager_convert(accmaxval,tmpax)*(pStPrivData->info.acccoef);
        pStPrivData->data.accy = gsensor_manager_convert(accmaxval,tmpay)*(pStPrivData->info.acccoef);
        pStPrivData->data.accz = gsensor_manager_convert(accmaxval,tmpaz)*(pStPrivData->info.acccoef);
        LOGGER_INFO("rawdata x=%d y=%d z=%d  maxvalue=%d \r\n",stRawData.accx,stRawData.accy,stRawData.accz,accmaxval);
      }
    }
    LOGGER_INFO("x = %fmg, y=%fmg, z=%fmg\r\n",pStPrivData->data.accx,pStPrivData->data.accy,pStPrivData->data.accz);

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
        LOGGER_ERROR("param error\n");
        return;
    }
    iFd = pStPrivData->iFd;
    FD_ZERO(&read_fd);
    while(TRUE)
    {
        iRet = sys_mqueue_recv(&pStPrivData->stReqMsgID, (CHAR *)&stCtrlMsg, sizeof(stCtrlMsg), pStPrivData->iWaitTime);
        if (iRet >= 0)
        {
             iRet = gsensor_manager_recv_msg_handle(pStPrivData,&stCtrlMsg);
             if((iRet < 0) || (GSENSOR_MANAGER_CMD_STOP == pStPrivData->eState))
             {
                    pStPrivData->iWaitTime = WAIT_FOREVER;
                    continue;
             }
        }
        FD_SET(iFd, &read_fd);
		select(iFd + 1, &read_fd, NULL, NULL, NULL);
	    if(FD_ISSET(iFd, &read_fd))
		{
				sys_gsensor_getdata(iFd,&stData);
				gsensor_manager_calculate(pStPrivData,stData);		
		}
        sys_time_sleep_ms(1);
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
        LOGGER_ERROR("param error\n");
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
        LOGGER_ERROR("param error\n");
        return iRet;
    }
    do
    {
        //创建请求消息任务信号量
        iRet = snprintf(strMsgName,sizeof(strMsgName),"GsensorReq_%s", pStPrivData->cGyroname);
        if(iRet < 0)
        {
            LOGGER_ERROR("snprintf error\n");
            break;
        }
        iRet = sys_mqueue_create(&pStPrivData->stReqMsgID, strMsgName, 8, sizeof(GSENSOR_MANAGER_MSG_T));
        if(iRet < 0)
        {
            LOGGER_ERROR("sys_mqueue_create fail\n");
            break;
        }
        //创建回复消息任务信号量
        iRet = snprintf(strMsgName,sizeof(strMsgName),"GsensorResp_%s", pStPrivData->cGyroname);
        if(iRet < 0)
        {
            LOGGER_ERROR("snprintf error\n");
            break;
        }
        iRet = sys_mqueue_create(&pStPrivData->stRespMsgID, strMsgName, 8, sizeof(GSENSOR_MANAGER_MSG_T));
        if(iRet < 0)
        {
            LOGGER_ERROR("sys_mqueue_create fail\n");
            break;
        }
        iRet = snprintf(strThreadName,sizeof(strThreadName),"Gsensor_%dAxis",pStPrivData->info.uAxis);
        if(iRet < 0)
        {
            LOGGER_ERROR("snprintf error!\n");
            break;
        }
        iRet = sys_pthread_create(&pStPrivData->stThreadID, strThreadName, TASK_PRIORITY_5, SIZE_32KB, (FUNCPTR)gsensor_manager_work_thread, 1, pStPrivData);
        if(iRet != OK)
        {
            LOGGER_ERROR("create gsensor task failed!\n");
            break;
        }
    } while (FALSE);

    if(iRet < 0)
    {
        LOGGER_ERROR("gsensor_manager_init failed!\n");
        gsensor_manager_init_resource_release(pStPrivData);
        sys_mem_free(pStPrivData);
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
        LOGGER_ERROR("param error\n");
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
        LOGGER_ERROR("param error\n");
        return iRet;
    }
    if(GSENSOR_MANAGER_CMD_START != pStPrivData->eState)
    {
        LOGGER_INFO("avb talker not in start state\n");
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
        LOGGER_ERROR("param error\n");
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

/**@fn         gsensor_manager_get_data  
 * @brief      获取指定通道推流状态
 * @param[in]  IGsensor_manager  对象操作指针
 * @param[in]  pState 待获取推流状态
 * @return     成功返回OK     失败返回错误码
 */
static INT32 gsensor_manager_get_data(IGsensor_manager *pIGsensor_manager,Sensordata_t *data)
{
    INT32 iRet = ERROR;
    GSENSOR_MANAGER_PRIV_DATA_T *pStPrivData = NULL;

    pStPrivData = gsensor_manager_get_priv_data(pIGsensor_manager);
    if((NULL == pStPrivData))
    {
        LOGGER_ERROR("param error\n");
        return iRet;
    }
    data->accx = pStPrivData->data.accx;
    data->accy = pStPrivData->data.accy;
    data->accz = pStPrivData->data.accz;
    data->gyrox = pStPrivData->data.gyrox;
    data->gyroy = pStPrivData->data.gyroy;
    data->gyroz = pStPrivData->data.gyroz;
    data->magx = pStPrivData->data.magx;
    data->magy = pStPrivData->data.magy;
    data->magz = pStPrivData->data.magz;
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
        LOGGER_ERROR("param error\n");
        return iRet;
    }
    gsensor_manager_init_resource_release(pStPrivData);

    iRet = sys_gsensor_close(pStPrivData->iFd);
    if(iRet < 0)
    {
        LOGGER_ERROR("sys_gsensor_close error\n");
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
        LOGGER_ERROR("sys_mem_malloc failed\n");
        return ERROR;
    }
    pStPrivData->iFd = sys_gsensor_open();
    iFd = pStPrivData->iFd;
    if(iFd < 0)
    {
        LOGGER_ERROR("id < 0\n");
        sys_mem_free(pinfo);
        return ERROR;
    }
    iRet = sys_gsensor_getinfo(iFd,pinfo);
    if(iRet == ERROR)
    {
        LOGGER_ERROR("sys_gsensor_getinfo failed\n");
        sys_mem_free(pinfo);
        return ERROR;
    }
    pStPrivData->info.uAxis = pinfo->AXIS;
    sys_mem_copy(pStPrivData->cGyroname,pinfo->gyroname,strlen(pinfo->gyroname));
    iRet = sys_gsensor_getconfig(iFd,pinfo);
    if(iRet == ERROR)
    {
        LOGGER_ERROR("sys_gsensor_getinfo failed\n");
        sys_mem_free(pinfo);
        return ERROR;
    }
    pStPrivData->info.uAccrng = pinfo->accrng;
    pStPrivData->info.uGyrorng = pinfo->gyrorng;
    pStPrivData->eMode = NORMALE_MODE;
   
    if(0 == strncmp(pStPrivData->cGyroname,"GYRO_LIS2DH12",strlen("GYRO_LIS2DH12"))){
      iRet = sys_gsensor_normalpower(iFd);
      if(iRet == ERROR)
      {
        LOGGER_ERROR("sys_gsensor_normalpower failed\n");
        sys_mem_free(pinfo);
        return ERROR;
      }
       iRet = sys_gsensor_getparam(iFd,pinfo);
      if(iRet == ERROR)
      {
        LOGGER_ERROR("sys_gsensor_getparam failed\n");
        sys_mem_free(pinfo);
        return ERROR;
      }
      pStPrivData->info.accvalidnum=pinfo->accvalidnum;
      pStPrivData->info.acccoef=(FLOAT32)pinfo->acccoef/1000;
      pStPrivData->info.gyrovalidnum=pinfo->gyrovalidnum;
      pStPrivData->info.gyrocoef=pinfo->gyrorng;
      LOGGER_INFO("acc_rng = %d ,accvalidnum=%d , acccoef=%f\n",pStPrivData->info.uAccrng,pStPrivData->info.accvalidnum,pStPrivData->info.acccoef);
    }
    
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
        LOGGER_ERROR("sys_mem_malloc failed\n");
        return NULL;
    }

    iRet = gsensor_manager_interface(&pStBase->stInterface); 
    if(iRet < 0)
    {   
        LOGGER_ERROR("gsensor_manager_interface failed\n");
        sys_mem_free(pStBase);
        return NULL;
    }

    iRet = gsensor_manager_init_priv_data(&pStBase->stPrivData);
    if(iRet < 0)
    {
        LOGGER_ERROR("gsensor_manager_init_priv_data failed\n");
        sys_mem_free(pStBase);
        return NULL;
    }
    return &pStBase->stInterface;    
}
