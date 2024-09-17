/* 
 * @file    avtp_cvf.c
 * @note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @brief   avtp cvf格式封装及解封实现         
 * @author  chen
 * @date    2022-7-21
 * @version v1.0.0 
 */
#include "avtp_cvf.h"
#include "avtp_common.h"
#include "avtp_util.h"
#include "avtp_internal.h"

#define SHIFT_FORMAT		    (31 - 7)
#define SHIFT_FORMAT_SUBTYPE	(31 - 15)
#define SHIFT_M			(31 - 19)
#define SHIFT_EVT		(31 - 23)
#define SHIFT_PTV		(31 - 18)

#define MASK_FORMAT		(BITMASK(8) << SHIFT_FORMAT)
#define MASK_FORMAT_SUBTYPE	(BITMASK(8) << SHIFT_FORMAT_SUBTYPE)
#define MASK_M			(BITMASK(1) << SHIFT_M)
#define MASK_EVT		(BITMASK(4) << SHIFT_EVT)
#define MASK_PTV		(BITMASK(1) << SHIFT_PTV)

/**@fn	       avtp_cvf_get_field_value	  
 * @brief	   获取cvf格式特定的filed
 * @param[in]  pAvtpStreamPdu  pdu结构指针
 * @param[in]  eField  检索的pdu字段
 * @param[in]  uVal  保存检索pdu字段值的变量的指针
 * @return	   成功返回OK  失败返回错误码
 */
static INT avtp_cvf_get_field_value(const AVTP_STREAM_PDU_T *pPduHeader, AVTP_CVF_FIELD_E eField, UINT64 *uVal)
{
    UINT uBitmap = 0;
    UINT uMask = 0;
    UINT8 uShift = 0;

	if(NULL == pPduHeader)
	{
        AVTP_ERROR("param error\n");
		return ERROR;
	}
    switch (eField) 
	{
        case AVTP_CVF_FIELD_FORMAT:
            uMask = MASK_FORMAT;
            uShift = SHIFT_FORMAT;
            uBitmap = ntohl(pPduHeader->uFormatSpecific);
            break;
        case AVTP_CVF_FIELD_FORMAT_SUBTYPE:
            uMask = MASK_FORMAT_SUBTYPE;
            uShift = SHIFT_FORMAT_SUBTYPE;
            uBitmap = ntohl(pPduHeader->uFormatSpecific);
            break;
        case AVTP_CVF_FIELD_M:
            uMask = MASK_M;
            uShift = SHIFT_M;
            uBitmap = ntohl(pPduHeader->uPacketInfo);
            break;
        case AVTP_CVF_FIELD_EVT:
            uMask = MASK_EVT;
            uShift = SHIFT_EVT;
            uBitmap = ntohl(pPduHeader->uPacketInfo);
            break;
	    case AVTP_CVF_FIELD_H264_PTV:
            uMask = MASK_PTV;
            uShift = SHIFT_PTV;
            uBitmap = ntohl(pPduHeader->uPacketInfo);
            break;
        default:
            AVTP_ERROR("field %d not support\n", eField);
            return ERROR;
	}

	*uVal = BITMAP_GET_VALUE(uBitmap, uMask, uShift);
	return OK;
}

/**@fn	       avtp_cvf_set_field_value	  
 * @brief	   设置cvf格式特定的filed
 * @param[in]  pAvtpStreamPdu  pdu结构指针
 * @param[in]  eField  检索的pdu字段
 * @param[in]  uVal  值
 * @return	   成功返回OK  失败返回错误码
 */
static INT avtp_cvf_set_field_value(AVTP_STREAM_PDU_T *pAvtpStreamPdu, AVTP_CVF_FIELD_E eField, UINT uVal)
{
    UINT uBitmap = 0;
    UINT uMask = 0;
    UINT8 uShift = 0;
	void *ptr;

    if(NULL == pAvtpStreamPdu)
    {
        AVTP_ERROR("param error\n");
        return ERROR;
    }
    switch (eField) 
    {
        case AVTP_CVF_FIELD_FORMAT:
            uMask = MASK_FORMAT;
            uShift = SHIFT_FORMAT;
            ptr = &pAvtpStreamPdu->uFormatSpecific;
            break;
        case AVTP_CVF_FIELD_FORMAT_SUBTYPE:
            uMask = MASK_FORMAT_SUBTYPE;
            uShift = SHIFT_FORMAT_SUBTYPE;
            ptr = &pAvtpStreamPdu->uFormatSpecific;
            break;
        case AVTP_CVF_FIELD_M:
            uMask = MASK_M;
            uShift = SHIFT_M;
            ptr = &pAvtpStreamPdu->uPacketInfo;
            break;
        case AVTP_CVF_FIELD_EVT:
            uMask = MASK_EVT;
            uShift = SHIFT_EVT;
            ptr = &pAvtpStreamPdu->uPacketInfo;
            break;
        case AVTP_CVF_FIELD_H264_PTV:
            uMask = MASK_PTV;
            uShift = SHIFT_PTV;
            ptr = &pAvtpStreamPdu->uPacketInfo;
            break;
        default:
            AVTP_ERROR("field %d not support\n", eField);
            return ERROR;
    }
    uBitmap = get_unaligned_be32(ptr);
    BITMAP_SET_VALUE(uBitmap, uVal, uMask, uShift);
    put_unaligned_be32(uBitmap, ptr);
    return OK;
}

/**@fn	       avtp_cvf_pdu_filed_get	  
 * @brief	   获取CVF avtpdu filed的值
 * @param[in]  pAvtpStreamPdu  pdu结构指针
 * @param[in]  eField  检索的pdu字段
 * @param[in]  pVal  保存检索pdu字段值的变量的指针
 * @return	   成功返回OK  失败返回错误码
 */
INT avtp_cvf_pdu_filed_get(const AVTP_STREAM_PDU_T *pAvtpStreamPdu, AVTP_CVF_FIELD_E eField, UINT64 *pVal)
{
    INT iRet = ERROR;
    AVTP_CVF_PDU_H264_PAYLOAD_T *pH264Payload = NULL;
    if ((NULL == pAvtpStreamPdu) || (NULL == pVal))
    {
        AVTP_ERROR("param error\n");
        return iRet;	
    }
    switch (eField) 
    {
        case AVTP_CVF_FIELD_SV:
            iRet = avtp_common_stream_pdu_get(pAvtpStreamPdu, AVTP_STREAM_FIELD_SV, pVal);
            break;
        case AVTP_CVF_FIELD_MR:
            iRet = avtp_common_stream_pdu_get(pAvtpStreamPdu, AVTP_STREAM_FIELD_MR, pVal);
            break;
        case AVTP_CVF_FIELD_TV:
            iRet = avtp_common_stream_pdu_get(pAvtpStreamPdu, AVTP_STREAM_FIELD_TV, pVal);
            break;
        case AVTP_CVF_FIELD_SEQ_NUM:
            iRet = avtp_common_stream_pdu_get(pAvtpStreamPdu, AVTP_STREAM_FIELD_SEQ_NUM, pVal);
            break;
        case AVTP_CVF_FIELD_TU:
            iRet = avtp_common_stream_pdu_get(pAvtpStreamPdu, AVTP_STREAM_FIELD_TU, pVal);
            break;
        case AVTP_CVF_FIELD_STREAM_DATA_LEN:
            iRet = avtp_common_stream_pdu_get(pAvtpStreamPdu, AVTP_STREAM_FIELD_STREAM_DATA_LEN, pVal);
            break;
        case AVTP_CVF_FIELD_TIMESTAMP:
            iRet = avtp_common_stream_pdu_get(pAvtpStreamPdu, AVTP_STREAM_FIELD_TIMESTAMP, pVal);
            break;
        case AVTP_CVF_FIELD_STREAM_ID:
            iRet = avtp_common_stream_pdu_get(pAvtpStreamPdu, AVTP_STREAM_FIELD_STREAM_ID, pVal);
		    break;
        case AVTP_CVF_FIELD_FORMAT:
        case AVTP_CVF_FIELD_FORMAT_SUBTYPE:
        case AVTP_CVF_FIELD_M:
        case AVTP_CVF_FIELD_EVT:
        case AVTP_CVF_FIELD_H264_PTV:
            iRet = avtp_cvf_get_field_value(pAvtpStreamPdu, eField, pVal);
            break;
        case AVTP_CVF_FIELD_H264_TIMESTAMP:
            {
                /* This field lives on H.264 header, inside avtp_payload */
                pH264Payload = (AVTP_CVF_PDU_H264_PAYLOAD_T *)pAvtpStreamPdu->uAvtpPayload;
                if(NULL == pH264Payload)
                {
                    AVTP_ERROR("ptr is NULL\n");
                    return ERROR;
                }
                *pVal = ntohl(pH264Payload->uH264Header);
                iRet = OK;
                break;
            }
        default:
            AVTP_ERROR("field %d not support\n", eField);
            iRet = ERROR;
            break;
    }
    if(iRet < 0)
    {
        AVTP_ERROR("get field %d failed\n", eField);
    }
    return iRet;
}

/**@fn	       avtp_cvf_pdu_filed_set	  
 * @brief	   设置CVF avtpdu filed的值
 * @param[in]  pAvtpStreamPdu  pdu结构指针
 * @param[in]  eField  设置的pdu字段
 * @param[in]  uVal  设置的值
 * @return	   成功返回OK  失败返回错误码
 */
INT avtp_cvf_pdu_filed_set(AVTP_STREAM_PDU_T *pAvtpStreamPdu, AVTP_CVF_FIELD_E eField, UINT64 uVal)
{
    INT iRet = ERROR;
	AVTP_CVF_PDU_H264_PAYLOAD_T *pStH264Payload = NULL;

    if (NULL == pAvtpStreamPdu)
    {
        AVTP_ERROR("param error\n");
        return iRet;
    }

    //AVTP_INFO(">>> eField:%d\n", eField);
    switch (eField)
    {
        case AVTP_CVF_FIELD_SV:
            iRet = avtp_common_stream_pdu_set(pAvtpStreamPdu, AVTP_STREAM_FIELD_SV, uVal);
            break;
        case AVTP_CVF_FIELD_MR:
            iRet = avtp_common_stream_pdu_set(pAvtpStreamPdu, AVTP_STREAM_FIELD_MR, uVal);
            break;
        case AVTP_CVF_FIELD_TV:
            iRet = avtp_common_stream_pdu_set(pAvtpStreamPdu, AVTP_STREAM_FIELD_TV, uVal);
            break;
        case AVTP_CVF_FIELD_SEQ_NUM:
            iRet = avtp_common_stream_pdu_set(pAvtpStreamPdu, AVTP_STREAM_FIELD_SEQ_NUM, uVal);
            break;
        case AVTP_CVF_FIELD_TU:
            iRet = avtp_common_stream_pdu_set(pAvtpStreamPdu, AVTP_STREAM_FIELD_TU, uVal);
            break;
        case AVTP_CVF_FIELD_STREAM_DATA_LEN:
            iRet = avtp_common_stream_pdu_set(pAvtpStreamPdu, AVTP_STREAM_FIELD_STREAM_DATA_LEN, uVal);
            break;
        case AVTP_CVF_FIELD_TIMESTAMP:
            iRet = avtp_common_stream_pdu_set(pAvtpStreamPdu, AVTP_STREAM_FIELD_TIMESTAMP, uVal);
            break;
        case AVTP_CVF_FIELD_STREAM_ID:
            iRet = avtp_common_stream_pdu_set(pAvtpStreamPdu, AVTP_STREAM_FIELD_STREAM_ID, uVal);
            break;
        case AVTP_CVF_FIELD_FORMAT:
        case AVTP_CVF_FIELD_FORMAT_SUBTYPE:
        case AVTP_CVF_FIELD_M:
        case AVTP_CVF_FIELD_EVT:
        case AVTP_CVF_FIELD_H264_PTV:
            iRet = avtp_cvf_set_field_value(pAvtpStreamPdu, eField, uVal);
            break;
        case AVTP_CVF_FIELD_H264_TIMESTAMP:
            pStH264Payload = (AVTP_CVF_PDU_H264_PAYLOAD_T*)pAvtpStreamPdu->uAvtpPayload;
            if(NULL != pStH264Payload)
            {
                pStH264Payload->uH264Header =  htonl((UINT)uVal);
                iRet = OK;	
            }
            else
            {
                AVTP_ERROR("ptr is NULL\n");
            }
            break;
        default:
            AVTP_ERROR("field %d not support\n", eField);
            break;
    } 
    if(iRet < 0)
    {
        AVTP_ERROR("set field %d failed\n", eField);
    }
	return iRet;
}
