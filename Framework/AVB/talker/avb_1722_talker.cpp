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
    INT32 iRet = ERROR;
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
    INT32 iRet = ERROR;
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
static INT32 avb_1722_talker_init_frame_reader(AVB_1722_TALKER_PRIV_DATA_T *pStPrivData)
{
    return OK;
}
/**@fn         avb_1722_talker_avtp_instance_create
 * @brief      创建avtp实例
 * @param[in]  pStPrivData   私有数据指针
 * @return     成功返回OK     失败返回错误码
 */
static INT32 avb_1722_talker_avtp_instance_create(AVB_1722_TALKER_PRIV_DATA_T *pStPrivData)
{
    INT32 iRet = ERROR;

    if(NULL == pStPrivData)
    {
        AVB_ERROR("param error\n");
        return iRet;
    }
    switch(pStPrivData->uStreamType)
    {
        case AVB_STREAM_TYPE_H264VIDEO: //h264视频流
            //iRet = avb_1722_talker_avtp_h264_instance(pStPrivData);
            break;
        case AVB_STREAM_TYPE_UNKNOW:

            break;
        default:
            AVB_ERROR("stream type %d not support\n", pStPrivData->stCfgParam.uStreamType);
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



/**@fn         avb_1722_talker_socket_create
 * @brief      创建socket
 * @param[in]  pStPrivData   私有数据指针
 * @return     成功返回OK     失败返回错误码
 */
static INT32 avb_1722_talker_socket_create(AVB_1722_TALKER_PRIV_DATA_T *pStPrivData)
{
    INT32 iRet = ERROR;

    iRet = avtp_talker_socket_create(pStPrivData->stTransmitParam.iSock, pStPrivData->stTransmitParam.aInterfaceName, 
        pStPrivData->stTransmitParam.aMacAddr,  pStPrivData->stTransmitParam.iProtocol, &pStPrivData->stDstSockAddr);
    if (iRet < 0)
    {
        AVB_ERROR("avb_avtp_talker_socket_create failed\n");
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
static INT32 avb_1722_talker_init(IAVB1722Talker *pIAVB1722Talker)
{
    INT32 iRet = ERROR; 
    CHAR strMsgName[64] = {0};
    CHAR strThreadName[64] = {0}; 
    AVB_1722_TALKER_PRIV_DATA_T *pStPrivData = NULL;
    VOID *pDest = NULL;

    pStPrivData = avb_1722_talker_get_priv_data(pIAVB1722Talker);
    if((NULL == pStPrivData))
    {
        AVB_ERROR("param error\n");
        return iRet;
    }
    do
    {
        iRet = snprintf(strMsgName,sizeof(strMsgName),"1722TkReq_%d", pStPrivData->uChannel);
        if(iRet < 0)
        {
            AVB_ERROR("snprintf error\n");
            break;
        }
        iRet = sys_mqueue_create(&pStPrivData->stReqMsgID, strMsgName, 8, sizeof(AVB_1722_TALKER_MSG_T));
        if(iRet < 0)
        {
            AVB_ERROR("sys_mqueue_create fail\n");
            break;
        }
        iRet = snprintf(strMsgName,sizeof(strMsgName),"1722TkResp_%d", pStCfgParam->uChannel);
        if(iRet < 0)
        {
            AVB_ERROR("snprintf error\n");
            break;
        }
        iRet = sys_mqueue_create(&pStPrivData->stRespMsgID, strMsgName, 8, sizeof(AVB_1722_TALKER_MSG_T));
        if(iRet < 0)
        {
            AVB_ERROR("sys_mqueue_create fail\n");
            break;
        }

        /*创建server*/
        iRet = avb_1722_talker_socket_create(pStPrivData);
        if(iRet < 0)
        {
            AVB_ERROR("avb_1722_talker_socket_create error\n");
            break;
        }
        /*创建avtp对象及初始化*/
        iRet = avb_1722_talker_avtp_instance_create(pStPrivData);
        if(iRet < 0)
        {
            AVB_ERROR("avb_1722_talker_avtp_instance_create error\n");
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
            AVB_ERROR("avb_1722_talker_rtp_payload_instance error\n");
            break;
        }
        iRet = snprintf(strThreadName,sizeof(strThreadName),"1722Talker_%d",pStPrivData->uChannel);
        if(iRet < 0)
        {
            AVB_ERROR("snprintf error!\n");
            break;
        }
        iRet = sys_pthread_create(&pStPrivData->stThreadID, strThreadName, TASK_PRIORITY_5, SIZE_32KB, (FUNCPTR)avb_1722_talker_work_thread, 1, pStPrivData);
        if(iRet != OK)
        {
            AVB_ERROR("create avtp task failed!\n");
            break;
        }
    } while (FALSE);

    if(iRet < 0)
    {
        AVB_ERROR("init 1722 talker failed!\n");
        avb_1722_talker_init_resource_release(pStPrivData);
        return iRet;
    }
    return OK;
}

/**@fn         avb_1722_talker_init_interface    
 * @brief      初始化构造对外接口
 * @param[in]  pIAVBTalker  AVBTalker对象接口
 * @return     成功返回OK  失败返回错误码
 */
static INT32 avb_1722_talker_init_interface(IAVB1722Talker *pIAVB1722Talker)
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
static INT32 avb_1722_talker_init_priv_data(AVB_1722_TALKER_PRIV_DATA_T *pStPrivData, AVB_1722_TALKER_INIT_PARAM_T *pStInitParam)
{
    INT32 iRet = ERROR;

    if((NULL == pStPrivData) || (NULL == pStInitParam))
    {
        AVB_ERROR("param error\n");
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
IAVB1722Talker *avb_1722_talker_instance(AVB_1722_TALKER_INIT_PARAM_T *pStInitParam)
{   
    INT32 iRet = ERROR;
    AVB_1722_TALKER_BASE_T *pStBase = NULL;
    VOID *pMemset = NULL;
    
    pStBase = sys_mem_malloc(sizeof(AVB_1722_TALKER_BASE_T));
    if(NULL == pStBase)
    {
        AVB_ERROR("sys_mem_malloc failed\n");
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
        AVB_ERROR("avb_1722_talker_init_interface failed\n");
        sys_mem_free(pStBase);
        return NULL;
    }

    iRet = avb_1722_talker_init_priv_data(&pStBase->stPrivData, pStInitParam);
    if(iRet < 0)
    {
        AVB_ERROR("avb_1722_talker_init_priv_data failed\n");
        sys_mem_free(pStBase);
        return NULL;
    }
    return &pStBase->stInterface;
}