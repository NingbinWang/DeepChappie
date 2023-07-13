#ifndef _STORAGER_H_
#define _STORAGER_H_

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

#include "Notification.h"

#define DIRNAMESTRMAX 256
#define DIRNUMMAX     6
#define TYPESTRMAX 64





typedef struct
{
    int                   iMediumNo;          // 介质ID
    int                   iMediumPart;        // 介质分区号
}STORAGER_ID_T;

typedef enum
{
    STORAGER_STATE_ABNORMAL = 0,              //存储介质异常
    STORAGER_STATE_NORMAL = 1,                //存储介质正常
    STORAGER_STATE_MEM_FULL = 2,              //存储卡满异常
    STORAGER_STATE_UNINITIALIZED = 3,         //未初始化,需要格式化
    STORAGER_STATE_REMOVEED = 4,         //存储卡被拔出
}STORAGER_STATE_E;



#define STORAGERPUBID "storager"

typedef struct IStorager IStorager;

struct IStorager
{
    /**@fn         Init   
     * @brief      初始化参数
     * @param[in]  IStorager   设备监听对象
     * @return     成功返回OK     失败返回错误码
     */
    int (*Init)(IStorager *pIStorager);


    


};

IStorager *storager_manager_get_instance(void);

void init_storager_manager_component(void);







#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif