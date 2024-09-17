/* 
 * @file    avtp_pdu.h
 * @note    HangZhou Hikvision System Technology Co., Ltd. All Right Reserved.
 * @brief   avtp封装及解封实现         
 * @author  chen
 * @date    2022-6-29
 * @version v1.0.0  1. 支持标准H.264 over CVF封装及解封装
 */
#ifndef __AVTP_PDU_CVF_H__
#define __AVTP_PDU_CVF_H__
#include "avtp_pdu_cvf_defines.h"

typedef struct IAvtpCvfPdu IAvtpCvfPdu;
struct IAvtpCvfPdu
{
     /**@fn	       Init	  
     * @brief	   初始化
     * @param[in]  pIAvtpCvfPdu   IAvtpCvfPdu对象指针
     * @param[in]  uPduLength  pdu长度, >= (64 + RTP payload)
     * @return	   成功返回OK  失败返回错误码
     */
    INT (*Init)(IAvtpCvfPdu *pIAvtpCvfPdu, UINT uPduLength);

    /**@fn	       SetField	  
     * @brief	   设置avtpdu域值
     * @param[in]  pIAvtpCvfPdu   IAvtpCvfPdu对象对象指针
     * @param[in]  uFieldType  域类型, 如H.264 CVF见枚举AVTP_H264_CVF_FIELD_E定义
     * @param[in]  uVal  域值
     * @return	   成功返回OK  失败返回错误码
     */
    INT (*SetField)(IAvtpCvfPdu *pIAvtpCvfPdu, UINT8 uFieldType, UINT64 uVal);

    /**@fn	       GetField	  
     * @brief	   设置avtpdu域值
     * @param[in]  pIAvtpCvfPdu   IAvtpCvfPdu对象对象指针
     * @param[in]  uFieldType  域类型, 如H.264 CVF见枚举AVTP_H264_CVF_FIELD_E定义
     * @param[out] pVal  域值
     * @return	   成功返回OK  失败返回错误码
     */
    INT (*GetField)(IAvtpCvfPdu *pIAvtpCvfPdu, UINT8 uFieldType, UINT64 *pVal);

    /**@fn         SetPayload
     * @brief      设置payload数据
     * @param[in]  pIAvtpCvfPdu  IAvtpCvfPdu对象操作指针
     * @param[in]  pPayloadData   payload数据
     * @param[in]  uDataLength payload数据长度
     * @return     成功返回OK  失败返回错误码
     */
    INT (*SetPayload)(IAvtpCvfPdu *pIAvtpCvfPdu, UINT8 *pPayloadData, UINT uDataLength);

    /**@fn         SetPayload
     * @brief      获取payload数据
     * @param[in]  pIAvtpCvfPdu  IAvtpCvfPdu对象操作指针
     * @param[in]  uLength   payload数据长度
     * @return     成功返回payload数据首地址，失败返回NULL
     */
    UINT8 *(*GetPayload)(IAvtpCvfPdu *pIAvtpCvfPdu, UINT *uLength);

    /**@fn         Serial
     * @brief      序列化
     * @param[in]  pIAvtpCvfPdu  IAvtpCvfPdu对象操作指针
     * @param[in]  uLength     序列化后avtp包长度
     * @return     成功返回序列化后avtp包地址，失败返回NULL
     */
    UINT8 *(*Serial)(IAvtpCvfPdu *pIAvtpCvfPdu, UINT *uLength);

    /**@fn         Deserial
     * @brief      反序列化
     * @param[in]  pIAvtpCvfPdu  IAvtpCvfPdu对象操作指针
     * @param[in]  pAvtpData  avtp包数据
     * @param[in]  uAvtpDataLength    avtp包数据长度
     * @return     成功返回OK  失败返回错误码
     */
    INT (*Deserial)(IAvtpCvfPdu *pIAvtpCvfPdu, UINT8 *pAvtpData, UINT uAvtpDataLength);

    /**@fn	       Release	  
     * @brief	   RTP资源释放接口
     * @param[in]  pIAvtpCvfPdu   IAvtpCvfPdu对象对象接口
     * @return	   VOID
     */
    VOID (*Release)(IAvtpCvfPdu *pIAvtpCvfPdu);
};

/**@fn	       pdu_avtp_init_instance	  
 * @brief	   初始化构造单例
 * @param[in]  pIPduAvtpHost 创建主函数指针集
 * @return	   成功返回单例  失败返回NULL
 */
IAvtpCvfPdu *avtp_cvf_pdu_init_instance(VOID);
#endif