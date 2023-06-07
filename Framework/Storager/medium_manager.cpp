#include "medium_manager_priv.h"

/**@fn	       medium_manager_get_priv_data
 * @brief	   获取私有数据指针
 * @param[in]  pIMediumManager  介质对象操作函数
 * @return	   成功返回OK  失败返回错误码
 */
MEDIUM_MANAGER_PRIV_DATA_T *medium_manager_get_priv_data(IMediumManager *pIMediumManager)
{
    MEDIUM_MANAGER_BASE_T *pStBase = NULL;
    
    if(!pIMediumManager)
    {
        LOGGER_ERROR("input prt null\n");
        return NULL;
    }
    
    pStBase = (MEDIUM_MANAGER_BASE_T*)pIMediumManager;
    return &pStBase->stPrivData;
}

/**@fn	       medium_manager_try_mount	  
 * @brief	   尝试挂载分区
 * @param[in]  iMediumNo     介质索引
 * @param[in]  iMediumPartNo   介质分区号
 * @return	   成功返回 0 失败返回-1 
 */
INT32 medium_manager_mount(const char *strMountPath, const char *strDevPath)
{
    INT32 iRet = -1;
    INT32 iIndex = 0;
    SYS_MEDIUM_FS_TYPE_E aMediumDefaultFS[] = {SYS_MEDIUM_FS_EXFAT,SYS_MEDIUM_FS_VFAT, SYS_MEDIUM_FS_EXT4, SYS_MEDIUM_FS_NTFS};

    if(NULL == strMountPath || NULL == strDevPath)
    {
        LOGGER_ERROR("invalid param input\n");
        return iRet;
    }
    for(iIndex = 0; iIndex < ARRAY_SIZE(aMediumDefaultFS); iIndex++)
    {
        iRet = sys_medium_mount(strDevPath, aMediumDefaultFS[iIndex], strMountPath);
        if(iRet < 0)
        {
            LOGGER_ERROR("mount %s %s ERROR:%s\n",strDevPath,strMountPath,strerror(sys_posix_get_last_errno()));
            continue;
        }
        LOGGER_INFO("mount successful, FileSystem(SYS_MEDIUM_FS_TYPE_E):%u\n", aMediumDefaultFS[iIndex]);
        break;
    }

    return iRet;
}

/**@fn	       medium_manager_mount_clear	  
 * @brief	   清理挂载信息
 * @param[in]  eType          格式化介质类型
 * @return	   成功 0 ,失败返回 -1
 */
VOID medium_manager_umount(const char *strMountDev)
{
    INT32 iRet = ERROR;

    if(NULL == strMountDev)
    {
        LOGGER_ERROR("invalid param input\n");
        return;
    }
    iRet = sys_medium_umount(strMountDev);
    if(OK != iRet)
    {
        LOGGER_ERROR("umount %s ERROR:%s\n",strMountDev,strerror(sys_posix_get_last_errno()));
        return;
    }
    sys_posix_rmdir(strMountDev);
}

/**@fn	       medium_manager_find_medium_node	  
 * @brief	   获取分区信息节点
 * @param[in]  pStPrivData     私有数据结构体指针
 * @param[in]  iMediumChn      介质对外通道号
 * @return	   成功返回节点数据指针 失败返回NULL
 */
MEDIUM_INFO_NODE_T *medium_manager_find_medium_node(MEDIUM_MANAGER_PRIV_DATA_T *pStPrivData,INT32 iMediumChn)
{
    MEDIUM_INFO_NODE_T *pStNode = NULL;
    if(!pStPrivData)
    {
        LOGGER_ERROR("input prt null\n");
        return NULL;
    }

    LIST_FOR_EACH(MEDIUM_INFO_NODE_T, pStNode, &pStPrivData->MediumInfoList)
    {
        if(iMediumChn != pStNode->iMediumNo)
        {
            continue;
        }

        break;
    }

    return pStNode;
}

/**@fn	       medium_manager_update_info	  
 * @brief	   更新介质节点信息
 * @param[in]  iMediumNo     介质索引
 * @param[in]  iMediumPartNo   介质分区号
 * @param[in]  eStates         介质状态类型
 * @return	   无
 */
VOID medium_manager_update_medium_info(MEDIUM_MANAGER_PRIV_DATA_T *pStPrivData,  MEDIUM_PART_INFO_T *pStPartNode,MEDIUM_STATE_E eStates)
{
    BOOL bUpData = FALSE;
    MEDIUM_INFO_NODE_T *pStNode = NULL;
    SYS_MEDIUM_INFO_T *pSysMediumInfo = NULL;
    if(!pStPrivData)
    {
        LOGGER_ERROR("medium_manager_get_priv_data failed\n");
        return ;
    }   
	if( NULL == pStPartNode )
	{
		LOGGER_ERROR("find medium part failed\n");
        return ;
	}
    pSysMediumInfo = (SYS_MEDIUM_INFO_T *)sys_mem_malloc(sizeof(SYS_MEDIUM_INFO_T));
    if(!pSysMediumInfo)
    {
        LOGGER_ERROR("sys_mem_malloc failed\n");
        return ;
    }
    LIST_FOR_EACH(MEDIUM_INFO_NODE_T, pStNode, &pStPrivData->MediumInfoList)
    {
        if(pStNode->iMediumNo != pStPartNode->uMediumId)
        {
            continue;
        }
	
        if(sys_medium_get_info(pStPartNode->strMountPath, pSysMediumInfo) < 0) 
        {
            LOGGER_ERROR("sys_medium_get_info, iMediumNo:%d\n", pStPartNode->uMediumId);
        }
        
        bUpData = pStNode->eStates != eStates ? TRUE : FALSE;
        sys_mutex_lock(&pStPrivData->MediumInfoMutex,WAIT_FOREVER);
        pStNode->eStates = eStates;
        pStNode->eFileType = pSysMediumInfo->eType;
        pStNode->uTotalSpaceSize = pSysMediumInfo->uTotalSize;
        pStNode->uRemainSpaceSize= pSysMediumInfo->uRemainSize;
        pStNode->uClusterSize = pSysMediumInfo->uClusterSize;
        sys_mutex_unlock(&pStPrivData->MediumInfoMutex);
        if(bUpData == FALSE)
        {
            LOGGER_INFO("medium iMediumNo:%d eStates:%d, need not update\n", 
                 pStPartNode->uMediumId, eStates);
            break;
        }
        break;
    }
}

/**@fn	       medium_manager_find_medium_part	  
 * @brief	   获取分区信息节点
 * @param[in]  pStPrivData   对象指针
 * @param[in]  iMediumNo     介质ID
 * @param[in]  iMediumPart   分区号
 * @return	   成功结点指针  失败空
 */
MEDIUM_PART_INFO_T *medium_manager_find_medium_part(MEDIUM_MANAGER_PRIV_DATA_T *pStPrivData, INT32 iMediumNo, INT32 iPartNo)
{
    MEDIUM_PART_INFO_T *pStNode = NULL;
    
    if( !pStPrivData )
    {
        LOGGER_ERROR("input prt null\n");
        return NULL;
    }
    
    LIST_FOR_EACH(MEDIUM_PART_INFO_T, pStNode, &pStPrivData->MediumPartInfoList)
    {
        if( iMediumNo != pStNode->uMediumId|| iPartNo != pStNode->uPartId)
        {
            continue;
        }

        break;
    }

    return pStNode;
}

/**@fn	       medium_manager_check_medium_read_write
 * @brief	   校验存储介质读写状态
 * @param[in]  iMediumNo     介质索引
 * @return	   存储状态
 */
INT32 medium_manager_check_medium_read_write(MEDIUM_PART_INFO_T  *pStPartNode)
{
    INT32                       iRet                  = ERROR;
    FILE_ID                    *pFileId               = NULL;
    UINT8                       pMagicNum[8]          = {'D', 'E', 'A', 'D', 'B', 'E', 'E', 'F'};
    UINT8                       pReadBuffer[8]        = {0};
    CHAR                        strTestFile[128 + 16] = {0};
    if (NULL == pStPartNode)
    {
        LOGGER_ERROR("find medium part failed\n");
        return ERROR;
    }

    snprintf(strTestFile, sizeof(strTestFile), "%s/rw.test", pStPartNode->strMountPath);

    // 写入数据
    pFileId = sys_file_fopen(strTestFile, "wb+");
    if (pFileId == NULL)
    {
        LOGGER_ERROR("ptr NULL\n");
        sys_posix_rm(strTestFile);
        return ERROR;
    }

    iRet = sys_file_fwrite(pFileId, pMagicNum, 1, sizeof(pMagicNum));
    if (iRet != sizeof(pMagicNum))
    {
        LOGGER_ERROR("sys_file_fwrite failed\n");
        sys_file_fclose(pFileId);
        sys_posix_rm(strTestFile);
        return ERROR;
    }

    iRet = sys_file_fsync(pFileId);
    if (iRet != OK)
    {
        LOGGER_ERROR("sys_file_fsync failed\n");
        sys_file_fclose(pFileId);
        sys_posix_rm(strTestFile);
        return ERROR;
    }

    iRet = sys_file_fclose(pFileId);
    if (iRet != OK)
    {
        LOGGER_ERROR("sys_file_fclose failed\n");
        sys_posix_rm(strTestFile);
        return ERROR;
    }
    pFileId = NULL;

    // 读出数据
    pFileId = sys_file_fopen(strTestFile, "rb");
    if (pFileId == NULL)
    {
        LOGGER_ERROR("ptr NULL\n");
        sys_posix_rm(strTestFile);
        return ERROR;
    }

    iRet = sys_file_fread(pFileId, pReadBuffer, 1, sizeof(pReadBuffer));
    if (iRet != sizeof(pMagicNum))
    {
        LOGGER_ERROR("sys_file_fread failed\n");
        sys_file_fclose(pFileId);
        sys_posix_rm(strTestFile);
        return ERROR;
    }

    iRet = sys_file_fclose(pFileId);
    if (iRet != OK)
    {
        LOGGER_ERROR("sys_file_fclose failed\n");
        sys_posix_rm(strTestFile);
        return ERROR;
    }

    sys_posix_rm(strTestFile);

    sys_posix_sync();

    // 校验数据
    if (memcmp(pReadBuffer, pMagicNum, sizeof(pMagicNum)) != 0)
    {
        LOGGER_ERROR("memcmp failed\n");
        return ERROR;
    }

    return OK;
}

/**@fn	       medium_manager_insert_handle	  
 * @brief	   介质插入动作联动操作
 * @param[in]  iMediumNo     介质索引
 * @param[in]  iMediumPartNo   介质分区号
 * @return	   无
 */
VOID medium_manager_insert_handle(MEDIUM_MANAGER_PRIV_DATA_T *pStPrivData,MEDIUM_MANAGER_MSG_T *pStMsg)
{
    INT32 iRet = -1;
    MEDIUM_STATE_E eState = MEDIUM_STATE_NORMAL;
    MEDIUM_PART_INFO_T *pStPartNode = NULL;
    if(NULL == pStMsg)
    {
        return ;
    }
    if(!pStPrivData)
    {
        LOGGER_ERROR("medium_manager_get_priv_data failed\n");
        return ;
    }
    pStPartNode = medium_manager_find_medium_part(pStPrivData, pStMsg->iMediumNo, pStMsg->iMediumPart);

    //检查主节点是否存在,不存在则需要格式化, 报真实的数据异常
    iRet  = sys_medium_check_dev_node(pStPartNode->strRootPath);
    if(iRet < 0)
    {   
        LOGGER_ERROR("check node no exist :%s  \n", pStPartNode->strRootPath);
        eState = MEDIUM_STATE_ABNORMAL;
        medium_manager_update_medium_info(pStPrivData,pStPartNode, eState);
        return ;
    }

    //检查分区节点是否存在,不存在则需要格式化, 报未格式化异常
    iRet  = sys_medium_check_dev_node(pStPartNode->strDevPath);
    if(iRet < 0)
    {   
        LOGGER_ERROR("check node no exist :%s  \n", pStPartNode->strDevPath);
        eState = MEDIUM_STATE_UNINITIALIZED;
        medium_manager_update_medium_info(pStPrivData,pStPartNode, eState);
        return ;
    }
    
    /* 存在分区节点, 先尝试性卸载目录 */
    sys_medium_umount(pStPartNode->strMountPath);
    
    /* 创建分区挂载目录，mmc%d%d，iMediumNo，iMediumPartNo */
    iRet = sys_posix_mkdir(pStPartNode->strMountPath);
    if(iRet < 0)
    {
        LOGGER_ERROR("medium note create mount path failed,  uMediumId:%d \n", pStPartNode->uMediumId);
        return ;
    }
    
    /* 尝试挂载目前支持的文件属性,后续考虑是否要由外界配置 */
    iRet = medium_manager_mount(pStPartNode->strMountPath, pStPartNode->strDevPath);
    if(iRet < 0)
    {
        LOGGER_ERROR("medium_manager_try_mount failed, uMediumId:%d \n", 
            pStPartNode->uMediumId);
        sys_posix_rmdir(pStPartNode->strMountPath);
        eState = MEDIUM_STATE_SYSTEM_NO_SUPPORT;
        medium_manager_update_medium_info(pStPrivData,pStPartNode, eState);
        return ;
    }

    /* 校验介质读写是否正常 */
    iRet = medium_manager_check_medium_read_write(pStPartNode);
    if(iRet < 0)
    {
        LOGGER_ERROR("medium_manager_check_medium_read_write failed, uMediumId:%d \n", 
            pStPartNode->uMediumId);
        eState = MEDIUM_STATE_ABNORMAL;
        medium_manager_update_medium_info(pStPrivData,pStPartNode, eState);
        return;
    }
    
    LOGGER_INFO("medium insert handle completed, uMediumId:%d  eState(MEDIUM_STATE_E):%u\n",
        pStPartNode->uMediumId, eState);
 }  

/**@fn	       medium_manager_remove_handle	  
 * @brief	   介质拔出动作联动操作
 * @param[in]  iMediumNo     介质索引
 * @param[in]  iMediumPartNo   介质分区号
 * @return	   无
 */
VOID medium_manager_remove_handle(MEDIUM_MANAGER_PRIV_DATA_T *pStPrivData,MEDIUM_MANAGER_MSG_T *pStMsg)
{ 
	MEDIUM_PART_INFO_T *pStPartNode = NULL;
    if(NULL == pStMsg)
    {
        return ;
    }
	 if(!pStPrivData)
    {
        LOGGER_ERROR("medium_manager_get_priv_data failed\n");
        return ;
    }

	pStPartNode = medium_manager_find_medium_part(pStPrivData, pStMsg->iMediumNo, pStMsg->iMediumPart);
	if( NULL == pStPartNode )
	{
		LOGGER_ERROR("find medium part failed\n");
        return ;
	}
	
    //update info  先更新卡状态信息
    medium_manager_update_medium_info(pStPrivData,pStPartNode,  MEDIUM_STATE_NOT_EXIST);   
    //卸载文件目录
    sys_medium_umount(pStPartNode->strMountPath);
    
    //删除挂载文件路径
    sys_posix_rmdir(pStPartNode->strMountPath);
}


/**@fn	       medium_manager_change_handle	  
 * @brief	   介质拔出变更联动操作
 * @param[in]  iMediumNo     介质索引
 * @param[in]  iMediumPartNo   介质分区号
 * @return	   无
 */
VOID medium_manager_change_handle(MEDIUM_MANAGER_PRIV_DATA_T *pStPrivData,MEDIUM_MANAGER_MSG_T *pStMsg)
{ 
	MEDIUM_INFO_NODE_T *pStNode = NULL;
    if(NULL == pStMsg)
    {
        return ;
    }
	 if(!pStPrivData)
    {
        LOGGER_ERROR("medium_manager_get_priv_data failed\n");
        return ;
    }
    pStNode = medium_manager_find_medium_node(pStPrivData, pStMsg->iMediumNo);
    if(!pStNode)
    {
        LOGGER_ERROR("medium_manager_get_info_node failed, iMediumChan:%d  \n", pStMsg->iMediumNo);
        return;
    }

    if(pStNode->eStates == MEDIUM_STATE_FORMATTING)
       sys_mutex_unlock(&pStPrivData->MediumformatMutex);

}



/**@fn	       medium_manager_action_monitor_thread	  
 * @brief	   介质插拔状态监听线程
 * @param[in]  pStPrivData  私有数据指针
 * @return	   成功返回OK  失败返回错误码
 */

static VOID medium_manager_action_monitor_thread(MEDIUM_MANAGER_PRIV_DATA_T *pStPrivData)
{   
    INT32 iRet = ERROR;
    MEDIUM_MANAGER_MSG_T stMediumMsg = {0};
    
    if(!pStPrivData)
    {
        LOGGER_ERROR("invaild param \n");
        return ;
    }
    
    while(1)
    {   
        memset((char *)&stMediumMsg, 0, sizeof(MEDIUM_MANAGER_MSG_T));
        iRet = sys_mqueue_recv(&pStPrivData->stMediumMsgID, (char *)&stMediumMsg, sizeof(MEDIUM_MANAGER_MSG_T), WAIT_FOREVER);
        if(iRet < 0)
        {
            continue;
        }
        LOGGER_INFO("iMediumNo:%d iMediumPart:%d type :%d eAction:%d\n", stMediumMsg.iMediumNo, stMediumMsg.iMediumPart, stMediumMsg.eType, stMediumMsg.eAction);
        switch(stMediumMsg.eAction)
        {
            case MEDIUM_STATE_ACTION_ADD:
                medium_manager_insert_handle(pStPrivData,&stMediumMsg);
                break;
            case MEDIUM_STATE_ACTION_REMOVE:
                medium_manager_remove_handle(pStPrivData,&stMediumMsg);
                break;
            case MEDIUM_STATE_ACTION_CHANGE:
                medium_manager_change_handle(pStPrivData,&stMediumMsg);
                break;
            default:
                LOGGER_ERROR("type no support :%d \n",stMediumMsg.eAction);
                break;
        }
    }
}


/**@fn         RegisterActionCallBack      
 * @brief      注册设备监听回调函数
 * @param[in]  eType         设备监听类型
 * @param[in]  iDevIndex     设备序列号索引
 * @param[in]  iPartIndex    子序号索引，0为无效值
 * @param[in]  eAction       设备动作
 * @return     无
 */
VOID medium_manager_msg_callback(DEV_MONITOR_TYPE_E eType, INT32 iDevIndex, INT32 iPartIndex, DEV_MONITOR_ACTION_E eAction)
{
    MEDIUM_MANAGER_PRIV_DATA_T *pStPrivData = NULL;
    MEDIUM_MANAGER_MSG_T stMediumMsg = {0};
    IMediumManager *pIMediumManager = NULL;
    pIMediumManager = Framework_Getmedium();
     INT32 iRet = ERROR;
    if(pIMediumManager == NULL){
         LOGGER_ERROR("Framework_Getmedium failed\n");
         return;
    }
    pStPrivData = medium_manager_get_priv_data(pIMediumManager);
    if(!pStPrivData)
    {
        LOGGER_ERROR("medium_manager_get_priv_data failed\n");
        return;
    }

    if((eType != DEV_MONITOR_TYPE_TF) && (eType != DEV_MONITOR_TYPE_HARD_DISK))
    {
        LOGGER_WARN("ignore dev type :%d \n",eType);
        return ;
    }
    stMediumMsg.iMediumNo = iDevIndex;
    stMediumMsg.iMediumPart = iPartIndex;

    if(eAction == DEV_MONITOR_ACTION_REMOVE)
    {
        stMediumMsg.eAction = MEDIUM_STATE_ACTION_REMOVE;
    }
    else if(eAction == DEV_MONITOR_ACTION_ADD)
    {
        stMediumMsg.eAction = MEDIUM_STATE_ACTION_ADD;
    }
    else if(eAction == DEV_MONITOR_ACTION_CHANGE)
    {
        stMediumMsg.eAction = MEDIUM_STATE_ACTION_CHANGE;
    }
    else
    {
        stMediumMsg.eAction = MEDIUM_STATE_ACTION_UNKNOWN;
    }
    if(eType == DEV_MONITOR_TYPE_HARD_DISK)
    {
        stMediumMsg.eType = MEDIUM_TYPE_HARD_DISK;
    }
    else
    {
        stMediumMsg.eType = MEDIUM_TYPE_TF;
    }
    
    LOGGER_INFO("medium_manager_msg_callback, iMediumNo:%d iMediumPart:%d eType:%d eAction:%u\n",
        stMediumMsg.iMediumNo, stMediumMsg.iMediumPart, stMediumMsg.eType, stMediumMsg.eAction);
    iRet = sys_mqueue_send(&pStPrivData->stMediumMsgID, (CHAR *)&stMediumMsg, sizeof(MEDIUM_MANAGER_MSG_T), NO_WAIT);
    if(iRet < 0)
    {
        LOGGER_ERROR("sys_mqueue_send start msg error \n");
    }
}



/**@fn	       medium_manager_check_capacity	  
 * @brief	   容量判断
 * @param[in]  iMediumNo     介质索引
 * @param[in]  iMediumPartNo   介质分区号
 * @return	   成功返回 OK 失败返回ERROR
 */
INT32 medium_manager_check_capacity(MEDIUM_PART_INFO_T *pStPartNode,MEDIUM_INFO_NODE_T *pStNode)
{
    INT32 iRet = ERROR;
    SYS_MEDIUM_INFO_T* pInfo = {0};
	if( NULL == pStNode || NULL == pStPartNode )
	{
		LOGGER_ERROR("invalid param input \n");
        return iRet;
	}
     pInfo = (SYS_MEDIUM_INFO_T *)sys_mem_malloc(sizeof(SYS_MEDIUM_INFO_T));
    if(!pInfo)
    {
        LOGGER_ERROR("sys_mem_malloc failed\n");
        return iRet;
    }
    iRet = sys_medium_get_info(pStPartNode->strMountPath, pInfo);
    if(iRet < 0)
    {   
        LOGGER_ERROR("sys_medium_get_info, iMediumNo:%d \n",pStPartNode->uMediumId);
        return iRet;
    }
    pStNode->uTotalSpaceSize = pInfo->uTotalSize;
    pStNode->uRemainSpaceSize = pInfo->uRemainSize; 
    pStNode->eFileType = pInfo->eType;
    pStNode->uClusterSize = pInfo->uClusterSize;
    return OK;
}


/**@fn	       medium_manager_add_medium_node_info	  
 * @brief	   获取介质节点信息
 * @param[in]  pStPartInfoNode 分区节点
 * @param[in]  eStates       介质状态类型
 * @return	   无
 */
VOID medium_manager_add_medium_node_info(MEDIUM_MANAGER_PRIV_DATA_T *pStPrivData,MEDIUM_PART_INFO_T *pStPartNode,MEDIUM_ACTION_E eAction)
{
    MEDIUM_INFO_NODE_T *pStNode = NULL;
   
    if(NULL == pStPartNode)
    {
        LOGGER_ERROR("invalid param input\n");
        return;
    }
    if(NULL == pStPrivData)
    {
        LOGGER_ERROR("medium_manager_get_priv_data failed\n");
        return;
    }

    pStNode = medium_manager_find_medium_node(pStPrivData, pStPartNode->uMediumId);
    if(pStNode)
    {
        LOGGER_ERROR("medium is already exist,  uMediumChan = %d i\n", pStPartNode->uMediumId);
        return;
    }

    pStNode = (MEDIUM_INFO_NODE_T*)sys_mem_malloc(sizeof(MEDIUM_INFO_NODE_T));
    if(!pStNode)
    {
        LOGGER_ERROR("sys_mem_malloc failed\n");
        return;
    }
    memset(pStNode,0,sizeof(MEDIUM_INFO_NODE_T));
    pStNode->iMediumNo  = pStPartNode->uMediumId;
    pStNode->iPartNo    = pStPartNode->uPartId;
    pStNode->eStates    = pStPartNode->eStates;
    pStNode->eType      = pStPartNode->uMeidumType;
    pStNode->eAction    = eAction;
    if(MEDIUM_ACTION_UNKNOWN == eAction)
    {
        medium_manager_check_capacity(pStPartNode,pStNode);
    }
    sys_mutex_lock(&pStPrivData->MediumInfoMutex,WAIT_FOREVER);
    list_add(&pStPrivData->MediumInfoList,&pStNode->node);
    sys_mutex_unlock(&pStPrivData->MediumInfoMutex);
    LOGGER_INFO("add medium info, uMediumId:%d eStates:%d uMeidumType:%d \n",pStNode->iMediumNo, pStPartNode->eStates, pStPartNode->uMeidumType);
}



/**@fn	       medium_manager_init_dev_path	  
 * @brief	   初始化分区节点路径
 * @param[in]  eType     介质类型
 * @param[in]  pStNode   分区节点
 * @return	   无
 */
INT32 medium_manager_init_dev_path(MEDIUM_TYPE_E eType, MEDIUM_PART_INFO_T *pStNode)
{
    CHAR strDriver = 'a';
	if( NULL == pStNode )
	{
		return ERROR;
	}
	switch(eType)
	{
		case MEDIUM_TYPE_TF:
            snprintf(pStNode->strDevPath, MEDIUM_MOUNT_PATH_LEN, MEDIUM_TF_DEV_PART_PATH, pStNode->uMediumId, pStNode->uPartId);
            snprintf(pStNode->strRootPath, MEDIUM_MOUNT_PATH_LEN,MEDIUM_TF_DEV_NODE_PATH, pStNode->uMediumId);
            snprintf(pStNode->strMountPath, MEDIUM_MOUNT_PATH_LEN,"/tmp/tf%d%d", pStNode->uMediumId,pStNode->uPartId);
        break;
		case MEDIUM_TYPE_HARD_DISK:
        if( 0 == pStNode->uPartId )
        {
            strDriver += pStNode->uMediumId;
            snprintf(pStNode->strDevPath, MEDIUM_MOUNT_PATH_LEN, MEDIUM_DISK_DEV_NODE_PATH, strDriver);
        }
        else
        {   
            strDriver += pStNode->uMediumId;
            snprintf(pStNode->strDevPath, MEDIUM_MOUNT_PATH_LEN, MEDIUM_DISK_DEV_PART_PATH, strDriver, pStNode->uPartId);
        }
        break;
        case MEDIUM_TYPE_EMMC:
                snprintf(pStNode->strDevPath, MEDIUM_MOUNT_PATH_LEN, MEDIUM_EMMC_DEV_PART_PATH, pStNode->uMediumId, pStNode->uPartId);
        break;

		default:
			LOGGER_ERROR("MEDIUM_TYPE_E error\n");
			break;
	}
	return OK;
}


/**@fn	       medium_manager_add_medium_part_info	  
 * @brief	   存储组件初始化当前介质状态信息
 * @param[in]  pStPrivData
 * @return	   无
 */
MEDIUM_PART_INFO_T *medium_manager_add_medium_part_info(MEDIUM_MANAGER_PRIV_DATA_T *pStPrivData,UINT8 uMediumId,UINT8 uPartId,MEDIUM_TYPE_E uMeidumType)
{
	MEDIUM_PART_INFO_T *pStNode = NULL;
	if( NULL == pStPrivData)
	{
		return NULL;
	}
	pStNode = (MEDIUM_PART_INFO_T*)sys_mem_malloc(sizeof(MEDIUM_PART_INFO_T));
	if(!pStNode)
	{
		LOGGER_ERROR("sys_mem_malloc failed\n");
		return NULL;
	}
	memset(pStNode,0,sizeof(MEDIUM_PART_INFO_T));
    pStNode->uMediumId   = uMediumId;
    pStNode->uPartId     = uPartId;
	if( medium_manager_init_dev_path(pStNode->uMeidumType,pStNode) < 0 )
	{
		LOGGER_ERROR("init dev path error\n");
	}
	list_add(&pStPrivData->MediumPartInfoList,&pStNode->node);
	return pStNode;
}

/**@fn	       medium_manager_init	  
 * @brief	   初始化构造对外接口
 * @param[in]  pIMediumManager      介质管理对象指针
 * @return	   成功返回OK  失败返回错误码
 */
INT32  medium_manager_init(IMediumManager *pIMediumManager,Storager_Info_T* storagerinfo)
{
    MEDIUM_MANAGER_PRIV_DATA_T *pStPrivData = NULL;
    IDevMonitor *pIDevMonitor = NULL;
    INT32 iRet = ERROR;
    CHAR strNodePath[128] = {0};
    MEDIUM_PART_INFO_T *pStNode = NULL;
    if(!storagerinfo)
    {
        LOGGER_ERROR("medium_manager_get_priv_data failed\n");
        return iRet;
    }
    pStPrivData = medium_manager_get_priv_data(pIMediumManager);
    if(!pStPrivData)
    {
        LOGGER_ERROR("medium_manager_get_priv_data failed\n");
        return iRet;
    }

    if(storagerinfo->tfid != -1)
    {
        //初始化介质配置能力
		pStNode =  medium_manager_add_medium_part_info(pStPrivData, (UINT8)storagerinfo->tfid,1,MEDIUM_TYPE_TF);
		if( NULL == pStNode )
		{
			LOGGER_ERROR("medium_manager_add_medium_part_info failed\n");
			return iRet;
		}
        //检查根节点
        iRet = sys_medium_check_dev_node(pStNode->strRootPath);
        if(iRet < 0)            /* 若未检测到介质节点，则节点状态为异常 */
        {
                pStNode->eStates = MEDIUM_STATE_NOT_EXIST;
                LOGGER_ERROR("sys_medium_check_dev_node failed, %s\n", pStNode->strRootPath);
        }else{
            //检查分区节点
             iRet = sys_medium_check_dev_node(pStNode->strDevPath);
             if(iRet < 0)            /* 若未检测到介质节点，则节点状态为异常 */
             {
                pStNode->eStates = MEDIUM_STATE_UNINITIALIZED;
                LOGGER_ERROR("sys_medium_check_dev_node failed, %s\n", pStNode->strDevPath);
             }else{
                pStNode->eStates = MEDIUM_STATE_SYSTEM_NO_SUPPORT;//初始设置为还未支持文件系统
             }
        }
         // 检测到节点，则定义为节点状态为未初始化，并尝试创建挂载目录
         if(pStNode->eStates == MEDIUM_STATE_SYSTEM_NO_SUPPORT)
         {
         	iRet = sys_posix_mkdir(pStNode->strMountPath);
            if(iRet < 0)
            {   
                LOGGER_ERROR("sys_posix_mkdir failed, %s  \n",pStNode->strMountPath);
            }
            sys_medium_umount(pStNode->strMountPath);
            iRet = medium_manager_mount(pStNode->strMountPath,pStNode->strDevPath);
            if(iRet < 0)
            {
                LOGGER_ERROR("medium_manager_try_mount failed\n");
                sys_posix_rmdir(pStNode->strMountPath);
            }else{
                pStNode->eStates = MEDIUM_STATE_NORMAL;
            }
         }
         //增加管理信息节点
         medium_manager_add_medium_node_info(pStPrivData,pStNode,MEDIUM_ACTION_UNKNOWN);
    }
   
    return OK;
}

/**@fn	       medium_manager_start	  
 * @brief	   开启介质管理
 * @param[in]  pIMediumManager      介质管理对象指针
 * @param[in]  pStMediumInitParam   介质管理初始化参数结构体指针
 * @return	   成功返回OK  失败返回错误码
 */
INT32 medium_manager_start(IMediumManager *pIMediumManager)
{
    INT32 iRet = ERROR;
    IDevMonitor *pIDevMonitor = NULL;
     pIDevMonitor = Framework_GetDevMonitor();
    if(pIDevMonitor == NULL){
         LOGGER_ERROR("Framework_GetDevMonitor failed\n");
        return iRet;
    }
    pIDevMonitor->RegisterActionCallBack(pIDevMonitor, DEV_MONITOR_TYPE_TF, medium_manager_msg_callback);
}


/**@fn	       medium_manager_format_part	  
 * @brief	   格式化分区操作
 * @param[in]  eType          格式化介质类型
 * @return	   成功 0 ,失败返回 -1
 */
INT32 medium_manager_format_part(MEDIUM_MANAGER_PRIV_DATA_T *pStPrivData, MEDIUM_PART_INFO_T *pStPartInfo, MEDIUM_FORMAT_TYPE_E eType, UINT32 uClusterSize)
{
    INT32  iRet = ERROR;
    SYS_MEDIUM_FS_TYPE_E eFsType = SYS_MEDIUM_FS_TYPE_UNKNOWN;

    LOGGER_INFO("start format,dev node:%s mount:%s\n",pStPartInfo->strDevPath,pStPartInfo->strMountPath);
    eFsType = SYS_MEDIUM_FS_VFAT;
    /* 格式化分区 */
    iRet = sys_medium_format(pStPartInfo->strDevPath, eFsType, uClusterSize);
    if(iRet < 0)
    {
        LOGGER_ERROR("format strDevPath:%s strMountPath:%s uMediumId:%d part:%d  eFsType:%d ERROR:%s\n",pStPartInfo->strDevPath,
                         pStPartInfo->strMountPath,pStPartInfo->uMediumId,pStPartInfo->uPartId,eFsType,strerror(sys_posix_get_last_errno()));
        return iRet;
    }

    /* 创建路径挂载路径 */
    iRet = sys_posix_mkdir(pStPartInfo->strMountPath);
    if(iRet < 0)
    {
        LOGGER_ERROR("sys_posix_mkdir strMountPath:%s strDevPath:%s error\n",pStPartInfo->strMountPath,pStPartInfo->strDevPath);
        return iRet;
    }
    
    /* 尝试挂载 */
    iRet = medium_manager_mount(pStPartInfo->strMountPath, pStPartInfo->strDevPath);
    if(iRet < 0)
    {
        LOGGER_ERROR("medium_manager_try_mount strDevPath:%s strMountPath:%s uMediumId:%d part:%d :%d error \n",pStPartInfo->strDevPath,
                        pStPartInfo->strMountPath,pStPartInfo->uMediumId,pStPartInfo->uPartId,eFsType);
        sys_posix_rmdir(pStPartInfo->strMountPath);
        return iRet;
    }

    return iRet;
}

/**@fn	       medium_manager_format_handle	  
 * @brief	   格式化处理函数
 * @param[in]  pStPrivData      私有数据指针
 * @param[in]  eType          格式化介质类型
 * @return	   成功 0 ,失败返回 -1
 */
INT32 medium_manager_format_handle_tf_card(MEDIUM_MANAGER_PRIV_DATA_T *pStPrivData, MEDIUM_PART_INFO_T *pStPartNode, MEDIUM_FORMAT_TYPE_E eType)
{
    INT32  iRet = ERROR;
    CHAR strDriver ='a';
    MEDIUM_STATE_E eState = MEDIUM_STATE_UNKNWON;
    UINT32 uClusterSize = 0;
	CHAR strSysFormatCmd[64] = {0};
	CHAR strNodePath[128] = {0};
	
    if(!pStPrivData)
    {
        LOGGER_ERROR("input prt null\n");
        return iRet;
    }
    
    /* 判断主介质是否存在 */
    iRet = sys_medium_check_dev_node(pStPartNode->strRootPath);
    if(iRet < 0)
    {
        LOGGER_ERROR("main dev node is not exist, iMediumNo = %d\n", pStPartNode->uMediumId);
        return iRet;
    }
    eState = MEDIUM_STATE_FORMATTING;
    medium_manager_update_medium_info(pStPrivData,pStPartNode, eState);
    medium_manager_umount(pStPartNode->strMountPath);
    /* 制造分区，内存卡默认格式化为1个分区 */
    if( pStPartNode->uPartId != 1 )
    {
        LOGGER_ERROR("unsupport tf card iPartNo:%d \n",pStPartNode->uPartId);
        medium_manager_update_medium_info(pStPrivData,pStPartNode, MEDIUM_STATE_UNINITIALIZED);
        return ERROR;
    }

    iRet = sys_medium_make_part(pStPartNode->strRootPath, pStPartNode->uPartId);
    if( iRet < 0 )
    {
        LOGGER_ERROR("sys_medium_make_part :%d error \n", pStPartNode->uMediumId);
        medium_manager_update_medium_info(pStPrivData,pStPartNode, MEDIUM_STATE_ABNORMAL);
        return iRet;
    }
    sys_mutex_lock(&pStPrivData->MediumformatMutex,200);

    iRet = sys_medium_check_dev_node(pStPartNode->strDevPath);
    if(iRet < 0)
    {
            LOGGER_ERROR("dev node is not existed\n");
			medium_manager_update_medium_info(pStPrivData,pStPartNode, MEDIUM_STATE_NOT_EXIST);
            return iRet;
    }
    uClusterSize = SIZE_4KB;
    /* 格式化分区 */
    iRet = medium_manager_format_part(pStPrivData, pStPartNode, eType, uClusterSize);
    if(iRet < 0)
    {
        LOGGER_ERROR("medium_manager_format_part failed\n");
        medium_manager_update_medium_info(pStPrivData,pStPartNode,MEDIUM_STATE_ABNORMAL);
        return iRet;
    }

   
    /* 并更新介质状态 */
    eState = MEDIUM_STATE_NORMAL;
    medium_manager_update_medium_info(pStPrivData,pStPartNode, eState);
    
    sys_time_sleep_ms(200);
    return iRet;
}

/**@fn	       Format	  
 * @brief	   介质格式化
 * @param[in]  pIMediumManager     介质管理对象指针
 * @param[in]  iMediumNo           介质所有号
 * @param[in]  eType               介质类型
 * @return	   成功返回OK  失败返回错误码
 */
INT32 medium_manager_format(IMediumManager *pIMediumManager, INT32 iMediumChan, MEDIUM_FORMAT_TYPE_E eType)
{
    INT32 iRet = ERROR;
    MEDIUM_MANAGER_PRIV_DATA_T *pStPrivData = NULL;
    MEDIUM_INFO_NODE_T *pStNode = NULL;
	MEDIUM_PART_INFO_T *pStPartNode = NULL;
    pStPrivData = medium_manager_get_priv_data(pIMediumManager);
    if(!pStPrivData)
    {
        LOGGER_ERROR("medium_manager_get_priv_data failed\n");
        return iRet;
    }

    iRet = ERROR;
    pStNode = medium_manager_find_medium_node(pStPrivData, iMediumChan);
    if(!pStNode)
    {
        LOGGER_ERROR("medium_manager_get_info_node failed, iMediumChan:%d  \n", iMediumChan);
        return iRet;
    }

    /* 存储卡容量不支持或正在格式化时不尝试格式化 */
    if(pStNode->eStates == MEDIUM_STATE_CAPACITY_NO_SUPPORT || pStNode->eStates == MEDIUM_STATE_FORMATTING)
    {
        LOGGER_ERROR("no capacity support :%d %d  \n", iMediumChan,pStNode->eStates);
        return iRet;
    }

   pStPartNode = medium_manager_find_medium_part(pStPrivData, pStNode->iMediumNo, pStNode->iPartNo);
	if( NULL == pStPartNode )
	{
		LOGGER_ERROR("find medium part failed\n");
        return ERROR;
	}
	
    /* 同步格式化 */
    switch(pStNode->eType)
    {
        case MEDIUM_TYPE_TF:
            iRet = medium_manager_format_handle_tf_card(pStPrivData, pStPartNode, eType);
            break;
        default:
            LOGGER_ERROR("unsupport medium type %u\n", pStNode->eType);
            break;
    }
    
    if(iRet < 0)
    {
        LOGGER_ERROR("medium_manager_format_handle failed, iMediumChan:%d\n", iMediumChan);
        return iRet;
    }

    LOGGER_INFO("medium_manager_format_handle completed, iMediumChan:%d\n", iMediumChan);
    return iRet;
}

/**@fn	       medium_manager_init_priv_data	  
 * @brief	   初始化私有数据
 * @param[in]  pStPrivData     IID号 统一分配
 * @return	   成功返回OK  失败返回错误码
 */
INT32 medium_manager_init_priv_data(MEDIUM_MANAGER_PRIV_DATA_T *pStPrivData)
{
    INT32 iRet = ERROR;

    if(!pStPrivData)
    {
        LOGGER_ERROR("input prt null\n");
        return iRet;
    }
    list_init(&pStPrivData->MediumInfoList);
	list_init(&pStPrivData->MediumPartInfoList);
    list_init(&pStPrivData->MediumStateCallBackList);
	list_init(&pStPrivData->MediumActionCallBackList);
    sys_mutex_create(&pStPrivData->MediumInfoMutex, MUTEX_NORMAL);
    sys_mutex_create(&pStPrivData->MediumformatMutex, MUTEX_NORMAL);
  //  pStPrivData->eSupportFileType = SYS_MEDIUM_FS_TYPE_UNKNOWN;

    iRet = sys_mqueue_create(&pStPrivData->stMediumMsgID, "MediumActionMsg", 64, sizeof(MEDIUM_MANAGER_MSG_T));
    if(iRet < 0)
    {
        LOGGER_ERROR("sys_mqueue_create :MediumMsg error.iRet = %d",iRet);
        return iRet;
    }

    iRet = sys_pthread_create(NULL, "medium_medium_monitor", TASK_PRIORITY_6, SIZE_64KB, 
        (FUNCPTR)medium_manager_action_monitor_thread, 1, pStPrivData);
    if(iRet < 0)
    {
        LOGGER_ERROR("sys_pthread_create failed\n");
        return iRet;
    }
        
    return iRet;
}



/**@fn	       medium_manager_init_interface	  
 * @brief	   初始化构造对外接口
 * @return	   成功返回OK  失败返回错误码
 */
INT32 medium_manager_init_interface(IMediumManager *pIMediumManager)
{
    INT32 iRet = ERROR;
    
    if(!pIMediumManager)
    {
        LOGGER_ERROR("input prt null\n");
        return iRet;
    }
    
    pIMediumManager->Init   = medium_manager_init;
    pIMediumManager->Start  = medium_manager_start;
    pIMediumManager->Format = medium_manager_format;

    return OK;
}

/**@fn	       medium_manager_init_instance	  
 * @brief	   初始化构造单例
 * @return	   成功返回OK  失败返回错误码
 */
IMediumManager *medium_manager_init_instance(VOID)
{   
    INT32 iRet = ERROR;
    MEDIUM_MANAGER_BASE_T *pBase = NULL;
    INotification * notifybroker = NULL;
    
    pBase = (MEDIUM_MANAGER_BASE_T *)sys_mem_malloc(sizeof(MEDIUM_MANAGER_BASE_T));
    if(!pBase)
    {
        LOGGER_ERROR("sys_mem_malloc failed\n");
        return NULL;
    }
    memset(pBase, 0, sizeof(MEDIUM_MANAGER_BASE_T));

    iRet = medium_manager_init_interface(&pBase->stInterface);
    if(iRet < 0)
    {   
        LOGGER_ERROR("init interface error \n");
        sys_mem_free(pBase);
        pBase = NULL;
        return NULL;
    }

    iRet = medium_manager_init_priv_data(&pBase->stPrivData);
    if(iRet < 0)
    {   
        LOGGER_ERROR("init priv data error \n");
        sys_mem_free(pBase);
        pBase = NULL;
        return NULL;
    }
    notifybroker = notification_get_notifybroker();
    if(notifybroker == NULL)
    {
        LOGGER_WARN("no register notificaition\n");
    }else{
        //notifybroker->Subscribe(notifybroker,MEDIUMPUBID,);
    }
    return &pBase->stInterface;
}