#ifndef _STORAGER_H_
#define _STORAGER_H_

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/



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

IStorager *storage_manager_init_instance(void);









#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif