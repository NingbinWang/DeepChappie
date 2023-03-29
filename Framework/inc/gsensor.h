#ifndef _GSENSOR_H_
#define _GSENSOR_H_

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

typedef enum
{
    GSENSOR_MANAGER_INIT,
    GSENSOR_MANAGER_START,   /*开始*/
    GSENSOR_MANAGER_STOP,    /*停止*/
}GSENSOR_MANAGER_STATE_E;

typedef struct IGsensor_manager IGsensor_manager;
struct IGsensor_manager
{
    /**@fn         Init   
     * @brief      初始化参数
     * @param[in]  IGsensor_manager   IGsensor_manager对象操作指针
     * @return     成功返回OK     失败返回错误码
     */
    int (*Init)(IGsensor_manager *pIGsensor_manager);

    /**@fn         Start      
     * @brief      开启服务
     * @param[in]  IGsensor_manager   pIGsensor_manager对象操作指针
     * @return     成功返回OK     失败返回错误码
     */
    int (*Start)(IGsensor_manager *pIGsensor_manager);

      /**@fn         Stop   
     * @brief      关闭服务
     * @param[in]  IGsensor_manager   pIGsensor_manager对象操作指针
     * @return     成功返回OK     失败返回错误码
     */
    int (*Stop)(IGsensor_manager *pIGsensor_manager);

    /**@fn         GetState   
     * @brief      获取指定通道推流状态
     * @param[in]  IGsensor_manager  对象操作指针
     * @param[in]  pState 待获取推流状态
     * @return     成功返回OK     失败返回错误码
     */
    int (*GetState)(IGsensor_manager *pIGsensor_manager, GSENSOR_MANAGER_STATE_E *pState);

    /**@fn         Release    
     * @brief      模块资源释放
     * @param[in]  pIGsensor_manager   IGsensor_manager对象操作指针
     * @return     int
     */
    int (*Release)(IGsensor_manager *pIGsensor_manager);
};


IGsensor_manager *gsensor_manager_init_instance(void);


#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif