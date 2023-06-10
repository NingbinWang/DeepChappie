#ifndef _STORAGER_H_
#define _STORAGER_H_

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

#define DIRNAMESTRMAX 256
#define DIRNUMMAX     6
#define TYPESTRMAX 64

typedef struct 
{
    int enable;
    int tfid;
    int emmcid;
    unsigned int dirnum;//根目录下有多少个文件夹
    char dirname[DIRNUMMAX][DIRNAMESTRMAX];//相关的名字
}Storager_Info_T;


typedef struct 
{
    int enable;
}Storager_Notifybroker_T;

Storager_Notifybroker_T storager_notifybroker;




#define STORAGERPUBID "strorager"

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

IStorager *storager_manager_init_instance(void);









#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif