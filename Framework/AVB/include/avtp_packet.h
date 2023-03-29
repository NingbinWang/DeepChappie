#ifndef _ACTP_PACKET_H_
#define _ACTP_PACKET_H_

/*avtp pdu subtype*/
typedef enum
{
    AVTP_PDU_SUBTYPE_IECIIDC = 0x00,  /*IEC61883和IIDC格式,如MPEG2-TS*/
    AVTP_PDU_SUBTYPE_AAF     = 0x02,  /*avtp音频格式(AVTP Audio Format), 如PCM*/
    AVTP_PDU_SUBTYPE_CVF     = 0x03,  /*压缩视频格式(Compressed Video Format), 如RFC H.264 CVF*/
    AVTP_PDU_SUBTYPE_CRF     = 0x04,  /*时钟参考格式(Clock Reference Format)*/
}AVTP_PDU_SUBTYPE_E;

typedef struct IAvtpPdu IAvtpPdu;
struct IAvtpPdu
{
    /**@fn	       SetSubType	  
     * @brief	   设置subtype
     * @param[in]  pIAvtpPdu   IAvtpPdu对象对象指针
     * @param[in]  eSubType avtp类型
     * @return	   成功返回OK  失败返回错误码
     */
    INT32 (*SetSubType)(IAvtpPdu *pIAvtpPdu, AVTP_PDU_SUBTYPE_E eSubType);

    /**@fn	       GetSubType	  
     * @brief	   获取subtype
     * @param[in]  pIAvtpPdu   IAvtpPdu对象对象指针
     * @param[in]  pSubType avtp类型
     * @return	   成功返回OK  失败返回错误码
     */
    INT32 (*GetSubType)(IAvtpPdu *pIAvtpPdu, AVTP_PDU_SUBTYPE_E *pSubType);

    /**@fn	       SetField	  
     * @brief	   设置avtpdu域值
     * @param[in]  pIAvtpPdu   IAvtpPdu对象对象指针
     * @param[in]  uFieldType  域类型, 根据SetSubType选择不同定义,如CVF H.264见枚举AVTP_H264_CVF_FIELD_E定义
     * @param[in]  uVal  域值
     * @return	   成功返回OK  失败返回错误码
     */
    INT32 (*SetField)(IAvtpPdu *pIAvtpPdu, UINT8 uFieldType, UINT64 uVal);

    /**@fn	       GetField	  
     * @brief	   设置avtpdu域值
     * @param[in]  pIAvtpPdu   IAvtpPdu对象对象指针
     * @param[in]  uFieldType  域类型, 根据GetSubType选择不同定义, 如CVF H.264见枚举AVTP_H264_CVF_FIELD_E定义
     * @param[out] pVal  域值
     * @return	   成功返回OK  失败返回错误码
     */
    INT32 (*GetField)(IAvtpPdu *pIAvtpPdu, UINT8 uFieldType, UINT64 *pVal);

    /**@fn         SetPayload
     * @brief      设置payload数据
     * @param[in]  pIAvtpPdu  IAvtpPdu对象操作指针
     * @param[in]  pPayloadData   payload数据
     * @param[in]  uDataLength payload数据长度
     * @return     成功返回OK  失败返回错误码
     */
    INT32 (*SetPayload)(IAvtpPdu *pIAvtpPdu, UINT8 *pPayloadData, UINT32 uDataLength);

    /**@fn         SetPayload
     * @brief      获取payload数据
     * @param[in]  pIAvtpPdu  IAvtpPdu对象操作指针
     * @param[in]  uLength   指向保存获取payload数据长度的变量的指针
     * @return     成功返回payload数据首地址，失败返回NULL
     */
    UINT8 *(*GetPayload)(IAvtpPdu *pIAvtpPdu, UINT32 *uLength);

    /**@fn         Serial
     * @brief      序列化
     * @param[in]  pIAvtpPdu  IAvtpPdu对象操作指针
     * @param[in]  uLength    指向保存序列化后avtp包长度的变量的指针
     * @return     成功返回序列化后avtp包地址，失败返回NULL
     */
    UINT8 *(*Serial)(IAvtpPdu *pIAvtpPdu, UINT32 *uLength);

    /**@fn         Deserial
     * @brief      反序列化
     * @param[in]  pIAvtpPdu  IAvtpPdu对象操作指针
     * @param[in]  pAvtpData  avtp包数据
     * @param[in]  uAvtpDataLength    avtp包数据长度
     * @return     成功返回序列化长度   失败返回<0
     */
    INT32 (*Deserial)(IAvtpPdu *pIAvtpPdu, UINT8 *pAvtpData, UINT32 uAvtpDataLength);

    /**@fn	       Release	  
     * @brief	   RTP资源释放接口
     * @param[in]  pIAvtpPdu   IAvtpPdu对象对象接口
     * @return	   VOID
     */
    VOID (*Release)(IAvtpPdu *pIAvtpPdu);
};

/**@fn	       pdu_avtp_init_instance	  
 * @brief	   初始化构造单例
 * @param[in]  uPduLength  pdu缓冲区长度
 * @return	   成功返回单例  失败返回NULL
 */
IAvtpPdu *avtp_pdu_init_instance(const UINT32 uPduLength);


#endif