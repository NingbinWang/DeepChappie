#include "storager_manager_priv.h"


 /**@fn	       storage_manager_get_priv_data	  
 * @brief	   获取串口对象私有数据 
 * @param[in]  pIDevMonitor      串口对象操作指针
 * @return	   成功 私有数据指针 失败 NULL
 */

static STORAGER_MANAGER_PRIV_DATA_T *storage_manager_get_priv_data(IStorager *pIStorager)
{
    STORAGER_MANAGER_BASE_T *pBase =NULL;
    if(!pIStorager)
    {
        LOGGER_ERROR("invaild param \n");
        return NULL;
    }
    pBase = (STORAGER_MANAGER_BASE_T*)pIStorager;
    return &pBase->stPrivData;
}



/**@fn	       storage_manager_init	  
 * @brief	   初始化对外接口
 * @param[in]  无
 * @return	   成功返回IDevMonitor操作指针  失败返回NULL;
 */
static INT32 storager_manager_init(IStorager *pIStorager)
{   
    INT32 iRet = ERROR;
    if(NULL == pIStorager)
    {
        LOGGER_ERROR("init interface func error\n");
        return iRet;
    }

    return OK;
}


/**@fn	       storage_manager_onevent
 * @brief	   初始化对外接口
 * @param[in]  无
 * @return	   成功返回IDevMonitor操作指针  失败返回NULL;
 */
static INT32 storage_manager_onevent(IStorager *pIStorager,EventCallback_t pCallback)
{ 
    INotification* notifybroker = NULL;
    if(NULL == pIStorager)
    {
        LOGGER_ERROR("init interface func error\n");
        return ERROR;
    }
    if(NULL == pCallback)
    {
        LOGGER_ERROR("no suppor now \n");
        return ERROR;
    }
    notifybroker =  notification_get_notifybroker();
    notifybroker->SetEventCallback(notifybroker,STORAGERPUBID,pCallback);
    return OK;
}





/**@fn	       storage_manager_init_priv_data	  
 * @brief	   初始化私有数据
 * @param[in]  pStPrivData          私有数据指针
 * @return	   无
 */
static INT32 storager_manager_init_priv_data(STORAGER_MANAGER_PRIV_DATA_T *pStPrivData)
{
    INotification* notifybroker = NULL;
    if(!pStPrivData)
    {
        LOGGER_ERROR("init pPriv Data error\n");
        return ERROR;
    }
    storager_notifybroker = (Storager_Notifybroker_T *)sys_mem_malloc(sizeof(Storager_Notifybroker_T));
    notifybroker =  notification_get_notifybroker();
    if(notifybroker !=NULL)
      notifybroker->Subscribe(notifybroker,STORAGERPUBID,(const void *)storager_notifybroker,sizeof(Storager_Notifybroker_T));

    return OK;
}



/**@fn	       storage_manager_init_interface	  
 * @brief	   初始化对外接口
 * @param[in]  无
 * @return	   成功返回IDevMonitor操作指针  失败返回NULL;
 */
static INT32 storager_manager_init_interface(IStorager *pIStorager)
{   
    INT32 iRet = ERROR;
    
    if(NULL == pIStorager)
    {
        LOGGER_ERROR("init interface func error\n");
        return iRet;
    }
    pIStorager->Init=storager_manager_init;
    pIStorager->OnEvent=storage_manager_onevent;



    
    return OK;
}

/**@fn	       storage_manager_init_instance	  
 * @brief	   初始化构造单例
 * @param[in]  无
 * @return	   成功返回对象指针,失败返回NULL
 */
IStorager *storager_manager_init_instance(VOID)
{   
    INT32 iRet = ERROR;
    STORAGER_MANAGER_BASE_T *pBase = NULL;
    pBase = (STORAGER_MANAGER_BASE_T *)sys_mem_malloc(sizeof(STORAGER_MANAGER_BASE_T));
    if(!pBase)
    {
        LOGGER_ERROR("malloc errror \n");
        return NULL;
    }
    memset(pBase,0,sizeof(STORAGER_MANAGER_BASE_T));
    //初始化对外接口
    iRet = storager_manager_init_interface(&pBase->stInterface);
    if(iRet < 0)
    {   
        LOGGER_ERROR("init interface error \n");
        sys_mem_free(pBase);
        pBase = NULL;
        return NULL;
    }

    //初始化私有数据
    iRet = storager_manager_init_priv_data(&pBase->stPrivData);
    if(iRet < 0)
    {   
        LOGGER_ERROR("init priv data error \n");
        sys_mem_free(pBase);
        pBase = NULL;
        return NULL;
    }
    return &pBase->stInterface;
}

void init_storager_manager_component(void)
{
    storagermanager = storager_manager_init_instance();
    if(storagermanager == NULL)
    {
        LOGGER_ERROR("notification_init_instance error \n");
    }
}

IStorager *storager_manager_get_instance(void)
{
    return storagermanager;
}

Storager_Notifybroker_T *storager_manager_get_broker()
{
    return storager_notifybroker;
}