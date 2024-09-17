#include "avtp_common.h"
#include "avtp_util.h"
#include "avtp_internal.h"

#define SHIFT_SUBTYPE			(31u - 7u)
#define SHIFT_VERSION			(31u - 11u)

#define MASK_SUBTYPE			(BITMASK(8u) << SHIFT_SUBTYPE)
#define MASK_VERSION			(BITMASK(3u) << SHIFT_VERSION)

#define SHIFT_SV			(31u - 8u)
#define SHIFT_MR			(31u - 12u)
#define SHIFT_TV			(31u - 15u)
#define SHIFT_SEQ_NUM			(31u - 23u)
#define SHIFT_STREAM_DATA_LEN		(31u - 15u)

#define MASK_SV				(BITMASK(1u) << SHIFT_SV)
#define MASK_MR				(BITMASK(1u) << SHIFT_MR)
#define MASK_TV				(BITMASK(1u) << SHIFT_TV)
#define MASK_SEQ_NUM			(BITMASK(8u) << SHIFT_SEQ_NUM)
#define MASK_TU				(BITMASK(1u))
#define MASK_STREAM_DATA_LEN		(BITMASK(16u) << SHIFT_STREAM_DATA_LEN)

/**@fn	       avtp_common_pdu_get	  
 * @brief	   获取通用AVTPDU field值
 * @param[in]  pStAvtpCommonPdu   pdu结构体指针
 * @param[in]  eField  field类型
 * @param[in]  uVal  待获取值的指针
 * @return	   成功返回OK  失败返回ERROR
 */
INT avtp_common_pdu_get(const AVTP_COMMON_PDU_T *pStAvtpCommonPdu, AVTP_COMMOM_FIELD_E eField, UINT *uVal)
{
    INT iRet = OK;
    UINT uBitmap = 0;
    UINT uMask = 0;
    UINT8 uShift = 0;
    if ((NULL == pStAvtpCommonPdu) || (NULL == uVal))
    {
        AVTP_ERROR("param error\n");
        return ERROR;
    }
	switch (eField) 
    {
        case AVTP_FIELD_SUBTYPE:
            uMask = MASK_SUBTYPE;
            uShift = SHIFT_SUBTYPE;
            break;
        case AVTP_FIELD_VERSION:
            uMask = MASK_VERSION;
            uShift = SHIFT_VERSION;
            break;
        default:
            AVTP_ERROR("field %d not support\n", eField);
            iRet = ERROR;
            break;
    }
    if(iRet < 0)
    {
        return iRet;
    }
    uBitmap = ntohl(pStAvtpCommonPdu->uSubtypeData);
    *uVal = BITMAP_GET_VALUE(uBitmap, uMask, uShift);
    return OK;
}

/**@fn	       avtp_common_pdu_set	  
 * @brief	   设置通用AVTPDU field值
 * @param[in]  pStAvtpStreamPdu   pdu结构体指针
 * @param[in]  eField  field类型
 * @param[in]  uVal  设置的值
 * @return	   成功返回OK  失败返回ERROR
 */
INT avtp_common_pdu_set(AVTP_COMMON_PDU_T *pStAvtpCommonPdu, AVTP_COMMOM_FIELD_E eField, UINT uVal)
{
    INT iRet = OK;
    UINT uBitmap = 0;
    UINT uMask = 0;
    UINT8 uShift = 0;

    if (NULL == pStAvtpCommonPdu)
    {
        AVTP_ERROR("param error\n");
        return ERROR;
    }
	switch (eField) 
    {
        case AVTP_FIELD_SUBTYPE:
            uMask = MASK_SUBTYPE;
            uShift = SHIFT_SUBTYPE;
            break;
        case AVTP_FIELD_VERSION:
            uMask = MASK_VERSION;
            uShift = SHIFT_VERSION;
            break;
        default:
            AVTP_ERROR("field %d not support\n", eField);
            iRet = ERROR;
            break;
	}
    if(iRet < 0)
    {
        return iRet;
    }
    uBitmap = ntohl(pStAvtpCommonPdu->uSubtypeData);
    BITMAP_SET_VALUE(uBitmap, uVal, uMask, uShift);
    pStAvtpCommonPdu->uSubtypeData = htonl(uBitmap);
    return OK;
}

/**@fn	       avtp_get_field_value	  
 * @brief	   获取AVTPDU field值
 * @param[in]  pStAvtpStreamPdu   pdu结构体指针
 * @param[in]  eField  field类型
 * @param[in]  uVal  待获取值的指针
 * @return	   成功返回OK  失败返回ERROR
 */
static INT avtp_get_field_value(const AVTP_STREAM_PDU_T *pStAvtpStreamPdu,AVTP_STREAM_FIELD_E eField, UINT64 *uVal)
{
    INT iRet = OK;
	UINT uBitmap = 0;
    UINT uMask = 0;
    UINT8 uShift = 0;

    if((NULL == pStAvtpStreamPdu) || (NULL == uVal))
    {
        AVTP_ERROR("param error\n");
        return ERROR;
    }
    switch (eField) 
    {
        case AVTP_STREAM_FIELD_SV:
            uMask = MASK_SV;
            uShift = SHIFT_SV;
            uBitmap = ntohl(pStAvtpStreamPdu->uSubtypeData);
            break;
        case AVTP_STREAM_FIELD_MR:
            uMask = MASK_MR;
            uShift = SHIFT_MR;
            uBitmap = ntohl(pStAvtpStreamPdu->uSubtypeData);
            break;
        case AVTP_STREAM_FIELD_TV:
            uMask = MASK_TV;
            uShift = SHIFT_TV;
            uBitmap = ntohl(pStAvtpStreamPdu->uSubtypeData);
            break;
        case AVTP_STREAM_FIELD_SEQ_NUM:
            uMask = MASK_SEQ_NUM;
            uShift = SHIFT_SEQ_NUM;
            uBitmap = ntohl(pStAvtpStreamPdu->uSubtypeData);
            break;
        case AVTP_STREAM_FIELD_TU:
            uMask = MASK_TU;
            uShift = 0;
            uBitmap = ntohl(pStAvtpStreamPdu->uSubtypeData);
            break;
        case AVTP_STREAM_FIELD_STREAM_DATA_LEN:
            uMask = MASK_STREAM_DATA_LEN;
            uShift = SHIFT_STREAM_DATA_LEN;
            uBitmap = ntohl(pStAvtpStreamPdu->uPacketInfo);
            break;
        default:
            AVTP_ERROR("field %d not support\n", eField);
            iRet = ERROR;
            break;
    }
    if(iRet < 0)
    {
        return iRet;
    }
    *uVal = BITMAP_GET_VALUE(uBitmap, uMask, uShift);
    return OK;
}

/**@fn	       avtp_set_field_value	  
 * @brief	   设置AVTPDU field值
 * @param[in]  pStAvtpStreamPdu   pdu结构体指针
 * @param[in]  eField  field类型
 * @param[in]  uVal  设置的值
 * @return	   成功返回OK  失败返回ERROR
 */
static INT avtp_set_field_value(AVTP_STREAM_PDU_T *pStAvtpStreamPdu,AVTP_STREAM_FIELD_E eField, UINT64 uVal)
{
	UINT uBitmap = 0;
    UINT uMask = 0;
    UINT8 uShift = 0;
    void *ptr = NULL;

	if(NULL == pStAvtpStreamPdu)
	{
        AVTP_ERROR("param error\n");
		return ERROR;
	}
    switch(eField) 
    {
        case AVTP_STREAM_FIELD_SV:
            uMask = MASK_SV;
            uShift = SHIFT_SV;
            ptr = &pStAvtpStreamPdu->uSubtypeData;
            break;
        case AVTP_STREAM_FIELD_MR:
            uMask = MASK_MR;
            uShift = SHIFT_MR;
            ptr = &pStAvtpStreamPdu->uSubtypeData;
            break;
        case AVTP_STREAM_FIELD_TV:
            uMask = MASK_TV;
            uShift = SHIFT_TV;
            ptr = &pStAvtpStreamPdu->uSubtypeData;
            break;
        case AVTP_STREAM_FIELD_SEQ_NUM:
            uMask = MASK_SEQ_NUM;
            uShift = SHIFT_SEQ_NUM;
            ptr = &pStAvtpStreamPdu->uSubtypeData;
            break;
        case AVTP_STREAM_FIELD_TU:
            uMask = MASK_TU;
            uShift = 0;
            ptr = &pStAvtpStreamPdu->uSubtypeData;
            break;
        case AVTP_STREAM_FIELD_STREAM_DATA_LEN:
            uMask = MASK_STREAM_DATA_LEN;
            uShift = SHIFT_STREAM_DATA_LEN;
            ptr = &pStAvtpStreamPdu->uPacketInfo;
            break;
        default:
            AVTP_ERROR("field %d not support\n", eField);
            break;
    }
    if(NULL == ptr)
    {
        return ERROR;
    }
    uBitmap = get_unaligned_be32(ptr);
    BITMAP_SET_VALUE(uBitmap, uVal, uMask, uShift);
    put_unaligned_be32(uBitmap, ptr);
    return OK;
}

/**@fn	       avtp_common_stream_pdu_get	  
 * @brief	   获取AVTPDU field值
 * @param[in]  pStAvtpStreamPdu   pdu结构体指针
 * @param[in]  eField  field类型
 * @param[in]  uVal  待获取值的指针
 * @return	   成功返回OK  失败返回ERROR
 */
INT avtp_common_stream_pdu_get(const AVTP_STREAM_PDU_T *pStAvtpStreamPdu, AVTP_STREAM_FIELD_E eField, UINT64 *uVal)
{
    INT iRet = ERROR;
	if ((NULL == pStAvtpStreamPdu) || (NULL == uVal))
	{
        AVTP_ERROR("param error\n");
		return iRet;
	}
    switch (eField) 
    {
        case AVTP_STREAM_FIELD_SV:
        case AVTP_STREAM_FIELD_MR:
        case AVTP_STREAM_FIELD_TV:
        case AVTP_STREAM_FIELD_SEQ_NUM:
        case AVTP_STREAM_FIELD_TU:
        case AVTP_STREAM_FIELD_STREAM_DATA_LEN:
            iRet = avtp_get_field_value(pStAvtpStreamPdu, eField, uVal);
            break;
        case AVTP_STREAM_FIELD_TIMESTAMP:
            *uVal = ntohl(pStAvtpStreamPdu->uAvtpTime);
            iRet = OK;
            break;
        case AVTP_STREAM_FIELD_STREAM_ID:
            *uVal = be64toh(pStAvtpStreamPdu->uStreamID);
            iRet = OK;
            break;
        default:
            AVTP_ERROR("field %d not support\n", eField);
            iRet = ERROR;
            break;
    }
    return iRet;
}

/**@fn	       avtp_common_stream_pdu_set	  
 * @brief	   设置AVTPDU field值
 * @param[in]  pStAvtpStreamPdu   pdu结构体指针
 * @param[in]  eField  field类型
 * @param[in]  uVal  设置的值
 * @return	   成功返回OK  失败返回ERROR
 */
INT avtp_common_stream_pdu_set(AVTP_STREAM_PDU_T *pStAvtpStreamPdu, AVTP_STREAM_FIELD_E eField, UINT64 uVal)
{
    INT iRet = ERROR;

    if (NULL == pStAvtpStreamPdu)
    {
        AVTP_ERROR("param error\n");
        return iRet;
    }
	switch (eField) 
    {
        case AVTP_STREAM_FIELD_SV:
        case AVTP_STREAM_FIELD_MR:
        case AVTP_STREAM_FIELD_TV:
        case AVTP_STREAM_FIELD_SEQ_NUM:
        case AVTP_STREAM_FIELD_TU:
        case AVTP_STREAM_FIELD_STREAM_DATA_LEN:
            iRet = avtp_set_field_value(pStAvtpStreamPdu, eField, uVal);
            break;
        case AVTP_STREAM_FIELD_TIMESTAMP:
            pStAvtpStreamPdu->uAvtpTime = htonl(uVal);
            iRet = OK;
            break;
        case AVTP_STREAM_FIELD_STREAM_ID:
            pStAvtpStreamPdu->uStreamID = htobe64(uVal);
            iRet = OK;
            break;
        default:
            AVTP_ERROR("field %d not support\n", eField);
            iRet = ERROR;
            break;
    }
    return iRet;
}