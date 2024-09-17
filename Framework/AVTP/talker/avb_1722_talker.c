#include "avb_1722_talker_priv.h"
#include "avb_1722_talker.h"
#include "avb_1722_manager.h"

/**@fn         avb_1722_talker_init_resource_release   
 * @brief      释放单例初始化过程中申请的资源
 * @param[in]  pStPrivData   私有数据指针
 * @return     VOID
 */
static VOID avb_1722_talker_init_resource_release(AVB_1722_TALKER_PRIV_DATA_T *pStPrivData)
{
    INT iRet = ERROR;
    if(NULL == pStPrivData)
    {
        AVB_ERROR("param error\n");
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
    if(-1 != pStPrivData->iSock)
    {
        iRet = avb_avtp_socket_close(pStPrivData->iSock);
        if(OK == iRet)
        {
            pStPrivData->iSock = -1;
        }
    }
    if(NULL != pStPrivData->pIAvtpPdu)
    {
        pStPrivData->pIAvtpPdu->Release(pStPrivData->pIAvtpPdu);
        pStPrivData->pIAvtpPdu = NULL;
    }
    if(NULL != pStPrivData->pIRtpPayloadH264)
    {
        pStPrivData->pIRtpPayloadH264->Release(pStPrivData->pIRtpPayloadH264);
        pStPrivData->pIRtpPayloadH264 = NULL;
    }
    if(NULL != pStPrivData->pIFrameReader)
    {
        pStPrivData->pIFrameReader->Destory(pStPrivData->pIFrameReader);
        pStPrivData->pIFrameReader = NULL;
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
/**@fn          avb_1722_talker_work_thread    
 * @brief       AVBTalker工作线程
 * @param[in]   pStPrivData  私有数据结构指针
 * @return      VOID
 */
static VOID avb_1722_talker_work_thread(AVB_1722_TALKER_PRIV_DATA_T *pStPrivData)
{
    INT iRet = ERROR;
    AVB_1722_TALKER_MSG_T  stCtrlMsg = {0};


    if(NULL == pStPrivData)
    {
        AVB_ERROR("param error\n");
        return;
    }

    while(TRUE)
    {
        pStPrivData->st1722FrameInfo.bFirstFrame = FALSE;

        /* rtp payload */
        iRet = pStPrivData->pIRtpPayloadH264->Serial(pStPrivData->pIRtpPayloadH264, pStPrivData->stFrameInfo.pFrameBuff, pStPrivData->stFrameInfo.pStEsInfo->uFrameSize);
        if(iRet < 0)
        {
            AVB_ERROR("pIRtpPayloadH264->Serial failed\n");
        }
        sys_time_sleep_ms(1);
    }
}


/**@fn          avb_1722_talker_init_frame_reader    
 * @brief       初始化帧读取器
 * @param[in]   pStPrivData  私有数据结构指针
 * @return     成功返回OK  失败返回错误码
 */
static INT avb_1722_talker_init_frame_reader(AVB_1722_TALKER_PRIV_DATA_T *pStPrivData)
{
    return OK;
}

/**@fn         avb_1722_talker_avtp_h264_instance
 * @brief      创建avtp pdu实例
 * @param[in]  pStPrivData   私有数据指针
 * @return     成功返回OK     失败返回错误码
 */
static INT avb_1722_talker_avtp_h264_instance(AVB_1722_TALKER_PRIV_DATA_T *pStPrivData)
{
    INT iRet = ERROR;
    if(NULL == pStPrivData)
    {
        LOGGER_ERROR("param error\n");
        return iRet;
    }
    pStPrivData->pIAvtpPdu = avtp_pdu_init_instance(pStPrivData->uMaxPayloadLen);
    if(NULL == pStPrivData->pIAvtpPdu)
    {
        AVB_ERROR("create avtp pdu instance failed\n");
        return ERROR;
    }
    do 
    {
        iRet = pStPrivData->pIAvtpPdu->SetSubType(pStPrivData->pIAvtpPdu, AVTP_PDU_SUBTYPE_CVF);
        if(iRet < 0)
        {
            AVB_ERROR("avtp pdu set subtype error\n");
            break;
        }
        iRet = pStPrivData->pIAvtpPdu->SetField(pStPrivData->pIAvtpPdu, AVTP_CVF_FIELD_SV, 1);
        if(iRet < 0)
        {
            AVB_ERROR("avtp pdu set failed\n");
            break;
        }
        iRet = pStPrivData->pIAvtpPdu->SetField(pStPrivData->pIAvtpPdu, AVTP_CVF_FIELD_FORMAT, AVTP_CVF_FORMAT_RFC);
        if(iRet < 0)
        {
            AVB_ERROR("avtp pdu set failed\n");
            break;
        }
        iRet = pStPrivData->pIAvtpPdu->SetField(pStPrivData->pIAvtpPdu, AVTP_CVF_FIELD_FORMAT_SUBTYPE, AVTP_CVF_FORMAT_SUBTYPE_H264);
        if(iRet < 0)
        {
            AVB_ERROR("avtp pdu set failed\n");
            break;
        }
        iRet = pStPrivData->pIAvtpPdu->SetField(pStPrivData->pIAvtpPdu, AVTP_CVF_FIELD_STREAM_ID, pStPrivData->stCfgParam.stTransmitParam.uStreamID);
        if(iRet < 0)
        {
            AVB_ERROR("avtp pdu set failed\n");
            break;
        }
        iRet = pStPrivData->pIAvtpPdu->SetField(pStPrivData->pIAvtpPdu, AVTP_CVF_FIELD_H264_PTV, 0);
        if(iRet < 0)
        {
            AVB_ERROR("avtp pdu set failed\n");
            break;
        }
        iRet = pStPrivData->pIAvtpPdu->SetField(pStPrivData->pIAvtpPdu, AVTP_CVF_FIELD_H264_TIMESTAMP, 0);
        if(iRet < 0)
        {
            AVB_ERROR("avtp pdu set failed\n");
            break;
        }
    }while(FALSE);
    if(iRet < 0)
    {
        pStPrivData->pIAvtpPdu->Release(pStPrivData->pIAvtpPdu);
        pStPrivData->pIAvtpPdu = NULL;
        return iRet;
    }
    return OK;
}

/**@fn         avb_1722_talker_avtp_instance_create
 * @brief      创建avtp实例
 * @param[in]  pStPrivData   私有数据指针
 * @return     成功返回OK     失败返回错误码
 */
static INT avb_1722_talker_avtp_instance_create(AVB_1722_TALKER_PRIV_DATA_T *pStPrivData)
{
    INT iRet = ERROR;

    if(NULL == pStPrivData)
    {
        LOGGER_ERROR("param error\n");
        return iRet;
    }
    switch(pStPrivData->uStreamType)
    {
        case AVB_STREAM_TYPE_H264VIDEO: //h264视频流
            iRet = avb_1722_talker_avtp_h264_instance(pStPrivData);
            break;
        case AVB_STREAM_TYPE_UNKNOW:

            break;
        default:
            LOGGER_ERROR("stream type %d not support\n", pStPrivData->uStreamType);
            iRet = ERROR;
            break;
    }
    return iRet;
}

/**@fn         avb_1722_talker_get_priv_data
 * @brief      获取私有信息指针
 * @param[in]  pIAVB1722Talker  IAVB1722Talker指针
 * @return    成功返回AVB_1722_TALKER_PRIV_DATA_T类型指针,失败返回NULL
 */
static AVB_1722_TALKER_PRIV_DATA_T *avb_1722_talker_get_priv_data(IAVB1722Talker *pIAVB1722Talker)
{
    AVB_1722_TALKER_BASE_T *pStBase = NULL;
    pStBase = (AVB_1722_TALKER_BASE_T*)pIAVB1722Talker;
    if(NULL == pStBase)
    {
        AVB_ERROR("param error\n");
        return NULL;
    }
    return &pStBase->stPrivData;
}


/**@fn         avb_1722_talker_send_ctrl_msg        
 * @brief      发送消息          
 * @param[in]  pStPrivData私有数据结构体指针
 * @param[in]  eStates    RTSP会话状态类型
 * @return     成功返回OK  失败返回错误码
 */
static INT avb_1722_talker_send_ctrl_msg(AVB_1722_TALKER_PRIV_DATA_T *pStPrivData, AVB_1722_TALKER_CMD_E eCmd)
{
    INT iRet = ERROR;
    UINT uWaitMsec = 2000;
    AVB_1722_TALKER_MSG_T stCtrlMsg = {0};

    if(NULL == pStPrivData)
    {
        AVB_ERROR("param error\n");
        return iRet;
    }
    stCtrlMsg.eCmd = eCmd;
    iRet = sys_mqueue_send(&pStPrivData->stReqMsgID, (CHAR *)&stCtrlMsg, sizeof(AVB_1722_TALKER_MSG_T), NO_WAIT);
    if(iRet < 0)
    {
        AVB_ERROR("sys_mqueue_send start msg error \n");
        return iRet;
    }
    iRet = sys_mqueue_recv(&pStPrivData->stRespMsgID, (CHAR *)&stCtrlMsg, sizeof(AVB_1722_TALKER_MSG_T),uWaitMsec);
    if(iRet < 0)
    {
        AVB_ERROR("eCmd %d recv result time out\n", eCmd);
        return iRet;
    }
    iRet = (stCtrlMsg.eResult == AVB_1722_TALKER_RESULT_SUCCESS) ? OK : ERROR;
    if(iRet < 0)
    {
        AVB_ERROR("control cmd %d error\n", eCmd);
        return iRet;
    }
    return iRet;
}

/**@fn	       avb_1722_talker_send_msg_response	  
 * @brief	   发送消息响应
 * @param[in]  pStSegMsg   消息指针
 * @param[in]  pStPrivData 私有数据指针
 * @return	   无
 */
static VOID avb_1722_talker_send_msg_response(AVB_1722_TALKER_PRIV_DATA_T *pStPrivData, AVB_1722_TALKER_MSG_T *pStMsg, AVB_1722_TALKER_RESULT_E eResult)
{
    INT iRet = ERROR;
    AVB_1722_TALKER_MSG_T stSegMsg = {0};

    if((NULL == pStPrivData) || (NULL == pStMsg))
    {
        AVB_ERROR("param error\n");
        return;
    }
    stSegMsg.eCmd    = pStMsg->eCmd;
    stSegMsg.eResult = eResult;
    iRet = sys_mqueue_send(&pStPrivData->stRespMsgID, (CHAR *)&stSegMsg, sizeof(AVB_1722_TALKER_MSG_T), NO_WAIT);
    if(iRet < 0)
    {
        AVB_ERROR("sys_mqueue_send start msg error \n");
        return;
    }
    return;
}


/**@fn	       avb_1722_talker_recv_msg_handle	  
 * @brief	   消息处理接口
 * @param[in]  pStPrivData 私有数据指针
 * @param[in]  pStMsg   消息指针
 * @return	  成功返回OK 其它返回ERROR
 */
static INT avb_1722_talker_recv_msg_handle(AVB_1722_TALKER_PRIV_DATA_T *pStPrivData, AVB_1722_TALKER_MSG_T *pStMsg)
{
    INT iRet = ERROR;
    UINT uCurrentTime = 0;
    STRAEAM_FRAME_INFO_T stFrameInfo = {0};

    if((NULL == pStPrivData) || (NULL == pStMsg))
    {
        AVB_ERROR("param error\n");
        return iRet;
    }
    switch(pStMsg->eCmd)
    {
        case AVB_1722_TALKER_CMD_START:
            if(AVB_1722_TALKER_CMD_START == pStPrivData->eState)
            {
                AVB_INFO("avb talker already start\n");
                iRet = OK;
                break;
            }
            pStPrivData->stFrameInfo.pFrameBuff = pStPrivData->st1722FrameInfo.pFrameBuf;
            pStPrivData->stFrameInfo.uFrameBuffSize = pStPrivData->st1722FrameInfo.uFrameBufSize;
            if((NULL == pStPrivData->stFrameInfo.pFrameBuff) || (0 == pStPrivData->stFrameInfo.uFrameBuffSize))
            {
                //帧缓存，在setparam设置并申请空间，避免调用顺序导致问题
                AVB_ERROR("frame buff is Null or frame buff size is 0.\n");
                break;
            }

            iRet = sys_time_get_utc_sec(&uCurrentTime);
            if(OK != iRet)
            {
                AVB_ERROR("sys_time_get_utc_sec error\n");
            }
            stFrameInfo.pFrameBuff = pStPrivData->st1722FrameInfo.pFrameBuf;
            stFrameInfo.uFrameBuffSize = pStPrivData->st1722FrameInfo.uFrameBufSize;
            iRet = pStPrivData->pIFrameReader->SeekKeyFrameByTime(pStPrivData->pIFrameReader, uCurrentTime, &stFrameInfo);
            if(iRet < 0)
            {
                AVB_ERROR("SeekKeyFrameByTime failed\n");
                break;
            }
            pStPrivData->iWaitTime = 2;
            pStPrivData->st1722FrameInfo.bFirstFrame = TRUE;
            pStPrivData->eState = AVB_1722_TALKER_CMD_START;
            iRet = OK;
            break;
        case AVB_1722_TALKER_CMD_STOP:
            if(AVB_1722_TALKER_CMD_START != pStPrivData->eState)
            {
                AVB_ERROR("avb talker not in start state\n");
                break;
            }
            pStPrivData->iWaitTime = WAIT_FOREVER;
            pStPrivData->eState = AVB_1722_TALKER_CMD_STOP;
            pStPrivData->iFlag = 0;
            iRet = OK;
            break;
        default:
            AVB_ERROR("ctrl cmd %d not support\n", pStMsg->eCmd);
            iRet = ERROR;
            break;
    }
    
    //发送本次操作的结果 成功返回OK  失败返回ERROR
    if(iRet < 0)
    {   
        avb_1722_talker_send_msg_response(pStPrivData,pStMsg,AVB_1722_TALKER_RESULT_FAILED);
        return iRet;
    }
    avb_1722_talker_send_msg_response(pStPrivData,pStMsg,AVB_1722_TALKER_RESULT_SUCCESS);
    return iRet;
}



/**@fn         avb_1722_talker_socket_create
 * @brief      创建socket
 * @param[in]  pStPrivData   私有数据指针
 * @return     成功返回OK     失败返回错误码
 */
static INT avb_1722_talker_socket_create(AVB_1722_TALKER_PRIV_DATA_T *pStPrivData)
{
    INT iRet = ERROR;

    iRet = avtp_talker_socket_create(pStPrivData->stTransmitParam.iSockPriority, pStPrivData->stTransmitParam.aInterfaceName, 
        pStPrivData->stTransmitParam.aMacAddr,  pStPrivData->stTransmitParam.iProtocol, &pStPrivData->stDstSockAddr);
    if (iRet < 0)
    {
        LOGGER_ERROR("avb_avtp_talker_socket_create failed\n");
    }
    pStPrivData->iSock = iRet;
    return iRet;
}

/**@fn         avb_1722_talker_init   
 * @brief      初始化参数
 * @param[in]  pIAVBTalker   AVBLiveTalker对象操作指针
 * @param[in]  pTransmitParam   传输参数
 * @return     成功返回OK     失败返回错误码
 */
static INT avb_1722_talker_init(IAVB1722Talker *pIAVB1722Talker)
{
    INT iRet = ERROR; 
    CHAR strMsgName[64] = {0};
    CHAR strThreadName[64] = {0}; 
    AVB_1722_TALKER_PRIV_DATA_T *pStPrivData = NULL;
    VOID *pDest = NULL;

    pStPrivData = avb_1722_talker_get_priv_data(pIAVB1722Talker);
    if((NULL == pStPrivData))
    {
        LOGGER_ERROR("param error\n");
        return iRet;
    }
    do
    {
        iRet = snprintf(strMsgName,sizeof(strMsgName),"1722TkReq_%d", pStPrivData->uChannel);
        if(iRet < 0)
        {
            LOGGER_ERROR("snprintf error\n");
            break;
        }
        iRet = sys_mqueue_create(&pStPrivData->stReqMsgID, strMsgName, 8, sizeof(AVB_1722_TALKER_MSG_T));
        if(iRet < 0)
        {
            LOGGER_ERROR("sys_mqueue_create fail\n");
            break;
        }
        iRet = snprintf(strMsgName,sizeof(strMsgName),"1722TkResp_%d", pStCfgParam->uChannel);
        if(iRet < 0)
        {
            LOGGER_ERROR("snprintf error\n");
            break;
        }
        iRet = sys_mqueue_create(&pStPrivData->stRespMsgID, strMsgName, 8, sizeof(AVB_1722_TALKER_MSG_T));
        if(iRet < 0)
        {
            LOGGER_ERROR("sys_mqueue_create fail\n");
            break;
        }

        /*创建server*/
        iRet = avb_1722_talker_socket_create(pStPrivData);
        if(iRet < 0)
        {
            LOGGER_ERROR("avb_1722_talker_socket_create error\n");
            break;
        }
        /*创建avtp对象及初始化*/
        iRet = avb_1722_talker_avtp_instance_create(pStPrivData);
        if(iRet < 0)
        {
            LOGGER_ERROR("avb_1722_talker_avtp_instance_create error\n");
            break;
        }
        /*创建rtp payload对象*/
       // iRet = avb_1722_talker_rtp_payload_instance(pStPrivData);
       // if(iRet < 0)
      //  {
       //     AVB_ERROR("avb_1722_talker_rtp_payload_instance error\n");
       //     break;
      //  }
        /*创建帧读取*/
        iRet = avb_1722_talker_init_frame_reader(pStPrivData);
        if(iRet < 0)
        {
            LOGGER_ERROR("avb_1722_talker_rtp_payload_instance error\n");
            break;
        }
        iRet = snprintf(strThreadName,sizeof(strThreadName),"1722Talker_%d",pStPrivData->uChannel);
        if(iRet < 0)
        {
            LOGGER_ERROR("snprintf error!\n");
            break;
        }
        iRet = sys_pthread_create(&pStPrivData->stThreadID, strThreadName, TASK_PRIORITY_5, SIZE_32KB, (FUNCPTR)avb_1722_talker_work_thread, 1, pStPrivData);
        if(iRet != OK)
        {
            LOGGER_ERROR("create avtp task failed!\n");
            break;
        }
    } while (FALSE);

    if(iRet < 0)
    {
        LOGGER_ERROR("init 1722 talker failed!\n");
        avb_1722_talker_init_resource_release(pStPrivData);
        return iRet;
    }
    return OK;
}


/**@fn         avb_1722_talker_set_param   
 * @brief      设置相关参数
 * @param[in]  pIAVB1722Talker 对象操作指针
 * @param[in]  pParam 参数结构体指针
 * @return     成功返回OK     失败返回错误码
 * @note       设置AVB 1722 talker相关参数
 */
static INT avb_1722_talker_set_param(IAVB1722Talker *pIAVB1722Talker, AVB_1722_TALKER_SET_PARAM_T *pStParam)
{
    INT iRet = ERROR;
    UINT uFrameBufSize = 0; 
    AVB_1722_TALKER_PRIV_DATA_T *pStPrivData = NULL;

    pStPrivData = avb_1722_talker_get_priv_data(pIAVB1722Talker);
    if(NULL == pStPrivData)
    {
        LOGGER_ERROR("param error\n");
        return iRet;
    }

    /*申请缓存区，用于拷贝帧数据*/
    if(NULL == pStPrivData->st1722FrameInfo.pFrameBuf)
    {
        uFrameBufSize = (0u == pStParam->uFrameBufSize)?AVB_1722_TALKER_FRAME_BUFF_SIZE:pStParam->uFrameBufSize;
        pStPrivData->st1722FrameInfo.pFrameBuf = sys_mem_malloc(uFrameBufSize);
        if(NULL == pStPrivData->st1722FrameInfo.pFrameBuf)
        {
            LOGGER_ERROR("sys_mem_malloc fail\n");
            return ERROR;
        }
        pStPrivData->st1722FrameInfo.uFrameBufSize = uFrameBufSize;
    }
    pStPrivData->stTransmitParam.iSockPriority=pStParam->stParam.iSockPriority;
    pStPrivData->stTransmitParam.uMaxTransmitTime = pStParam->stParam.uMaxTransmitTime;
    pStPrivData->stTransmitParam.uMaxPayloadLen = pStParam->stParam.uMaxPayloadLen;
     pStPrivData->stTransmitParam.uStreamID = pStParam->stParam.uStreamID;
    sys_mem_copy(pStPrivData->stTransmitParam.aInterfaceName,pStParam->stParam.aInterfaceName,strlen(pStParam->stParam.aInterfaceName));
    sys_mem_copy(pStPrivData->stTransmitParam.aMacAddr,pStParam->stParam.aMacAddr,strlen(pStParam->stParam.aMacAddr));

    return OK;
}

/**@fn         avb_1722_talker_start      
 * @brief      开启服务
 * @param[in]  pIAVB1722Talker   AVBLiveTalker对象操作指针
 * @return     成功返回OK     失败返回错误码
 */
static INT avb_1722_talker_start(IAVB1722Talker *pIAVB1722Talker)
{
    INT iRet = ERROR;
    AVB_1722_TALKER_PRIV_DATA_T *pStPrivData = NULL;

    pStPrivData = avb_1722_talker_get_priv_data(pIAVB1722Talker);
    if(NULL == pStPrivData)
    {
        AVB_ERROR("param error\n");
        return iRet;
    }
    return avb_1722_talker_send_ctrl_msg(pStPrivData, AVB_1722_TALKER_CMD_START);
}

/**@fn         avb_1722_talker_stop   
 * @brief      关闭服务
 * @param[in]  pIAVB1722Talker   IAVB1722Talker对象操作指针
 * @return     成功返回OK     失败返回错误码
 */
static INT avb_1722_talker_stop(IAVB1722Talker *pIAVB1722Talker)
{
    INT iRet = ERROR; 
    AVB_1722_TALKER_PRIV_DATA_T *pStPrivData = NULL;

    pStPrivData = avb_1722_talker_get_priv_data(pIAVB1722Talker);
    if(NULL == pStPrivData)
    {
        AVB_ERROR("param error\n");
        return iRet;
    }
    if(AVB_1722_TALKER_CMD_START != pStPrivData->eState)
    {
        AVB_ERROR("avb talker not in start state\n");
        return iRet;
    }
    return avb_1722_talker_send_ctrl_msg(pStPrivData, AVB_1722_TALKER_CMD_STOP);
}

/**@fn         avb_1722_talker_get_state   
 * @brief      获取指定通道推流状态
 * @param[in]  pIAVB1722Talker  对象操作指针
 * @param[in]  pState 待获取推流状态
 * @return     成功返回OK     失败返回错误码
 */
static INT avb_1722_talker_get_state(IAVB1722Talker *pIAVB1722Talker, AVB_TALKER_STATE_E *pState)
{
    INT iRet = ERROR; 
    AVB_1722_TALKER_PRIV_DATA_T *pStPrivData = NULL;

    pStPrivData = avb_1722_talker_get_priv_data(pIAVB1722Talker);
    if((NULL == pStPrivData) || (NULL == pState))
    {
        AVB_ERROR("param error\n");
        return iRet;
    }
    switch(pStPrivData->eState)
    {
        case AVB_1722_TALKER_CMD_START:
            *pState = AVB_TALKER_STATE_START;
            break;
        case AVB_1722_TALKER_CMD_STOP:
            *pState = AVB_TALKER_STATE_STOP;
            break;
        default:
            *pState = AVB_TALKER_STATE_INIT;
            break;
    }
    return OK;
}

/**@fn         avb_1722_talker_release
 * @brief      模块资源释放
 * @param[in]  pIAVB1722Talker   IAVB1722Talker对象操作指针
 * @return     VOID
 */
static VOID avb_1722_talker_release(IAVB1722Talker *pIAVB1722Talker)
{ 
    AVB_1722_TALKER_PRIV_DATA_T *pStPrivData = NULL;

    pStPrivData = avb_1722_talker_get_priv_data(pIAVB1722Talker);
    if(NULL == pStPrivData)
    {
        AVB_ERROR("param error\n");
        return;
    }
    
    avb_1722_talker_init_resource_release(pStPrivData);
    avb_1722_talker_priv_resource_release(pStPrivData);
    sys_mem_free(pIAVB1722Talker);
    return;
}
/**@fn         avb_1722_talker_init_interface    
 * @brief      初始化构造对外接口
 * @param[in]  pIAVBTalker  AVBTalker对象接口
 * @return     成功返回OK  失败返回错误码
 */
static INT avb_1722_talker_init_interface(IAVB1722Talker *pIAVB1722Talker)
{
    if(NULL == pIAVB1722Talker)
    {
        AVB_ERROR("param error\n");
        return ERROR;
    }
    pIAVB1722Talker->Init = avb_1722_talker_init;
    pIAVB1722Talker->SetParam = avb_1722_talker_set_param;
    pIAVB1722Talker->Start = avb_1722_talker_start;
    pIAVB1722Talker->Stop = avb_1722_talker_stop;
    pIAVB1722Talker->GetState = avb_1722_talker_get_state;
    pIAVB1722Talker->Release = avb_1722_talker_release;
    return OK;
}

/**@fn          avb_1722_talker_init_priv_data    
 * @brief       AVBLiveVideoTalker私有数据初始化
 * @param[in]   pStPrivData  私有数据结构指针
 * @return     成功返回OK  失败返回错误码
 */
static INT avb_1722_talker_init_priv_data(AVB_1722_TALKER_PRIV_DATA_T *pStPrivData)
{
    INT iRet = ERROR;

    if((NULL == pStPrivData) || (NULL == pStInitParam))
    {
        LOGGER_ERROR("param error\n");
        return iRet;
    }
    pStPrivData->iWaitTime = WAIT_FOREVER;
    pStPrivData->iSock = -1;
    pStPrivData->uMaxPayloadLen = 1400;
    pStPrivData->uChannel = 1;

    return OK;
}

/**@fn         avb_1722_talker
 * @brief      创建avb 1722 talker实例
 * @param[in]  pStInitParam 初始参数
 * @return     成功返回IAVB1722Talker对象指针，  失败返回NULL
 */
IAVB1722Talker *avb_1722_talker_instance(void)
{   
    INT iRet = ERROR;
    AVB_1722_TALKER_BASE_T *pStBase = NULL;
    VOID *pMemset = NULL;
    
    pStBase = sys_mem_malloc(sizeof(AVB_1722_TALKER_BASE_T));
    if(NULL == pStBase)
    {
        LOGGER_ERROR("sys_mem_malloc failed\n");
        return NULL;
    }
    pMemset = memset(pStBase,0,sizeof(AVB_1722_TALKER_BASE_T));
    if(NULL == pMemset)
    {
        return NULL;
    }
    
    iRet = avb_1722_talker_init_interface(&pStBase->stInterface);
    if(iRet < 0)
    {   
        LOGGER_ERROR("avb_1722_talker_init_interface failed\n");
        sys_mem_free(pStBase);
        return NULL;
    }

    iRet = avb_1722_talker_init_priv_data(&pStBase->stPrivData);
    if(iRet < 0)
    {
        LOGGER_ERROR("avb_1722_talker_init_priv_data failed\n");
        sys_mem_free(pStBase);
        return NULL;
    }
    return &pStBase->stInterface;
}