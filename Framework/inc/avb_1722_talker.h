#ifndef _AVB_1722_TALKER_H_
#define _AVB_1722_TALKER_H_

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/



/**
* @struct AVB_1722_TALKER_TRANSMIT_PARAM_T
* @brief 
*/
 typedef struct 
{
     signed char iSockPriority;   //传输套接字优先级，0~6
     unsigned char aMacAddr[6]; //talker:目的mac地址，listener:源（本地）mac地址
     char  aInterfaceName[32];//网卡名称
     unsigned int uMaxTransmitTime;//最大传输时间 单位ms
     unsigned int uMaxPayloadLen;//avtp报文payload最大长度
     unsigned long long uStreamID;       //SteamID
     unsigned int uRes[4];
}AVB_1722_TALKER_PARAM_T;  

/**
* @struct AVB_1722_TALKER_SET_PARAM_T
* @brief 配置参数结构体定义
*/
typedef struct
{
     AVB_1722_TALKER_PARAM_T stParam;
     unsigned int uFrameBufSize;  //帧读取缓冲区大小
     unsigned int uRes[7];
}AVB_1722_TALKER_SET_PARAM_T;

//AVB 1722 Talker
typedef struct IAVB1722Talker IAVB1722Talker;
struct IAVB1722Talker
{
    /**@fn         Init   
     * @brief      初始化参数
     * @param[in]  IAVB1722Talker   IAVB1722Talker对象操作指针
     * @param[in]  pStCfgParam   传输参数
     * @return     成功返回OK     失败返回错误码
     */
    int (*Init)(IAVB1722Talker *pIAVB1722Talker);

    /**@fn         SetParam   
     * @brief      设置相关参数
     * @param[in]  pIAVB1722Talker 对象操作指针
     * @param[in]  pParam 参数结构体指针
     * @return     成功返回OK     失败返回错误码
     * @note       设置AVB 1722 talker相关参数
     */
    int (*SetParam)(IAVB1722Talker *pIAVB1722Talker, AVB_1722_TALKER_SET_PARAM_T *pStParam);

    /**@fn         Start      
     * @brief      开启服务
     * @param[in]  pIAVB1722Talker   IAVB1722Talker对象操作指针
     * @return     成功返回OK     失败返回错误码
     */
    int (*Start)(IAVB1722Talker *pIAVB1722Talker);


    /**@fn         Stop   
     * @brief      关闭服务
     * @param[in]  pIAVB1722Talker   IAVB1722Talker对象操作指针
     * @return     成功返回OK     失败返回错误码
     */
    int (*Stop)(IAVB1722Talker *pIAVB1722Talker);

    /**@fn         GetState   
     * @brief      获取指定通道推流状态
     * @param[in]  pIAVB1722Talker  对象操作指针
     * @param[in]  pState 待获取推流状态
     * @return     成功返回OK     失败返回错误码
     */
    int (*GetState)(IAVB1722Talker *pIAVB1722Talker, AVB_TALKER_STATE_E *pState);
    
    /**@fn         Release    
     * @brief      模块资源释放
     * @param[in]  pIAVB1722Talker   IAVB1722Talker对象操作指针
     * @return     VOID
     */
    void (*Release)(IAVB1722Talker *pIAVB1722Talker);
};

/**@fn         avb_1722_talker
 * @brief      创建avb 1722 talker实例
 * @param[in]  pStInitParam 初始参数
 * @return     成功返回IAVB1722Talker对象指针，  失败返回NULL
 */
IAVB1722Talker *avb_1722_talker_instance(void);



#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif