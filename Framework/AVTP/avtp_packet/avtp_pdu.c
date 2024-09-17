#include "avtp_pdu_priv.h"

/**@fn         avtp_pdu_get_priv_data
 * @brief      获取私有信息指针
 * @param[in]  pIAvtpPdu  AVTPDU指针
 * @return    成功返回AVTP_PRIV_DATA_T类型指针,失败返回NULL
 */
static AVTP_PDU_PRIV_DATA_T *avtp_pdu_get_priv_data(IAvtpPdu *pIAvtpPdu)
{
    AVTP_PDU_BASE_T *pStBase = NULL;

    pStBase = (AVTP_PDU_BASE_T*)pIAvtpPdu;
    if(NULL == pStBase)
    {
        AVTP_ERROR("param error\n");
        return NULL;
    }
    return &pStBase->stPrivData;
}

 /**@fn	       avtp_pdu_init_sub_instance_cvf	  
 * @brief	   初始化子实例-CVF
 * @param[in]  pStPrivData   私有数据指针
 * @return	   成功返回OK  失败返回错误码
 */
static INT avtp_pdu_init_sub_instance_cvf(AVTP_PDU_PRIV_DATA_T *pStPrivData)
{
    INT iRet = ERROR;

    if(NULL == pStPrivData)
    {
        AVTP_ERROR("param error\n");
        return iRet;
    }
    if(NULL != pStPrivData->pCVFInstance)
    {
        //已经创建单例
        return OK;
    }
    pStPrivData->pCVFInstance = avtp_cvf_pdu_init_instance();
    if(NULL == pStPrivData->pCVFInstance)
    {
        AVTP_ERROR("avtp_cvf_pdu_init_instance error\n");
        return iRet;
    }
    iRet = pStPrivData->pCVFInstance->Init(pStPrivData->pCVFInstance, pStPrivData->uPduLength);
    if(iRet < 0)
    {
        AVTP_ERROR("cvf instance init error\n");
        pStPrivData->pCVFInstance->Release(pStPrivData->pCVFInstance);
        pStPrivData->pCVFInstance = NULL;
    }
    return iRet;
}

 /**@fn	       avtp_pdu_init_sub_instance	  
 * @brief	   初始化子实例
 * @param[in]  pStPrivData   私有数据指针
 * @param[in]  eSubType avtp类型
 * @return	   成功返回OK  失败返回错误码
 */
static INT avtp_pdu_init_sub_instance(AVTP_PDU_PRIV_DATA_T *pStPrivData, AVTP_PDU_SUBTYPE_E eSubType)
{
    INT iRet = ERROR;

    if(NULL == pStPrivData)
    {
        AVTP_ERROR("param error\n");
        return iRet;
    }
    switch(eSubType)
    {
        case AVTP_PDU_SUBTYPE_CVF:
            iRet = avtp_pdu_init_sub_instance_cvf(pStPrivData);
            break;
        default:
            AVTP_ERROR("subtyp %d not support\n", eSubType);
            break;
    }
    return iRet;
}

 /**@fn	       SetSubType	  
 * @brief	   设置subtype
 * @param[in]  pIAvtpPdu   IAvtpPdu对象对象指针
 * @param[in]  eSubType avtp类型
 * @return	   成功返回OK  失败返回错误码
 */
static INT avtp_pdu_set_subtype(IAvtpPdu *pIAvtpPdu, AVTP_PDU_SUBTYPE_E eSubType)
{
    INT iRet = ERROR;
    AVTP_PDU_PRIV_DATA_T *pStPrivData = NULL;

    pStPrivData = avtp_pdu_get_priv_data(pIAvtpPdu);
    if(NULL == pStPrivData)
    {
        AVTP_ERROR("param error\n");
        return ERROR;
    }
    pStPrivData->eType = eSubType;
    iRet = avtp_pdu_init_sub_instance(pStPrivData, eSubType);
    if(iRet < 0)
    {
        AVTP_ERROR("avtp_pdu_init_sub_instance %d error\n", eSubType);
    }
    return iRet;
}

/**@fn	       GetSubType	  
 * @brief	   获取subtype
 * @param[in]  pIAvtpPdu   IAvtpPdu对象对象指针
 * @param[in]  pSubType avtp类型
 * @return	   成功返回OK  失败返回错误码
 */
static INT avtp_pdu_get_subtype(IAvtpPdu *pIAvtpPdu, AVTP_PDU_SUBTYPE_E *pSubType)
{
    AVTP_PDU_PRIV_DATA_T *pStPrivData = NULL;

    pStPrivData = avtp_pdu_get_priv_data(pIAvtpPdu);
    if(NULL == pStPrivData)
    {
        AVTP_ERROR("param error\n");
        return ERROR;
    }
    *pSubType = pStPrivData->eType;
    return OK;
}

/**@fn	       avtp_pdu_cvf_field_set	  
 * @brief	   设置avtpdu cvf域值
 * @param[in]  pStPrivData   私有数据指针
 * @param[in]  uFieldType  域类型, 如H.264 CVF见枚举AVTP_H264_CVF_FIELD_E定义
 * @param[in]  uVal  域值
 * @return	   成功返回OK  失败返回错误码
 */
static INT avtp_pdu_cvf_field_set(AVTP_PDU_PRIV_DATA_T *pStPrivData, UINT8 uFieldType, UINT64 uVal)
{
    INT iRet = ERROR;

    if((NULL == pStPrivData) || (NULL == pStPrivData->pCVFInstance))
    {
        AVTP_ERROR("param error\n");
        return iRet;
    }
    iRet = pStPrivData->pCVFInstance->SetField(pStPrivData->pCVFInstance, uFieldType, uVal);
    if(iRet < 0)
    {
        AVTP_ERROR("set filed %d vaule %llu error\n", uFieldType, uVal);
    }
    return iRet;
}

/**@fn	       avtp_pdu_field_set	  
 * @brief	   设置avtpdu域值
 * @param[in]  pIAvtpPdu   IAvtpPdu对象对象指针
 * @param[in]  uFieldType  域类型, 如H.264 CVF见枚举AVTP_H264_CVF_FIELD_E定义
 * @param[in]  uVal  域值
 * @return	   成功返回OK  失败返回错误码
 */
static INT avtp_pdu_field_set(IAvtpPdu *pIAvtpPdu, UINT8 uFieldType, UINT64 uVal)
{
    INT iRet = ERROR;
    AVTP_PDU_PRIV_DATA_T *pStPrivData = NULL;

    pStPrivData = avtp_pdu_get_priv_data(pIAvtpPdu);
    if(NULL == pStPrivData)
    {
        AVTP_ERROR("param error\n");
        return ERROR;
    }
    switch(pStPrivData->eType)
    {
        case AVTP_PDU_SUBTYPE_CVF:
            iRet = avtp_pdu_cvf_field_set(pStPrivData, uFieldType, uVal);
            break;
        default:
            AVTP_ERROR("subtyp %d not support\n", pStPrivData->eType);
            break;
    }
    return iRet;
}

/**@fn	       avtp_pdu_cvf_field_get	  
 * @brief	   获取avtpdu cvf域值
 * @param[in]  pStPrivData   私有数据指针
 * @param[in]  uFieldType  域类型, 如H.264 CVF见枚举AVTP_H264_CVF_FIELD_E定义
 * @param[out] pVal  域值
 * @return	   成功返回OK  失败返回错误码
 */
static INT avtp_pdu_cvf_field_get(AVTP_PDU_PRIV_DATA_T *pStPrivData, UINT8 uFieldType, UINT64 *pVal)
{
    INT iRet = ERROR;

    if((NULL == pStPrivData) || (NULL == pStPrivData->pCVFInstance))
    {
        AVTP_ERROR("param error\n");
        return iRet;
    }
    iRet = pStPrivData->pCVFInstance->GetField(pStPrivData->pCVFInstance, uFieldType, pVal);
    if(iRet < 0)
    {
        AVTP_ERROR("get filed %d error\n", uFieldType);
    }
    return iRet;
}

/**@fn	       avtp_pdu_field_get	  
 * @brief	   设置avtpdu域值
 * @param[in]  pIAvtpPdu   IAvtpPdu对象对象指针
 * @param[in]  uFieldType  域类型, 如H.264 CVF见枚举AVTP_H264_CVF_FIELD_E定义
 * @param[out] pVal  域值
 * @return	   成功返回OK  失败返回错误码
 */
static INT avtp_pdu_field_get(IAvtpPdu *pIAvtpPdu, UINT8 uFieldType, UINT64 *pVal)
{
    INT iRet = ERROR;
    AVTP_PDU_PRIV_DATA_T *pStPrivData = NULL;

    pStPrivData = avtp_pdu_get_priv_data(pIAvtpPdu);
    if(NULL == pStPrivData)
    {
        AVTP_ERROR("param error\n");
        return ERROR;
    }
    switch(pStPrivData->eType)
    {
        case AVTP_PDU_SUBTYPE_CVF:
            iRet = avtp_pdu_cvf_field_get(pStPrivData, uFieldType, pVal);
            break;
        default:
            AVTP_ERROR("subtyp %d not support\n", pStPrivData->eType);
            break;
    }
    return iRet;
}

/**@fn         avtp_pdu_cvf_payload_set
 * @brief      设置cvf payload数据
 * @param[in]  pStPrivData  私有数据指针
 * @param[in]  pPayloadData   payload数据
 * @param[in]  uDataLength payload数据长度
 * @return     成功返回OK  失败返回错误码
 */
static INT avtp_pdu_cvf_payload_set(AVTP_PDU_PRIV_DATA_T *pStPrivData, UINT8 *pPayloadData, UINT uDataLength)
{
    INT iRet = ERROR;

    if((NULL == pStPrivData) || (NULL == pStPrivData->pCVFInstance))
    {
        AVTP_ERROR("param error\n");
        return iRet;
    }
    iRet = pStPrivData->pCVFInstance->SetPayload(pStPrivData->pCVFInstance, pPayloadData, uDataLength);
    if(iRet < 0)
    {
        AVTP_ERROR("subtype %d, set payload error\n", pStPrivData->eType);
    }
    return iRet;
}

/**@fn         SetPayload
 * @brief      设置payload数据
 * @param[in]  pIAvtpPdu  IAvtpPdu对象操作指针
 * @param[in]  pPayloadData   payload数据
 * @param[in]  uDataLength payload数据长度
 * @return     成功返回OK  失败返回错误码
 */
static INT avtp_pdu_payload_set(IAvtpPdu *pIAvtpPdu, UINT8 *pPayloadData, UINT uDataLength)
{
    INT iRet = ERROR;
    AVTP_PDU_PRIV_DATA_T *pStPrivData = NULL;

    pStPrivData = avtp_pdu_get_priv_data(pIAvtpPdu);
    if(NULL == pStPrivData)
    {
        AVTP_ERROR("param error\n");
        return ERROR;
    }
    switch(pStPrivData->eType)
    {
        case AVTP_PDU_SUBTYPE_CVF:
            iRet = avtp_pdu_cvf_payload_set(pStPrivData, pPayloadData, uDataLength);
            break;
        default:
            AVTP_ERROR("subtyp %d not support\n", pStPrivData->eType);
            break;
    }
    return iRet;
}

/**@fn         avtp_pdu_cvf_payload_get
 * @brief      获取cvf payload数据
 * @param[in]  pStPrivData  私有数据指针
 * @param[in]  uLength   payload数据长度
 * @return     成功返回payload数据首地址，失败返回NULL
 */
static UINT8 *avtp_pdu_cvf_payload_get(AVTP_PDU_PRIV_DATA_T *pStPrivData, UINT *uLength)
{
    if((NULL == pStPrivData) || (NULL == pStPrivData->pCVFInstance))
    {
        AVTP_ERROR("param error\n");
        return NULL;
    }
    return pStPrivData->pCVFInstance->GetPayload(pStPrivData->pCVFInstance, uLength);
}

/**@fn         avtp_pdu_payload_get
 * @brief      获取payload数据
 * @param[in]  pIAvtpPdu  IAvtpPdu对象操作指针
 * @param[in]  uLength   payload数据长度
 * @return     成功返回payload数据首地址，失败返回NULL
 */
static UINT8 *avtp_pdu_payload_get(IAvtpPdu *pIAvtpPdu, UINT *uLength)
{
    AVTP_PDU_PRIV_DATA_T *pStPrivData = NULL;
    UINT8 *pPayLoad = NULL;

    pStPrivData = avtp_pdu_get_priv_data(pIAvtpPdu);
    if(NULL == pStPrivData)
    {
        AVTP_ERROR("param error\n");
        return NULL;
    }
    switch(pStPrivData->eType)
    {
        case AVTP_PDU_SUBTYPE_CVF:
            pPayLoad = avtp_pdu_cvf_payload_get(pStPrivData, uLength);
            break;
        default:
            AVTP_ERROR("subtyp %d not support\n", pStPrivData->eType);
            break;
    }
    if(NULL == pPayLoad)
    {
        AVTP_ERROR("get subtype %d payload failed\n", pStPrivData->eType);
    }
    return pPayLoad;
}

/**@fn         avtp_pdu_cvf_serial
 * @brief      cvf序列化
 * @param[in]  pStPrivData  私有数据指针
 * @param[in]  uLength     序列化后avtp包长度
 * @return     成功返回序列化后avtp包地址，失败返回NULL
 */
static UINT8 *avtp_pdu_cvf_serial(AVTP_PDU_PRIV_DATA_T *pStPrivData, UINT *uLength)
{
    if((NULL == pStPrivData) || (NULL == pStPrivData->pCVFInstance))
    {
        AVTP_ERROR("param error\n");
        return NULL;
    }
    return pStPrivData->pCVFInstance->Serial(pStPrivData->pCVFInstance, uLength);
}

/**@fn         avtp_pdu_serial
 * @brief      序列化
 * @param[in]  pIAvtpPdu  IAvtpPdu对象操作指针
 * @param[in]  uLength     序列化后avtp包长度
 * @return     成功返回序列化后avtp包地址，失败返回NULL
 */
static UINT8 *avtp_pdu_serial(IAvtpPdu *pIAvtpPdu, UINT *uLength)
{
    AVTP_PDU_PRIV_DATA_T *pStPrivData = NULL;
    UINT8 *pAvtpdu = NULL;

    pStPrivData = avtp_pdu_get_priv_data(pIAvtpPdu);
    if(NULL == pStPrivData)
    {
        AVTP_ERROR("param error\n");
        return NULL;
    }
    switch(pStPrivData->eType)
    {
        case AVTP_PDU_SUBTYPE_CVF:
            pAvtpdu = avtp_pdu_cvf_serial(pStPrivData, uLength);
            break;
        default:
            AVTP_ERROR("subtyp %d not support\n", pStPrivData->eType);
            break;
    }
    if(NULL == pAvtpdu)
    {
        AVTP_ERROR("serial subtype %d payload failed\n", pStPrivData->eType);
    }
    return pAvtpdu;
}

/**@fn         avtp_pdu_cvf_deserial
 * @brief      cvf 反序列化
 * @param[in]  pIAvtpPdu  IAvtpPdu对象操作指针
 * @param[in]  pAvtpData  avtp包数据
 * @param[in]  uAvtpDataLength    avtp包数据长度
 * @return     成功返回序列化长度   失败返回<0
 */
static INT avtp_pdu_cvf_deserial(AVTP_PDU_PRIV_DATA_T *pStPrivData, UINT8 *pAvtpData, UINT uAvtpDataLength)
{
    if((NULL == pStPrivData) || (NULL == pStPrivData->pCVFInstance) || (NULL == pAvtpData))
    {
        AVTP_ERROR("param error\n");
        return ERROR;
    }
    return pStPrivData->pCVFInstance->Deserial(pStPrivData->pCVFInstance, pAvtpData, uAvtpDataLength);
}

/**@fn         avtp_pdu_deserial
 * @brief      反序列化
 * @param[in]  pIAvtpPdu  IAvtpPdu对象操作指针
 * @param[in]  pAvtpData  avtp包数据
 * @param[in]  uAvtpDataLength    avtp包数据长度
 * @return     成功返回序列化长度   失败返回<0
 */
static INT avtp_pdu_deserial(IAvtpPdu *pIAvtpPdu, UINT8 *pAvtpData, UINT uAvtpDataLength)
{
    INT iRet = ERROR;
    UINT8 uSubType = 0;
    AVTP_PDU_PRIV_DATA_T *pStPrivData = NULL;

    pStPrivData = avtp_pdu_get_priv_data(pIAvtpPdu);
    if((NULL == pStPrivData) || (0u == uAvtpDataLength))
    {
        AVTP_ERROR("param error\n");
        return ERROR;
    }
    
    uSubType = pAvtpData[0];
    iRet = avtp_pdu_init_sub_instance(pStPrivData, (AVTP_PDU_SUBTYPE_E)uSubType);
    if(iRet < 0)
    {
        AVTP_ERROR("avtp_pdu_init_sub_instance %d error\n", uSubType);
        return iRet;
    }
    pStPrivData->eType = uSubType;
    switch(uSubType)
    {
        case AVTP_PDU_SUBTYPE_CVF:
            iRet = avtp_pdu_cvf_deserial(pStPrivData, pAvtpData, uAvtpDataLength);
            break;
        default:
            AVTP_ERROR("subtyp %d not support\n", pStPrivData->eType);
            iRet = ERROR;
            break;
    }
    return iRet;
}

/**@fn	       avtp_pdu_release	  
 * @brief	   RTP资源释放接口
 * @param[in]  pIAvtpPdu   IAvtpPdu对象对象接口
 * @return	   VOID
 */
static VOID avtp_pdu_release(IAvtpPdu *pIAvtpPdu)
{
    AVTP_PDU_PRIV_DATA_T *pStPrivData = NULL;

    pStPrivData = avtp_pdu_get_priv_data(pIAvtpPdu);
    if(NULL == pStPrivData)
    {
        AVTP_ERROR("param error\n");
        return;
    }

    if(NULL != pStPrivData->pCVFInstance)
    {
        pStPrivData->pCVFInstance->Release(pStPrivData->pCVFInstance);
        pStPrivData->pCVFInstance = NULL;
    }
    sys_mem_free(pIAvtpPdu);
    return;
}

/**@fn         avtp_pdu_init_interface
 * @brief      初始化构造对外接口
 * @param[in]  pIAvtpPdu  IAvtpPdu对象接口
 * @return     成功返回OK  失败返回错误码
 */
static INT avtp_pdu_init_interface(IAvtpPdu *pIAvtpPdu)
{
    if(NULL == pIAvtpPdu)
    {
        AVTP_ERROR("param error\n");
        return ERROR;
    }
    
    pIAvtpPdu->SetSubType = avtp_pdu_set_subtype;
    pIAvtpPdu->GetSubType = avtp_pdu_get_subtype;
    pIAvtpPdu->SetField = avtp_pdu_field_set;
    pIAvtpPdu->GetField = avtp_pdu_field_get;
    pIAvtpPdu->SetPayload = avtp_pdu_payload_set;
    pIAvtpPdu->GetPayload = avtp_pdu_payload_get;
    pIAvtpPdu->Serial = avtp_pdu_serial;
    pIAvtpPdu->Deserial = avtp_pdu_deserial;
    pIAvtpPdu->Release = avtp_pdu_release;
    return OK;
}

/**@fn         avtp_pdu_init_priv_data
 * @brief      AVTPDU私有数据初始化
 * @param[in]   pStPrivData  私有数据结构指针
 * @return     成功返回OK  失败返回错误码
 */
static INT avtp_pdu_init_priv_data(AVTP_PDU_PRIV_DATA_T *pStPrivData, const UINT uPduLength)
{
    INT iRet = ERROR;
    VOID *pMemset = NULL;
    if(NULL == pStPrivData)
    {
        AVTP_ERROR("param error\n");
        return iRet;
    }
    pMemset = memset(pStPrivData, 0, sizeof(AVTP_PDU_PRIV_DATA_T));
    if(NULL == pMemset)
    {
        AVTP_ERROR("memset error\n");
        return iRet;
    }
    pStPrivData->uPduLength = uPduLength;
    return OK;
}

/**@fn	       avtp_cvf_pdu_init_instance	  
 * @brief	   初始化构造单例
 * @param[in]  uPduLength pdu长度
 * @return	   成功返回单例  失败返回NULL
 */
IAvtpPdu *avtp_pdu_init_instance(const UINT uPduLength)
{
    INT iRet = ERROR;
    AVTP_PDU_BASE_T *pStBase = NULL;
    VOID *pMemset = NULL;

    pStBase = sys_mem_malloc(sizeof(AVTP_PDU_BASE_T));
    if(NULL == pStBase)
    {
        LOGGER_ERROR("sys_mem_malloc failed\n");
        return NULL;
    }
    pMemset = memset(pStBase,0,sizeof(AVTP_PDU_BASE_T));
    if(NULL == pMemset)
    {
        LOGGER_ERROR("memset error\n");
        return NULL;
    }
    
    iRet = avtp_pdu_init_interface(&pStBase->stInterface);
    if(iRet < 0)
    {   
        AVTP_ERROR("avtp_pdu_init_interface failed\n");
        sys_mem_free(pStBase);
        return NULL;
    }

    iRet = avtp_pdu_init_priv_data(&pStBase->stPrivData, uPduLength);
    if(iRet < 0)
    {
        AVTP_ERROR("avtp_pdu_init_priv_data failed\n");
        sys_mem_free(pStBase);
        return NULL;
    }
    return &pStBase->stInterface;
}