#ifndef __AVTP_COMMON_H__
#define __AVTP_COMMON_H__
#include "sys_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* AVTP subtypes values. For further information refer to section 4.4.3.2 from
 * IEEE 1722-2016 spec.
 */
#define AVTP_SUBTYPE_61883_IIDC			0x00
#define AVTP_SUBTYPE_MMA_STREAM			0x01
#define AVTP_SUBTYPE_AAF			0x02
#define AVTP_SUBTYPE_CVF			0x03
#define AVTP_SUBTYPE_CRF			0x04
#define AVTP_SUBTYPE_TSCF			0x05
#define AVTP_SUBTYPE_SVF			0x06
#define AVTP_SUBTYPE_RVF			0x07
#define AVTP_SUBTYPE_AEF_CONTINUOUS		0x6E
#define AVTP_SUBTYPE_VSF_STREAM			0x6F
#define AVTP_SUBTYPE_EF_STREAM			0x7F
#define AVTP_SUBTYPE_NTSCF			0x82
#define AVTP_SUBTYPE_ESCF			0xEC
#define AVTP_SUBTYPE_EECF			0xED
#define AVTP_SUBTYPE_AEF_DISCRETE		0xEE
#define AVTP_SUBTYPE_ADP			0xFA
#define AVTP_SUBTYPE_AECP			0xFB
#define AVTP_SUBTYPE_ACMP			0xFC
#define AVTP_SUBTYPE_MAAP			0xFE
#define AVTP_SUBTYPE_EF_CONTROL			0xFF

/* XXX: Fields from PDU structs should not be read or written directly since
 * they are encoded in Network order which may be different from the Host
 * order (see section 3.4.1 from IEEE 1722-2016 spec for further information).
 *
 * Any read or write operation with PDU structs should be done via getter and
 * setter APIs which handle byte order conversion.
 */

//AVTPDU common header field
typedef struct
{
    UINT32  uSubtypeData;
    UINT8   uPduSpecific[0];
}__attribute__ ((__packed__))AVTP_COMMON_PDU_T;

//Additional header and payload data
typedef struct
{
    UINT32 uSubtypeData;
    UINT64 uStreamID;
    UINT32 uAvtpTime;
    UINT32 uFormatSpecific;
    UINT32 uPacketInfo;
    UINT8  uAvtpPayload[0];
}__attribute__ ((__packed__))AVTP_STREAM_PDU_T;

//AVTPDU common header field
typedef enum
{
    AVTP_FIELD_SUBTYPE,
    AVTP_FIELD_VERSION,
    AVTP_FIELD_MAX,
}AVTP_COMMOM_FIELD_E;

//Addition header
typedef enum
{
    AVTP_STREAM_FIELD_SV,      //1bit sv
    AVTP_STREAM_FIELD_MR,      //1bit mr
	AVTP_STREAM_FIELD_RSV1,    //2bit rsv
    AVTP_STREAM_FIELD_TV,      //1bit tv
    AVTP_STREAM_FIELD_SEQ_NUM, //8bit sequence number
	AVTP_STREAM_FIELD_RSV2,    //7bit rsv
    AVTP_STREAM_FIELD_TU,      //1bit tu

    AVTP_STREAM_FIELD_STREAM_ID,  //64bite stream id
    AVTP_STREAM_FIELD_TIMESTAMP,  //32bit avtp timestamp
    AVTP_STREAM_FIELD_STREAM_DATA_LEN, //16bit avtp data length
    AVTP_STREAM_FIELD_MAX
}AVTP_STREAM_FIELD_E;

/**@fn	       avtp_common_pdu_get	  
 * @brief	   获取通用AVTPDU field值
 * @param[in]  pStAvtpCommonPdu   pdu结构体指针
 * @param[in]  eField  field类型
 * @param[in]  uVal  待获取值的指针
 * @return	   成功返回OK  失败返回ERROR
 */
INT32 avtp_common_pdu_get(const AVTP_COMMON_PDU_T *pStAvtpCommonPdu, AVTP_COMMOM_FIELD_E eField, UINT32 *uVal);

/**@fn	       avtp_common_pdu_set	  
 * @brief	   设置通用AVTPDU field值
 * @param[in]  pStAvtpStreamPdu   pdu结构体指针
 * @param[in]  eField  field类型
 * @param[in]  uVal  设置的值
 * @return	   成功返回OK  失败返回ERROR
 */
INT32 avtp_common_pdu_set(AVTP_COMMON_PDU_T *pStAvtpCommonPdu, AVTP_COMMOM_FIELD_E eField, UINT32 uVal);

/**@fn	       avtp_common_stream_pdu_get	  
 * @brief	   获取AVTPDU field值
 * @param[in]  pStAvtpStreamPdu   pdu结构体指针
 * @param[in]  eField  field类型
 * @param[in]  uVal  待获取值的指针
 * @return	   成功返回OK  失败返回ERROR
 */
INT32 avtp_common_stream_pdu_get(const AVTP_STREAM_PDU_T *pStAvtpStreamPdu, AVTP_STREAM_FIELD_E eField, UINT64 *uVal);

/**@fn	       avtp_common_stream_pdu_set	  
 * @brief	   设置AVTPDU field值
 * @param[in]  pStAvtpStreamPdu   pdu结构体指针
 * @param[in]  eField  field类型
 * @param[in]  uVal  设置的值
 * @return	   成功返回OK  失败返回ERROR
 */
INT32 avtp_common_stream_pdu_set(AVTP_STREAM_PDU_T *pStAvtpStreamPdu, AVTP_STREAM_FIELD_E eField, UINT64 uVal);

#ifdef __cplusplus
}
#endif

#endif
