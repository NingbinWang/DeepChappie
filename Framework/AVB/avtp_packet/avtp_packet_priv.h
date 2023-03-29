#ifndef _AVTP_PACKET_H_
#define _AVTP_PACKET_H_

typedef struct
{
    AVTP_PDU_SUBTYPE_E eType;
    UINT32             uPduLength;       /* pdu长度 */
    IAvtpCvfPdu        *pCVFInstance;    /* cvf单例 */
}AVTP_PDU_PRIV_DATA_T;

/* AVTP封装对象实例结构体 */
typedef struct
{
    IAvtpPdu stInterface;                   /* 对外API接口 */
    AVTP_PDU_PRIV_DATA_T stPrivData;        /* 私有数据结构体 */
}AVTP_PDU_BASE_T;


#endif