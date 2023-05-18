#ifndef _NETWORK_H_
#define _NETWORK_H_

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

#define NETWORKSTRMAX 256

typedef struct 
{
    int  enable;
    char DevName[NETWORKSTRMAX];      //eth名字
   	char IP[NETWORKSTRMAX];           // IP地址
	  char NetMask[NETWORKSTRMAX];      // netmask 
	  char GateWay[NETWORKSTRMAX];      // gateway 
}NetworkInfo_t;

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


INetwork *network_init_instance(char* sDevname);

#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif