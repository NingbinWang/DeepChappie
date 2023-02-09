#include "sys_file_priv.h"
/**@fn        sys_file_fopen    
 * @brief     打开文件      
 * @param[in] strPath 文件路径
 * @param[in] mode 打开文件的方式, 参考man 3 fopen,如wr+
 * @return    成功返回 文件句柄 错误返回 NULL, 支持通过get_last_errno获取错误码
 */
FILE_ID* sys_file_fopen(const CHAR *strPath, const CHAR *strMode)
{
    FILE *pHandle = NULL ;
    
    if(NULL == strPath || NULL == strMode)
    {
        SYS_COMMON_ERROR("file invaild param \n");
        return NULL;
    }

    pHandle = fopen(strPath, strMode);
    if (NULL == pHandle)
    {
        /* SYS_COMMON_ERROR("fopen file error :%s strMode:%s errno:%s \n",strPath,strMode,strerror(errno)); */
        return NULL;
    }
	return (FILE_ID *)pHandle;
}

/**@fn         sys_file_fclose
 * @brief      关闭文件      
 * @param[in]  pFileID     文件句柄，通过sys_file_fopen获得
 * @return     成功返回0  错误返回 其他, 参考ERROR_CODE_E, 支持通过get_last_errno获取错误码
 */
INT32 sys_file_fclose(FILE_ID *pFileID)
{
    INT32 iRet = ERROR;
    if (NULL == pFileID)
    {
        SYS_COMMON_ERROR("file invaild param \n");
        return ERROR;
    }
    iRet = fclose((FILE *)pFileID);            /*关闭打开的文件*/
    if (OK != iRet)
    {
        SYS_COMMON_ERROR("fclose file error  \n");
        return ERROR;
    }
	return iRet;
}

/**@fn         sys_file_fread
 * @brief      读文件数据      
 * @param[in]  pFileID 文件句柄，通过sys_file_fopen获得
 * @param[in]  pBuffer 目的地址
 * @param[in]  uSize  每块字节数
 * @param[in]  uCount  要读取的块数
 * @return     成功返回 实际读取的块数 错误返回 ERROR, 支持通过get_last_errno获取错误码
 * @note       到达文件末尾的时候，返回值可能小于uCount
 */
INT32 sys_file_fread(FILE_ID *pFileID, VOID *pBuffer, UINT32 uSize, UINT32 uCount)
{
    UINT32  uReadCount= 0;
    if(NULL == pFileID || NULL == pBuffer || 0 == (uSize * uCount))
    {
        SYS_COMMON_ERROR("file invaild param :%p :%p :%d \n",pFileID,pBuffer,(uSize * uCount));
        return ERROR;
    }

    uReadCount = fread(pBuffer, uSize, uCount, (FILE *)pFileID);
    if (uReadCount == uCount)
    {
        return uReadCount;
    }
    if (sys_file_feof(pFileID) > 0)
    {
        return uReadCount;
    }
    SYS_COMMON_ERROR("sys_feof error \n");
    return ERROR;
}

/**@fn         sys_file_fwrite
 * @brief      写文件     
 * @param[in]  pFileID 文件句柄 
 * @param[in]  pBuffer 要写入文件的内容存放的地址
 * @param[in]  uSize   每块数据的大小
 * @param[in]  uCount  要写入的块数
 * @return     成功返回 实际写入的块数 错误返回 ERROR ,  支持通过get_last_errno获取错误码
 */
INT32 sys_file_fwrite(FILE_ID *pFileID, const VOID *pBuffer, UINT32 uSize, UINT32 uCount)
{
     if(NULL == pFileID || NULL == pBuffer || 0 == (uSize * uCount))
    {
        SYS_COMMON_ERROR("file invaild param :%p  :%p :%d \n",pFileID ,pBuffer,(uSize * uCount));
        return ERROR;
    }

    if (fwrite(pBuffer, uSize, uCount, (FILE *)pFileID) != uCount)
    {
        SYS_COMMON_ERROR("fwrite error \n");
        return ERROR;
    }
    return uCount;    
}

/**@fn         sys_file_fsync
 * @brief      强制写文件到存储介质      
 * @param[in]  pFileID  文件句柄
 * @return     成功返回 0 错误返回 其他, 参考ERROR_CODE_E, 支持通过get_last_errno获取错误码
 */
INT32 sys_file_fsync(FILE_ID *pFileID)
{
    INT32 iRet = ERROR;
    INT32 iFd = -1 ;
    if (NULL == pFileID)
    {
        return ERROR;
    }
    iRet = fflush((FILE*)pFileID);
    if (OK != iRet)
    {
        SYS_COMMON_ERROR(" fflush error :%s iRet:%d\n",strerror(errno), iRet);
    }

    iFd = fileno((FILE*)pFileID);
    iRet = fsync(iFd);
    if (OK != iRet)
    {
        SYS_COMMON_ERROR(" fsync error :%s iRet:%d\n",strerror(errno), iRet);
    }
	return iRet;
}

/**@fn         sys_file_fseek
 * @brief      重新定位文件读写位置
 * @param[in]  pFileID  文件句柄
 * @param[in]  lOffset为偏移量，正数表示正向偏移，负数表示负向偏移
 * @param[in]  iStartPos 设定从文件的哪里开始偏移,可能取值为：SEEK_CUR、 SEEK_END 或 SEEK_SET
 * @return     成功返回 0 错误返回 其他, 参考ERROR_CODE_E, 支持通过get_last_errno获取错误码
 */
INT32 sys_file_fseek(FILE_ID *pFileID, LONG lOffset, INT32 iStartPos)
{
    INT32 iRet = ERROR;
    if (NULL == pFileID)
    {
        return ERROR;
    }
    iRet = fseek((FILE *)pFileID, lOffset, iStartPos);
    if (OK != iRet)
    {
        SYS_COMMON_ERROR("fseek error \n");
    }
    return iRet;
}

/**@fn         sys_file_ftell    
 * @brief      文件指针所指位置相对于开头的偏移，单位字节byte          
 * @param[in]  pFileID  文件句柄
 * @return     成功返回 偏移值 错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
LONG sys_file_ftell(FILE_ID *pFileID)
{
    INT32 iRet = ERROR;

    if (NULL == pFileID)
    {
       SYS_COMMON_ERROR("file invaild param \n");
        return ERROR;
    }

    iRet = (LONG)ftell((FILE *)pFileID);
    if (ERROR == iRet)
    {
        SYS_COMMON_ERROR("ftell error \n");
    }
	return iRet;
}

/**@fn         sys_file_fstat    
 * @brief      查看文件信息
 * @param[in]  strPath 路径
 * @param[in]  pStSate 文件信息
 * @return     成功返回 0 错误返回 其他, 参考ERROR_CODE_E, 支持通过get_last_errno获取错误码
 */
INT32 sys_file_fstat(const CHAR *strPath, SYS_FS_STAT_T *pStSate)
{
    INT32 iRet = ERROR;
    struct stat stState = {0};
    if(NULL == strPath || NULL == pStSate)
    {
        return ERROR;
    }
    iRet = stat(strPath, &stState);
    if(iRet != OK)
    {
        fprintf(stderr,"[%s]strPath:%s error:%s iRet:%d \n",strPath,__FUNCTION__,strerror(errno),iRet);
        return iRet;
    }
    if (S_ISDIR(stState.st_mode))
    {
        pStSate->uAttr |= SYS_FS_ATTR_DIR;
    }

    pStSate->uSpaceSize = stState.st_blocks * 512;  /* block大小是512 字节，参考man 2 stat */
    pStSate->uSize = stState.st_size;
    pStSate->uClusterSize = stState.st_blksize;
    return iRet;
}

/**@fn         sys_file_feof
 * @brief      判断是否已经到达文件末尾
 * @param[in]  pFileID  文件句柄
 * @return     > 0 ,已经到达文件末尾;= 0 , 未到达文件末尾;< 0 , 文件句柄错误
 */
INT32 sys_file_feof(FILE_ID *pFileID)
{
    if (NULL == pFileID)
    {
        return ERROR;
    }
    
	return feof((FILE *)pFileID);
}

/**@fn         sys_file_read_dir 暂时考虑到拷贝封装的性能消耗，互调反馈      
 * @brief      读目录文件夹
 * @param[in]  strDir          查看目录
 * @param[in]  pUserParam      自定义参数
 * @param[in]  pCallBack       读数据回调接口
 * @return     成功 0  失败 -1 
 */
INT32 sys_file_read_dir(const CHAR *strDir, VOID *pUserParam, ReadDirCallBack pCallBack)
{
    INT32 iRet = ERROR;
    DIR *pDir = NULL ;
    struct dirent *pDirRent = NULL;
    CHAR strDirPath[512] = {0};
    if(!strDir || !pUserParam || !pCallBack)
    {
        SYS_COMMON_ERROR("invaild param :%p %p %p\n",strDir,pUserParam,pCallBack);
        return iRet;
    }
    pDir = opendir(strDir);
    if(pDir == NULL)
    {
        SYS_COMMON_ERROR("open dir: %s failed.\n",strDir);
        return iRet;
    }
    while((pDirRent = readdir(pDir)) != NULL)
    {
        //exfata 读不出文件
        if(strcmp(pDirRent->d_name,".") == 0)
        {
            continue;
        }
        if(strcmp(pDirRent->d_name,"..") == 0)
        {
            continue;
        }
        memset(strDirPath,0,sizeof(strDirPath));
        snprintf(strDirPath,sizeof(strDirPath),"%s/%s",strDir,pDirRent->d_name);
        pCallBack(strDirPath,pUserParam);
    }
    closedir(pDir);
    return OK;
}
/**@fn         sys_file_read_dir_file      
 * @brief      读目录文件
 * @param[in]  strDir          查看目录
 * @param[in]  pUserParam      自定义参数
 * @param[in]  pCallBack       读数据回调接口
 * @return     成功 0  失败 -1 
 */
INT32 sys_file_read_dir_file(const CHAR *strDir, VOID *pUserParam, ReadFileCallBack pCallBack)
{
    INT32 iRet = ERROR;
    DIR *pDir = NULL ;
    struct dirent *pDirRent = NULL;
    CHAR strFilePath[512] = {0};
    SYS_FS_STAT_T stStatBuf = {0};
    if(!strDir || !pUserParam || !pCallBack)
    {
        return iRet;
    }

    pDir = opendir(strDir);
    if(pDir == NULL)
    {
        SYS_COMMON_ERROR("open dir: %s failed.\n",strDir);
        return iRet;
    }
    while((pDirRent = readdir(pDir)) != NULL)
    {
		if(strcmp(pDirRent->d_name,".") == 0)
        {
            continue;
        }
        if(strcmp(pDirRent->d_name,"..") == 0)
        {
            continue;
        }
        snprintf(strFilePath,sizeof(strFilePath),"%s/%s",strDir,pDirRent->d_name);
        iRet = sys_file_fstat(strFilePath, &stStatBuf);
        if(iRet < 0)
        {
            SYS_COMMON_ERROR("lstat %s error.\n", strFilePath);
            break;
        }
        pCallBack(strFilePath,&stStatBuf,pUserParam);
    }

    closedir(pDir);
    return OK;
}


/**@brief	  获取文件当前定位
 * @param[in] pFileID  文件句柄
 * @param[in] plOffset为偏移量指针
 * @return 成功返回 0
 * @return 错误返回 其他, 参考ERROR_CODE_E, 支持通过get_last_errno获取错误码
 */
INT32 sys_file_fgetpos(FILE_ID *pFileID, LONG *plOffset)
{
    INT32 iRet = ERROR;
#ifndef SYS_ANDRIOD
    if (NULL == pFileID || NULL == plOffset)
    {
        return ERROR;
    }
    
    iRet = fgetpos((FILE *)pFileID, (fpos_t *)plOffset);
    if (OK != iRet)
    {
        SYS_COMMON_ERROR("fgetpos error \n");
    }
#endif
    return iRet;
}