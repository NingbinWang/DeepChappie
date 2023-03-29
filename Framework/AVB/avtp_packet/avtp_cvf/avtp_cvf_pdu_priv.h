/* 
 * @file    avtp_cvf_pdu_priv.h
 * @note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @brief   avtp cvf封装
 * 
 * @author  chen
 * @date    2022-7-15
 * @version v1.0.0
 */
#ifndef __AVTP_CVF_PDU_PRIV_H__
#define __AVTP_CVF_PDU_PRIV_H__
#include "avtp_cvf_pdu.h"
#include "avtp_internal.h"

typedef struct
{
    UINT8  *pAvtpPduBuf;          /*pdu指针*/
    UINT32 uPduLengthMax;         /*(pdu)一个avtp报文的最大长度*/
    UINT8  uFormatSubtype;        /*CVF格式子类型 参见AVTP_CVF_FORMAT_SUBTYPE_E*/
    BOOL   bDeserialFlag;
    UINT8  uRes[6];         
    UINT32 uPduPayloadHeaderLen;  /* avtp payload中header长度*/
    UINT32 uPduPayloadDataLen;    /* avtp payload中数据长度,注:payload格式为header+data*/
}AVTP_CVF_PDU_PRIV_DATA_T;

/* AVTP封装对象实例结构体 */
typedef struct
{
    IAvtpCvfPdu stInterface;                    /* 对外API接口 */
    AVTP_CVF_PDU_PRIV_DATA_T stPrivData;        /* 私有数据结构体 */
}AVTP_CVF_PDU_BASE_T;

#endif