#ifndef _MEDIA_H_
#define _MEDIA_H_

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

#define MEDIASTRMAX 256
typedef struct
{
   int  enable;
   char type[MEDIASTRMAX];//media的启动类型
}Media_Info_T;



typedef struct IMedia IMedia;

struct IMedia
{
    /**@fn         Init   
     * @brief      初始化参数
     * @param[in]  pIMedia   初始化的对象
     * @return     成功返回OK     失败返回错误码
     */
    int (*Init)(IMedia *pIMediainit,const char* type);
    
};



#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif