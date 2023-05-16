/* 
 * @file    avtp_cvf_pdu.c
 * @note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @brief   avtp Compressed Video Format PDU
 * 
 * @author  chen
 * @date    2022-7-15
 * @version v1.0.0
 */

#include "avtp_cvf_pdu_priv.h"
#include "avtp_cvf.h"
#include "avtp_common.h"

/**@fn         avtp_cvf_pdu_get_priv_data
 * @brief      获取私有信息指针
 * @param[in]  pIAvtpCvfPdu  AVTPDU指针
 * @return    成功返回AVTP_PRIV_DATA_T类型指针,失败返回NULL
 */
static AVTP_CVF_PDU_PRIV_DATA_T *avtp_cvf_pdu_get_priv_data(IAvtpCvfPdu *pIAvtpCvfPdu)
{
    AVTP_CVF_PDU_BASE_T *pStBase = NULL;

    pStBase = (AVTP_CVF_PDU_BASE_T*)pIAvtpCvfPdu;
    if(NULL == pStBase)
    {
        AVTP_ERROR("param error\n");
        return NULL;
    }
    return &pStBase->stPrivData;
}

/**@fn	       avtp_cvf_pdu_filed_init	  
 * @brief	   初始化
 * @param[in]  pStPrivData 私有数据指针
 * @return	   成功返回OK  失败返回错误码
 */
static INT32 avtp_cvf_pdu_filed_init(AVTP_CVF_PDU_PRIV_DATA_T *pStPrivData)
{
    INT32 iRet = ERROR;
    AVTP_COMMON_PDU_T *pStCommHeader = NULL;
    AVTP_STREAM_PDU_T *pStStreamPdu = NULL;

    if(NULL == pStPrivData)
    {
        return iRet;
    }
    //AVTPDU common header field
    pStCommHeader = (AVTP_COMMON_PDU_T *)pStPrivData->pAvtpPduBuf;
    if(NULL == pStCommHeader)
    {
        AVTP_ERROR("ptr is NULL\n");
        return ERROR;
    }
    iRet = avtp_common_pdu_set(pStCommHeader, AVTP_FIELD_SUBTYPE, AVTP_SUBTYPE_CVF);
    if(iRet < 0)
    {
        AVTP_ERROR("avtp_common_pdu_set subtype failed\n");
        return ERROR;
    }
    iRet = avtp_common_pdu_set(pStCommHeader, AVTP_FIELD_VERSION, 0);
    if(iRet < 0)
    {
        AVTP_ERROR("avtp_common_pdu_set version failed\n");
        return ERROR;
    }
    //StreamPdu
    pStStreamPdu = (AVTP_STREAM_PDU_T *)pStPrivData->pAvtpPduBuf;
    if(NULL == pStStreamPdu)
    {
        AVTP_ERROR("ptr is NULL\n");
        return ERROR;
    }
    //format subtype
    iRet = avtp_cvf_pdu_filed_set(pStStreamPdu, AVTP_CVF_FIELD_FORMAT, AVTP_CVF_FORMAT_RFC);
    if(iRet < 0)
    {
        AVTP_ERROR("avtp_cvf_pdu_filed_set failed\n");
        return ERROR;
    }
    return OK;
}

/**@fn	       avtp_cvf_pdu_init	  
 * @brief	   初始化
 * @param[in]  pIAvtpCvfPdu   IAvtpCvfPdu对象对象指针
 * @param[in]  uMaxPayloadLen  payload最大长度
 * @return	   成功返回OK  失败返回错误码
 */
static INT32 avtp_cvf_pdu_init(IAvtpCvfPdu *pIAvtpCvfPdu, UINT32 uPduLength)
{
    INT32 iRet = ERROR;  
    AVTP_CVF_PDU_PRIV_DATA_T *pStPrivData = NULL;

    pStPrivData = avtp_cvf_pdu_get_priv_data(pIAvtpCvfPdu);
    if((NULL == pStPrivData) || (0u == uPduLength))
    {
        AVTP_ERROR("param error\n");
        return iRet;
    }
    if(AVTP_HEADER_LEN > uPduLength)
    {
        AVTP_ERROR("init pdu pack length %d too small\n", uPduLength);
        return ERROR;
    }
    //avtp头长度+payload头长度+payload数据长度
    //cvf video payload头最多2个32位
    pStPrivData->uPduLengthMax = AVTP_CVF_HEADER_LEN + (2u*sizeof(UINT32)) + uPduLength;
    pStPrivData->pAvtpPduBuf = sys_mem_malloc(pStPrivData->uPduLengthMax);
    if(NULL == pStPrivData->pAvtpPduBuf)
    {
        AVTP_ERROR("sys_mem_malloc failed\n");
        return ERROR;
    }

    iRet = avtp_cvf_pdu_filed_init(pStPrivData);
    if(iRet < 0)
    {
        AVTP_ERROR("avtp_cvf_pdu_filed_init failed\n");
    }
    return iRet;
}

/**@fn	       avtp_cvf_pdu_payload_header_len	  
 * @brief	   获取avtp payload头长度
 * @param[in]  uFormatSubtype   类型
 * @return	   成功返回头长度  失败返回错误码
 */
static INT32 avtp_cvf_pdu_payload_header_len(UINT8 uFormatSubtype)
{
    INT32 iRet = ERROR;

    switch(uFormatSubtype)
    {
        case AVTP_CVF_FORMAT_SUBTYPE_H264: /*标准RFC MJPEG*/
            iRet = AVTP_CVF_PAYLOAD_H264_HEADER_LEN;
            break;
        default:
            AVTP_ERROR("format subtype %d not support\n", uFormatSubtype);
            break;
    }
    return iRet;
}

/**@fn	       avtp_cvf_pdu_field_set	  
 * @brief	   设置avtpdu域值
 * @param[in]  pIAvtpCvfPdu   IAvtpCvfPdu对象对象指针
 * @param[in]  uFieldType  域类型, 如H.264 CVF见枚举AVTP_H264_CVF_FIELD_E定义
 * @param[in]  uVal  域值
 * @return	   成功返回OK  失败返回错误码
 */
static INT32 avtp_cvf_pdu_field_set(IAvtpCvfPdu *pIAvtpCvfPdu, UINT8 uFieldType, UINT64 uVal)
{
    INT32 iRet = ERROR;
    UINT64 uTmpVal = 0;
    AVTP_COMMON_PDU_T *pStCommonPdu = NULL;
    AVTP_STREAM_PDU_T *pStStreamPdu = NULL;
    AVTP_CVF_PDU_PRIV_DATA_T *pStPrivData = NULL;

    pStPrivData = avtp_cvf_pdu_get_priv_data(pIAvtpCvfPdu);
    if(NULL == pStPrivData)
    {
        AVTP_ERROR("param error\n");
        return iRet;
    }
    
    uTmpVal = uVal;
    if(AVTP_CVF_FIELD_VERSION == (AVTP_CVF_FIELD_E)uFieldType)
    {
        pStCommonPdu = (AVTP_COMMON_PDU_T *)pStPrivData->pAvtpPduBuf;
        if(NULL == pStCommonPdu)
        {
            AVTP_ERROR("ptr is NULL\n");
            return ERROR;
        }
        iRet = avtp_common_pdu_set(pStCommonPdu, uFieldType, (UINT32)uVal);
    }
    else
    {
        pStStreamPdu = (AVTP_STREAM_PDU_T *)pStPrivData->pAvtpPduBuf;
        if(NULL == pStStreamPdu)
        {
            AVTP_ERROR("ptr is NULL\n");
            return ERROR;
        }
        if(AVTP_CVF_FIELD_FORMAT_SUBTYPE == (AVTP_CVF_FIELD_E)uFieldType)
        {
            pStPrivData->uFormatSubtype = (UINT8)uVal;
        }
        else if(AVTP_CVF_FIELD_STREAM_DATA_LEN == (AVTP_CVF_FIELD_E)uFieldType)
        {
            //uFieldType类型AVTP_CVF_FIELD_STREAM_DATA_LEN，需要加上头长度。
            //如：h.264时包括h.264 Header+H.264Data，即加上h.264 Header
            pStPrivData->uPduPayloadDataLen = uVal;
            iRet = avtp_cvf_pdu_payload_header_len(pStPrivData->uFormatSubtype);
            if(iRet < 0)
            {
                AVTP_ERROR("avtp_cvf_pdu_payload_header_len failed\n");
                return iRet;
            }
            //Data头长度+Data内容长度
            uTmpVal += (UINT32)iRet;
        }
        else
        {
            //do nothing
        }
        iRet = avtp_cvf_pdu_filed_set(pStStreamPdu, uFieldType, uTmpVal);
    }
    if(iRet < 0)
    {
        AVTP_ERROR("set field %d value %llu failed\n", uFieldType, uTmpVal);
    }
    return iRet;
}

/**@fn	       avtp_cvf_pdu_field_get	  
 * @brief	   设置avtpdu域值
 * @param[in]  pIAvtpCvfPdu   IAvtpCvfPdu对象对象指针
 * @param[in]  uFieldType  域类型, 如H.264 CVF见枚举AVTP_H264_CVF_FIELD_E定义
 * @param[out] pVal  域值
 * @return	   成功返回OK  失败返回错误码
 */
static INT32 avtp_cvf_pdu_field_get(IAvtpCvfPdu *pIAvtpCvfPdu, UINT8 uFieldType, UINT64 *pVal)
{
    INT32 iRet = ERROR;
    AVTP_COMMON_PDU_T *pStCommonPdu = NULL;
    AVTP_STREAM_PDU_T *pStStreamPdu = NULL;
    AVTP_CVF_PDU_PRIV_DATA_T *pStPrivData = NULL;

    pStPrivData = avtp_cvf_pdu_get_priv_data(pIAvtpCvfPdu);
    if((NULL == pStPrivData) || (NULL == pVal))
    {
        AVTP_ERROR("param error\n");
        return iRet;
    }
    
    if(AVTP_CVF_FIELD_VERSION == (AVTP_CVF_FIELD_E)uFieldType)
    {
        pStCommonPdu = (AVTP_COMMON_PDU_T *)pStPrivData->pAvtpPduBuf;
        if(NULL == pStCommonPdu)
        {
            AVTP_ERROR("ptr is NULL\n");
            return ERROR;
        }
        iRet = avtp_common_pdu_get(pStCommonPdu, uFieldType, (UINT32*)pVal);
    }
    else
    {
        pStStreamPdu = (AVTP_STREAM_PDU_T *)pStPrivData->pAvtpPduBuf;
        if(NULL == pStStreamPdu)
        {
            AVTP_ERROR("ptr is NULL\n");
            return ERROR;
        }
        iRet = avtp_cvf_pdu_filed_get(pStStreamPdu, uFieldType, pVal);
    }
    if(iRet < 0)
    {
        AVTP_ERROR("get filed %d value failed\n", uFieldType);
    }
    return iRet;
}

/**@fn         avtp_cvf_pdu_payload_set
 * @brief      设置payload数据
 * @param[in]  pIAvtpCvfPdu  IAvtpCvfPdu对象操作指针
 * @param[in]  pPayloadData   payload数据
 * @param[in]  uDataLength payload数据长度
 * @return     成功返回OK  失败返回错误码
 */
static INT32 avtp_cvf_pdu_payload_set(IAvtpCvfPdu *pIAvtpCvfPdu, UINT8 *pPayloadData, UINT32 uDataLength)
{
    INT32 iRet = ERROR;
    AVTP_STREAM_PDU_T *pStStreamPdu = NULL;
    AVTP_CVF_PDU_H264_PAYLOAD_T *pStPayload = NULL;
    AVTP_CVF_PDU_PRIV_DATA_T *pStPrivData = NULL;
    INT32 iPduLength = 0;
    VOID *pDst = NULL;

    pStPrivData = avtp_cvf_pdu_get_priv_data(pIAvtpCvfPdu);
    if((NULL == pStPrivData) || (NULL == pPayloadData))
    {
        AVTP_ERROR("param error\n");
        return iRet;
    }
    pStStreamPdu = (AVTP_STREAM_PDU_T *)pStPrivData->pAvtpPduBuf;
    if(NULL == pStStreamPdu)
    {
        AVTP_ERROR("ptr is NULL\n");
        return iRet;
    }
    pStPayload  = (AVTP_CVF_PDU_H264_PAYLOAD_T *)pStStreamPdu->uAvtpPayload;
    if(NULL == pStPayload)
    {
        AVTP_ERROR("ptr is NULL\n");
        return iRet;
    }
    iRet = avtp_cvf_pdu_payload_header_len(pStPrivData->uFormatSubtype);
    if(iRet < 0)
    {
        AVTP_ERROR("get payload header error\n");
        return iRet;
    }
    iPduLength += iRet; //payload header
    iPduLength += (INT32)uDataLength; //payload data
    iRet = avtp_cvf_pdu_filed_set(pStStreamPdu, AVTP_CVF_FIELD_STREAM_DATA_LEN, iPduLength);
    if(iRet < 0)
    {
        AVTP_ERROR("avtp_cvf_pdu_filed_set error\n");
        return iRet;
    }

    iPduLength += (INT32)AVTP_CVF_HEADER_LEN; //avtp header
    if(pStPrivData->uPduLengthMax < (UINT32)iPduLength)
    {
        AVTP_ERROR("buf not enough %d %d\n", pStPrivData->uPduLengthMax, iPduLength);
        return ERROR;
    }
    pStPrivData->uPduPayloadDataLen = uDataLength;
    pDst = memcpy(pStPayload->uH264Data, pPayloadData, pStPrivData->uPduPayloadDataLen);
    if(NULL == pDst)
    {
        AVTP_ERROR("memcpy error\n");
        return ERROR;
    }
    return OK;
}

/**@fn         avtp_cvf_pdu_payload_get
 * @brief      获取payload数据
 * @param[in]  pIAvtpCvfPdu  IAvtpCvfPdu对象操作指针
 * @param[in]  uLength   payload数据长度
 * @return     成功返回payload数据首地址，失败返回NULL
 */
static UINT8 *avtp_cvf_pdu_payload_get(IAvtpCvfPdu *pIAvtpCvfPdu, UINT32 *uLength)
{
    INT32 iRet = ERROR;
    AVTP_STREAM_PDU_T *pStPdu = NULL;
    AVTP_CVF_PDU_H264_PAYLOAD_T *pStPayload = NULL;
    UINT64 uDataLen = 0;
    AVTP_CVF_PDU_PRIV_DATA_T *pStPrivData = NULL;
    UINT8 *pPayloadData = NULL;

    pStPrivData = avtp_cvf_pdu_get_priv_data(pIAvtpCvfPdu);
    if(NULL == pStPrivData)
    {
        AVTP_ERROR("param error\n");
        return NULL;
    }
    pStPdu = (AVTP_STREAM_PDU_T *)pStPrivData->pAvtpPduBuf;
    if(NULL == pStPdu)
    {
        AVTP_ERROR("ptr is NULL\n");
        return NULL;
    }
    iRet = avtp_cvf_pdu_filed_get(pStPdu, AVTP_CVF_FIELD_STREAM_DATA_LEN, &uDataLen);
    if(iRet < 0)
    {
        AVTP_ERROR("avtp_cvf_pdu_filed_get error\n");
        return NULL;
    }
    pStPayload  = (AVTP_CVF_PDU_H264_PAYLOAD_T*)pStPdu->uAvtpPayload;
    if(NULL == pStPayload)
    {
        AVTP_ERROR("ptr is NULL\n");
        return NULL;
    }
    iRet = avtp_cvf_pdu_payload_header_len(pStPrivData->uFormatSubtype);
    if(iRet < 0)
    {
        AVTP_ERROR("get payload header error\n");
        return NULL;
    }
    uDataLen -= (UINT32)iRet;
    pPayloadData = pStPayload->uH264Data;
    *uLength = uDataLen;
    return pPayloadData;
}

/**@fn         avtp_cvf_pdu_serial
 * @brief      序列化
 * @param[in]  pIAvtpCvfPdu  IAvtpCvfPdu对象操作指针
 * @param[in]  uLength     序列化后avtp包长度
 * @return     成功返回序列化后avtp包地址，失败返回NULL
 */
static UINT8 *avtp_cvf_pdu_serial(IAvtpCvfPdu *pIAvtpCvfPdu, UINT32 *uLength)
{
    INT32 iRet = 0;
    AVTP_CVF_PDU_PRIV_DATA_T *pStPrivData = NULL;

    pStPrivData = avtp_cvf_pdu_get_priv_data(pIAvtpCvfPdu);
    if(NULL == pStPrivData)
    {
        AVTP_ERROR("param error\n");
        return NULL;
    }
    iRet = avtp_cvf_pdu_payload_header_len(pStPrivData->uFormatSubtype);
    if(iRet < 0)
    {
        AVTP_ERROR("get payload header error\n");
        return NULL;
    }
    *uLength = AVTP_CVF_HEADER_LEN + (UINT32)iRet + pStPrivData->uPduPayloadDataLen;
    return pStPrivData->pAvtpPduBuf;
}

/**@fn         avtp_cvf_pdu_deserial
* @brief      反序列化
* @param[in]  pIAvtpCvfPdu  IAvtpCvfPdu对象操作指针
* @param[in]  pAvtpData  avtp包数据
* @param[in]  uAvtpDataLength    avtp包数据长度
* @return     成功返回OK  失败返回错误码
*/
static INT32 avtp_cvf_pdu_deserial(IAvtpCvfPdu *pIAvtpCvfPdu, UINT8 *pAvtpData, UINT32 uAvtpDataLength)
{
    INT32 iRet = ERROR;
    UINT64 uValue = 0;
    UINT16 uStreamDataLen = 0;
    AVTP_STREAM_PDU_T *pStStreamPdu = NULL;
    AVTP_CVF_PDU_PRIV_DATA_T *pStPrivData = NULL;

    pStPrivData = avtp_cvf_pdu_get_priv_data(pIAvtpCvfPdu);
    if((NULL == pStPrivData) || (NULL == pAvtpData))
    {
        AVTP_ERROR("param error\n");
        return iRet;
    }
    pStPrivData->bDeserialFlag = TRUE;
    pStPrivData->pAvtpPduBuf = pAvtpData;
    if(uAvtpDataLength > pStPrivData->uPduLengthMax)
    { 
        AVTP_ERROR("avtp packet length bigger than pdu buff length\n");
        return iRet;
    }
    //avtp header
    if(uAvtpDataLength < AVTP_CVF_HEADER_LEN)
    {
        AVTP_ERROR("avtp packet length %d too small\n", uAvtpDataLength);
        return iRet;
    }    
    pStStreamPdu = (AVTP_STREAM_PDU_T *)pStPrivData->pAvtpPduBuf;
    if(NULL == pStStreamPdu)
    {
        AVTP_ERROR("ptr is NULL\n");
        return ERROR;
    }
    //format subtype
    iRet = avtp_cvf_pdu_filed_get(pStStreamPdu, AVTP_CVF_FIELD_FORMAT_SUBTYPE, &uValue);
    if(iRet < 0)
    {
        AVTP_ERROR("avtp_cvf_pdu_filed_get error\n");
        return iRet;
    }
    pStPrivData->uFormatSubtype = (UINT8)uValue;

    //stream data length
    iRet = avtp_cvf_pdu_filed_get(pStStreamPdu, AVTP_CVF_FIELD_STREAM_DATA_LEN, &uValue);
    if(iRet < 0)
    {
        AVTP_ERROR("avtp_cvf_pdu_filed_get error\n");
        return iRet;
    }

    uStreamDataLen = (UINT16)uValue;
    if(uAvtpDataLength < (AVTP_CVF_HEADER_LEN + uStreamDataLen))
    {
        AVTP_ERROR("avtp packet length %d too small %d \n", uAvtpDataLength, uStreamDataLen);
        return iRet;
    }
    return AVTP_CVF_HEADER_LEN+uStreamDataLen;
}

/**@fn	       avtp_cvf_pdu_release	  
 * @brief	   RTP资源释放接口
 * @param[in]  pIAvtpCvfPdu   IAvtpCvfPdu对象对象接口
 * @return	   VOID
 */
static VOID avtp_cvf_pdu_release(IAvtpCvfPdu *pIAvtpCvfPdu)
{
    AVTP_CVF_PDU_PRIV_DATA_T *pStPrivData = NULL;

    pStPrivData = avtp_cvf_pdu_get_priv_data(pIAvtpCvfPdu);
    if(NULL == pStPrivData)
    {
        AVTP_ERROR("param error\n");
        return;
    }
    if((NULL != pStPrivData->pAvtpPduBuf)
        && (!pStPrivData->bDeserialFlag))
    {
        sys_mem_free(pStPrivData->pAvtpPduBuf);
        pStPrivData->pAvtpPduBuf = NULL;
    }
    sys_mem_free(pIAvtpCvfPdu);
    return;
}

/**@fn         avtp_pdu_init_interface
 * @brief      初始化构造对外接口
 * @param[in]  pIAvtpCvfPdu  IAvtpCvfPdu对象接口
 * @return     成功返回OK  失败返回错误码
 */
static INT32 avtp_cvf_pdu_init_interface(IAvtpCvfPdu *pIAvtpCvfPdu)
{
    if(NULL == pIAvtpCvfPdu)
    {
        AVTP_ERROR("param error\n");
        return ERROR;
    }
    pIAvtpCvfPdu->Init = avtp_cvf_pdu_init;
    pIAvtpCvfPdu->SetField = avtp_cvf_pdu_field_set;
    pIAvtpCvfPdu->GetField = avtp_cvf_pdu_field_get;
    pIAvtpCvfPdu->SetPayload = avtp_cvf_pdu_payload_set;
    pIAvtpCvfPdu->GetPayload = avtp_cvf_pdu_payload_get;
    pIAvtpCvfPdu->Serial = avtp_cvf_pdu_serial;
    pIAvtpCvfPdu->Deserial = avtp_cvf_pdu_deserial;
    pIAvtpCvfPdu->Release = avtp_cvf_pdu_release;
    return OK;
}

/**@fn         avtp_pdu_init_priv_data
 * @brief      AVTPDU私有数据初始化
 * @param[in]   pStPrivData  私有数据结构指针
 * @return     成功返回OK  失败返回错误码
 */
static INT32 avtp_cvf_pdu_init_priv_data(AVTP_CVF_PDU_PRIV_DATA_T *pStPrivData)
{
    INT32 iRet = ERROR;
    VOID *pMemset = NULL;

    if(NULL == pStPrivData)
    {
        AVTP_ERROR("param error\n");
        return iRet;
    }
    pMemset = memset(pStPrivData, 0, sizeof(AVTP_CVF_PDU_PRIV_DATA_T));
    if(NULL == pMemset)
    {
        AVTP_ERROR("memset error\n");
        return iRet;
    }
    return OK;
}

/**@fn	       avtp_cvf_pdu_init_instance	  
 * @brief	   初始化构造单例
 * @param[in]  pIPduAvtpHost 创建主函数指针集
 * @return	   成功返回单例  失败返回NULL
 */
IAvtpCvfPdu *avtp_cvf_pdu_init_instance(VOID)
{
    INT32 iRet = ERROR;
    AVTP_CVF_PDU_BASE_T *pStBase = NULL;
    VOID *pMemset = NULL;

    pStBase = sys_mem_malloc(sizeof(AVTP_CVF_PDU_BASE_T));
    if(NULL == pStBase)
    {
        LOGGER_ERROR("sys_mem_malloc failed\n");
        return NULL;
    }
    pMemset = memset(pStBase,0,sizeof(AVTP_CVF_PDU_BASE_T));
    if(NULL == pMemset)
    {
       LOGGER_ERROR("memset error\n");
        return NULL;
    }
    iRet = avtp_cvf_pdu_init_interface(&pStBase->stInterface);
    if(iRet < 0)
    {   
        LOGGER_ERROR("avtp_init_interface failed\n");
        sys_mem_free(pStBase);
        return NULL;
    }

    iRet = avtp_cvf_pdu_init_priv_data(&pStBase->stPrivData);
    if(iRet < 0)
    {
        LOGGER_ERROR("avtp_init_priv_data failed\n");
        sys_mem_free(pStBase);
        return NULL;
    }
    return &pStBase->stInterface;
}

