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

/* Event type enumeration */
typedef enum
{
        EVENT_NONE,
        EVENT_PUB_PUBLISH, // Publisher posted information
        EVENT_SUB_PULL,    // Subscriber data pull request
        EVENT_NOTIFY,      // Subscribers send notifications to publishers
        EVENT_TIMER,       // Timed event
}EVENTCODE_E;


typedef enum
{
    ERROR_NONE                = 0,
    ERROR_UNKNOW              = -1,
    ERROR_SIZE_MISMATCH       = -2,
    ERROR_UNSUPPORTED_REQUEST = -3,
    ERROR_NO_CALLBACK         = -4,
    ERROR_NO_CACHE            = -5,
    ERROR_NO_COMMITED         = -6,
    ERROR_NOT_FOUND           = -7,
    ERROR_PARAM_ERROR         = -8
}ERRORCODE_E;


typedef struct
{
        EVENTCODE_E event; // Event type
        void* pData;      // Pointer to data
        unsigned int size;     // The length of the data
}EVENTPARAM_T;

 /* Event callback function pointer */
typedef ERRORCODE_E (*EventCallback_t)( EVENTPARAM_T* param);


typedef struct INotification INotification;
struct INotification
{
     /**@fn         Init   
     * @brief      初始化参数
     * @param[in]  pINotificaion   INotificaiton
     * @return     成功返回OK     失败返回错误码
     */
    ERRORCODE_E (*Init)(INotification *pINotificaion);
     /**@fn         Subscribe   
     * @brief      订阅发布者
     * @param[in]  pINotificaion   INotificaiton对象操作指针
     * @param[in]  pubID: Publisher ID
     * @return     成功返回OK     失败返回错误码
     */
    ERRORCODE_E (*Subscribe)(INotification *pINotification,const char* pubid,const void* data,unsigned int size);
     /**@fn        SetEventCallback   
     * @brief      设置发布者的回调
     * @param[in]  pINotificaion   INotificaiton对象操作指针
     * @param[in]  pubID: Publisher ID
     * @return     成功返回OK     失败返回错误码
     */
    ERRORCODE_E (*SetEventCallback)(INotification *pINotificaion,const char* pubid,EventCallback_t callback);

    /**@fn         Unsubscribe   
     * @brief      取消订阅
     * @param[in]  pINotificaion   INotificaiton对象操作指针
     * @param[in]  pubID: Publisher ID
     * @return     成功返回OK     失败返回错误码
     */
    ERRORCODE_E (*Unsubscribe)(INotification *pINotificaion, const char* pubid);

    /**@fn         Commit 
     * @brief      发送数据到cache中
     * @param[in]  pINotificaion   INotificaiton对象操作指针
     * @return     成功返回OK     失败返回错误码
     */
    ERRORCODE_E (*Commit)(INotification *pINotificaion,const char* pubid, const void* data,unsigned int size);

    /**@fn         Publish 
     * @brief      发送数据给订阅者
     * @param[in]  pINotificaion   INotificaiton对象操作指针
     * @return     成功返回OK     失败返回错误码
     */
    ERRORCODE_E (*Publish)(INotification *pINotificaion,const char* pubid);

    /**@fn         pull 
     * @brief      从订阅者那里拉去数据
     * @param[in]  pINotificaion   INotificaiton对象操作指针
     * @return     成功返回OK     失败返回错误码
     */
    ERRORCODE_E (*Pull)(INotification *pINotificaion,const char* pubid,void* data,unsigned int size);

    /**@fn         Notify
     * @brief      发送通知给发布者
     * @param[in]  pINotificaion   INotificaiton对象操作指针
     * @return     成功返回OK     失败返回错误码
     */
    ERRORCODE_E (*Notify)(INotification *pINotificaionn,const char* pubid,void* data,unsigned int size);

    /**@fn         Release    
     * @brief      模块资源释放
     * @param[in]  pINotificaion   INotificaiton对象操作指针
     * @return     int
     */
    ERRORCODE_E (*Release)(INotification *pINotificaionn);

};




void init_notification_component(void);
INotification *notification_get_notifybroker(void);

#ifdef __cplusplus
}
#endif/*__cplusplus*/



#endif