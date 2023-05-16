/* 
 * @file    avtp_cvf.h
 * @note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @brief   avtp cvf格式封装及解封实现         
 * @author  chen
 * @date    2022-7-21
 * @version v1.0.0 
 */
#ifndef __AVTP_CVF_H__
#define __AVTP_CVF_H__
#include <errno.h>
#include <stdint.h>
#include "sys_common.h"
#include "avtp_pdu.h"
#include "avtp_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AVTP_CVF_HEADER_LEN                 (sizeof(AVTP_STREAM_PDU_T))  /* CVF avtp头长度 24字节*/
#define AVTP_CVF_PAYLOAD_H264_HEADER_LEN	(sizeof(UINT32))  /* CVF avtp payload h264头长度 4字节*/  

#define AVTP_H264_HEADER_LEN	(sizeof(UINT32))    
#define AVTP_HEADER_LEN	        (sizeof(AVTP_STREAM_PDU_T))
#define AVTP_FULL_HEADER_LEN	(sizeof(AVTP_STREAM_PDU_T) + AVTP_H264_HEADER_LEN)

typedef struct
{
	UINT32 uH264Header;
	UINT8  uH264Data[0];
}AVTP_CVF_PDU_H264_PAYLOAD_T;

/**@fn	       avtp_cvf_pdu_filed_get	  
 * @brief	   获取CVF avtpdu filed的值
 * @param[in]  pAvtpStreamPdu  pdu结构指针
 * @param[in]  eField  检索的pdu字段
 * @param[in]  pVal  保存检索pdu字段值的变量的指针
 * @return	   成功返回OK  失败返回错误码
 */
INT32 avtp_cvf_pdu_filed_get(const AVTP_STREAM_PDU_T *pAvtpStreamPdu, AVTP_CVF_FIELD_E eField, UINT64 *pVal);

/**@fn	       avtp_cvf_pdu_filed_set	  
 * @brief	   设置CVF avtpdu filed的值
 * @param[in]  pAvtpStreamPdu  pdu结构指针
 * @param[in]  eField  设置的pdu字段
 * @param[in]  uVal  设置的值
 * @return	   成功返回OK  失败返回错误码
 */
INT32 avtp_cvf_pdu_filed_set(AVTP_STREAM_PDU_T *pAvtpStreamPdu, AVTP_CVF_FIELD_E eField, UINT64 uVal);

#ifdef __cplusplus
}
#endif
#endif
