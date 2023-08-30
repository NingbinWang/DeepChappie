#ifndef _NETWORK_H_
#define _NETWORK_H_

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

#define NETWORKSTRMAX 256



typedef struct INetwork INetwork;
struct INetwork
{
    /**@fn         Init   
     * @brief      初始化参数
     * @param[in]  pINetwork_manager   INetwork对象操作指针
     * @return     成功返回OK     失败返回错误码
     */
    int (*Init)(INetwork *pINetwork_manager,const char *strIP,const char *strNetMask, const char *strGateWay);

    /**@fn         UP    
     * @brief      开启服务
     * @param[in]  pINetwork_manager   INetwork对象操作指针
     * @return     成功返回OK     失败返回错误码
     */
    int (*Up)(INetwork *pINetwork_manager);

      /**@fn       Down  
     * @brief      关闭服务
     * @param[in]  pINetwork_manager   INetwork对象操作指针
     * @return     成功返回OK     失败返回错误码
     */
    int (*Down)(INetwork *pINetwork_manager);

    /**@fn         Release    
     * @brief      模块资源释放
     * @param[in]  pINetwork_manager   INetwork对象操作指针
     * @return     int
     */
    int (*Release)(INetwork *pIGsensor_manager);
    
};


void init_network_component(char* sDevname);
INetwork *network_get_instance(void);

#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif