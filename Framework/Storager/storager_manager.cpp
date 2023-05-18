#include "storager_manager_priv.h"


 /**@fn	       storage_manager_get_priv_data	  
 * @brief	   获取串口对象私有数据 
 * @param[in]  pIDevMonitor      串口对象操作指针
 * @return	   成功 私有数据指针 失败 NULL
 */
/*
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

*/


/**@fn	       storage_manager_common_check_and_create_dir_exist	  
 * @brief	   检查并创建目录
 * @param[in]  iMediumNo   介质ID
 * @return	   成功返回OK  失败返回错误码
 */
/*
INT32 storage_manager_check_and_create_dir_exist(INT32 iMediumNo, MEDIUM_STATE_E eStates)
{
    INT32 iRet = ERROR; 
    INT32 iIndex = 0;
    CONFIG_STORAGE_T stConfig;
    IFileManager *pIFileManager = NULL;
    STORAGE_MANAGER_PRIV_DATA_T *pStPrivData = NULL;

    pStPrivData = storage_manager_get_priv_data((IStorageManager*)storage_manager_get_instance());
    if(NULL == pStPrivData)
    {
        STORAGE_ERROR("get priv data error \n");
        return ERROR;
    }

    if(eStates != MEDIUM_STATE_NORMAL && MEDIUM_STATE_MEM_FULL != eStates)
    {
        return iRet;
    }
    
    iRet = storage_manager_get_storage_config(&stConfig);
    if(iRet < 0)
    {
        STORAGE_ERROR("storage_manager_get_storage_config failed\n");
        return iRet;
    }
    
    pIFileManager = FACTORY_GET_COMPONENT(IFileManager,FACTORY_IID_FILE_MANAGER,FACTORY_CID_DEFAULT);
    if(!pIFileManager)
    {   
        STORAGE_ERROR("pIFileManager is NULL\n");
        return iRet;
    }

    for(iIndex = 0;iIndex < ARRAY_SIZE(stConfig.stParam.aDirAttr);iIndex++)
    {
        if((FALSE == pStPrivData->bOmcVedio) && 
			((CONFIG_STORAGE_ATTR_NORMAL == stConfig.stParam.aDirAttr[iIndex].eAttr) || (CONFIG_STORAGE_ATTR_EVENT == stConfig.stParam.aDirAttr[iIndex].eAttr)))
        {
		    STORAGE_INFO("not support omc record, not create dir:%s \n", stConfig.stParam.aDirAttr[iIndex].strDir);
			continue;
		}
        if(stConfig.stParam.aDirAttr[iIndex].bValid == FALSE)
        {
            continue;
        }
        
        iRet = pIFileManager->CreateDir(pIFileManager,iMediumNo,stConfig.stParam.aDirAttr[iIndex].strDir);
        if(iRet < 0)
        {
            STORAGE_ERROR("create dir:%s failed\n", stConfig.stParam.aDirAttr[iIndex].strDir);
            break;
        }
        STORAGE_INFO("create dir:%s successful\n", stConfig.stParam.aDirAttr[iIndex].strDir);
    }
    return iRet;
}
*/




/**@fn	       storage_manager_init_priv_data	  
 * @brief	   初始化私有数据
 * @param[in]  pStPrivData          私有数据指针
 * @return	   无
 */
static INT32 storager_manager_init_priv_data(STORAGER_MANAGER_PRIV_DATA_T *pStPrivData)
{
    if(!pStPrivData)
    {
        LOGGER_ERROR("init pPriv Data error\n");
        return ERROR;
    }

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