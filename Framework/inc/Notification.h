#ifndef _NOTIFICATION_H_
#define _NOTIFICATION_H_

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

typedef struct 
{
    int  enable;
    unsigned int pingpongsize;
}Notification_Info_t;



typedef struct INotification INotification;
struct INotification
{
     /**@fn         Init   
     * @brief      初始化参数
     * @param[in]  pINotificaion   INotificaiton
     * @return     成功返回OK     失败返回错误码
     */
    int (*Init)(INotification *pINotificaion);
     /**@fn         Subscribe   
     * @brief      订阅发布者
     * @param[in]  pINotificaion   INotificaiton对象操作指针
     * @param[in]  pubID: Publisher ID
     * @return     成功返回OK     失败返回错误码
     */
    int (*Subscribe)(INotification *pINotificaion, const char* pubid);

    /**@fn         Unsubscribe   
     * @brief      取消订阅
     * @param[in]  pINotificaion   INotificaiton对象操作指针
     * @param[in]  pubID: Publisher ID
     * @return     成功返回OK     失败返回错误码
     */
    int (*Unsubscribe)(INotification *pINotificaion, const char* pubid);

    /**@fn         Commit 
     * @brief      发送数据到cache中
     * @param[in]  pINotificaion   INotificaiton对象操作指针
     * @return     成功返回OK     失败返回错误码
     */
    int (*Commit)(INotification *pINotificaion,const char* pubid, const void* data,unsigned int size);

    /**@fn         Publish 
     * @brief      发送数据给订阅者
     * @param[in]  pINotificaion   INotificaiton对象操作指针
     * @return     成功返回OK     失败返回错误码
     */
    int (*Publish)(INotification *pINotificaion);

    /**@fn         pull 
     * @brief      从订阅者那里拉去数据
     * @param[in]  pINotificaion   INotificaiton对象操作指针
     * @return     成功返回OK     失败返回错误码
     */
    int (*Pull)(INotification *pINotificaion,const char* pubid,void* data,unsigned int size);

    /**@fn         Notify
     * @brief      发送通知给发布者
     * @param[in]  pINotificaion   INotificaiton对象操作指针
     * @return     成功返回OK     失败返回错误码
     */
    int (*Notify)(INotification *pINotificaionn,const char* pubid,void* data,unsigned int size);

    /**@fn         Release    
     * @brief      模块资源释放
     * @param[in]  pINotificaion   INotificaiton对象操作指针
     * @return     int
     */
    int (*Release)(INotification *pINotificaionn);

};




INotification *notification_init_instance(unsigned int pingpongbuffsize);

#ifdef __cplusplus
}
#endif/*__cplusplus*/



#endif