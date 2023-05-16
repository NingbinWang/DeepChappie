#ifndef _MEDIA_H_
#define _MEDIA_H_

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/




typedef struct IMedia IMedia;

struct IMedia
{
    /**@fn         Init   
     * @brief      初始化参数
     * @param[in]  pIMedia   初始化的对象
     * @return     成功返回OK     失败返回错误码
     */
    int (*Init)(IMedia *pIMediainit);
};



#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif