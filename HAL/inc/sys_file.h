#ifndef  _SYS_FILE_H_
#define  _SYS_FILE_H_

/**@file    sys_file.h
 * @brief   文件操作接口的封装层，用标准C file接口封装屏蔽平台依赖
 * @author  alex king
 * @method  1.打开文件
 *          2.关闭文件
 *          3.读文件数据
 *          4.写文件
 *          5.强制写文件到存储介质
 *          6.重新定位文件读写位置
 *          7.文件指针所指位置相对于开头的偏移，单位字节byte
 *          8.查看文件信息
 *          9.判断是否已经到达文件末尾
 *          10.读目录文件
 *          11.读目录文件夹
 * @version V1.0.0  基于C的接口进行实现,屏蔽差异,部分系统文件操作接口,按平台差异实现
 */
#include "sys_common.h"

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

#ifndef FILE_WINDOWS
#define FILE_SEARCH_SEPARATOR ('/')
#else
#define FILE_SEARCH_SEPARATOR ('\\')
#endif

/**@struct SYS_FS_ATTR_E 
 * @brief  定义文件属性枚举
 */
typedef enum {
    /* Standard file attribute */
    SYS_FS_ATTR_RDONLY         =   0x01,
    SYS_FS_ATTR_HIDDEN         =   0x02,
    SYS_FS_ATTR_SYSTEM         =   0x04,
    SYS_FS_ATTR_VOLUME         =   0x08,
    SYS_FS_ATTR_DIR            =   0x10,
    SYS_FS_ATTR_ARCH           =   0x20,

    /* Other file attribute */
    SYS_FS_ATTR_NONE           =   0x40,
    SYS_FS_ATTR_ALL            =   0x7f,
    SYS_FS_ATTR_FILE_ONLY      =   0x100,
} SYS_FS_ATTR_E;

/**@struct SYS_FS_STAT_T 
 * @brief  定义文件属性结构
 */
typedef struct
{
    UINT64 uSize;               /*文件实际大小,单位Byte*/
    UINT32 uAttr;               /*位图，参考SYS_FS_ATTR_T*/
    UINT32 uClusterSize;        /*文件簇大小,单位Byte*/
    UINT64 uSpaceSize;          /*文件实际占用空间大小,单位 Byte,大小为文件大小的整数倍*/
    UINT32 uRes[4];             /*预留字节*/
}SYS_FS_STAT_T;

typedef LONG  FILE_ID;
typedef LONG  DIR_ID;

/*文件位置偏移current*/
#ifndef SEEK_CUR
#define SEEK_CUR             (1)             
#endif

/*文件位置偏移到头部*/
#ifndef SEEK_SET
#define SEEK_SET             (0)
#endif

/*文件位置偏移到尾部*/
#ifndef SEEK_END
#define SEEK_END             (2)
#endif

/**@fn         ReadDirCallBack      
 * @brief      读目录文件
 * @param[in]  strDir          查看目录
 * @param[in]  pUserParam      自定义参数
 * @return     成功 0  失败 -1 
 */
typedef INT32 (*ReadDirCallBack)(const CHAR *strDirPath, VOID *pUserParam);

/**@fn         ReadFileCallBack      
 * @brief      遍历目录文件信息
 * @param[in]  strFilePath     文件全路径
 * @param[in]  pStFileState    文件状态信息指针
 * @param[in]  pUserParam      自定义数据
 * @return       成功 0  失败 -1 
 */
typedef INT32 (*ReadFileCallBack)(const CHAR *strFilePath, SYS_FS_STAT_T *pStFileState, VOID *pUserParam);

/**@fn        sys_file_fopen    
 * @brief     打开文件      
 * @param[in] strPath 文件路径
 * @param[in] mode 打开文件的方式, 参考man 3 fopen,如wr+
 * @return    成功返回 文件句柄 错误返回 NULL, 支持通过get_last_errno获取错误码
 */
FILE_ID* sys_file_fopen(const CHAR *strPath, const CHAR *strMode);

/**@fn         sys_file_fclose
 * @brief      关闭文件      
 * @param[in]  pFileID     文件句柄，通过sys_file_fopen获得
 * @return     成功返回0  错误返回 其他, 参考ERROR_CODE_E, 支持通过get_last_errno获取错误码
 */
INT32 sys_file_fclose(FILE_ID *pFileID);

/**@fn         sys_file_fread
 * @brief      读文件数据      
 * @param[in]  pFileID 文件句柄，通过sys_file_fopen获得
 * @param[in]  pBuffer 目的地址
 * @param[in]  uSize  每块字节数
 * @param[in]  uCount  要读取的块数
 * @return     成功返回 实际读取的块数 错误返回 ERROR, 支持通过get_last_errno获取错误码
 * @note       到达文件末尾的时候，返回值可能小于uCount
 */
INT32 sys_file_fread(FILE_ID *pFileID, VOID *pBuffer, UINT32 uSize, UINT32 uCount);

/**@fn         sys_file_fwrite
 * @brief      写文件     
 * @param[in]  pFileID 文件句柄 
 * @param[in]  pBuffer 要写入文件的内容存放的地址
 * @param[in]  uSize   每块数据的大小
 * @param[in]  uCount  要写入的块数
 * @return     成功返回 实际写入的块数 错误返回 ERROR ,  支持通过get_last_errno获取错误码
 */
INT32 sys_file_fwrite(FILE_ID *pFileID, const VOID *pBuffer, UINT32 uSize, UINT32 uCount);

/**@fn         sys_file_fsync
 * @brief      强制写文件到存储介质      
 * @param[in]  pFileID  文件句柄
 * @return     成功返回 0 错误返回 其他, 参考ERROR_CODE_E, 支持通过get_last_errno获取错误码
 */
INT32 sys_file_fsync(FILE_ID *pFileID);

/**@fn         sys_file_fseek
 * @brief      重新定位文件读写位置
 * @param[in]  pFileID  文件句柄
 * @param[in]  lOffset为偏移量，正数表示正向偏移，负数表示负向偏移
 * @param[in]  iStartPos 设定从文件的哪里开始偏移,可能取值为：SEEK_CUR、 SEEK_END 或 SEEK_SET
 * @return     成功返回 0 错误返回 其他, 参考ERROR_CODE_E, 支持通过get_last_errno获取错误码
 */
INT32 sys_file_fseek(FILE_ID *pFileID, LONG lOffset, INT32 iStartPos);

/**@fn         sys_file_ftell    
 * @brief      文件指针所指位置相对于开头的偏移，单位字节byte          
 * @param[in]  pFileID  文件句柄
 * @return     成功返回 偏移值 错误返回 ERROR, 支持通过get_last_errno获取错误码
 */
LONG sys_file_ftell(FILE_ID *pFileID);

/**@fn         sys_file_fstat    
 * @brief      查看文件信息
 * @param[in]  strPath 路径
 * @param[in]  pStSate 文件信息
 * @return     成功返回 0 错误返回 其他, 参考ERROR_CODE_E, 支持通过get_last_errno获取错误码
 */
INT32 sys_file_fstat(const CHAR *strPath, SYS_FS_STAT_T *pStSate);

/**@fn         sys_file_feof
 * @brief      判断是否已经到达文件末尾
 * @param[in]  pFileID  文件句柄
 * @return     > 0 ,已经到达文件末尾;= 0 , 未到达文件末尾;< 0 , 文件句柄错误
 */
INT32 sys_file_feof(FILE_ID *pFileID);

/**@fn         sys_file_fgetpos
 * @brief      获取文件当前定位
 * @param[in]  pFileID  文件句柄
 * @return     成功返回0,错误返回其他
 */
INT32 sys_file_fgetpos(FILE_ID *pFileID, LONG *plOffset);

/**@fn         sys_file_read_dir 暂时考虑到拷贝封装的性能消耗，互调反馈      
 * @brief      读目录文件夹
 * @param[in]  strDir          查看目录
 * @param[in]  pUserParam      自定义参数
 * @param[in]  pCallBack       读数据回调接口
 * @return     成功 0  失败 -1 
 */
INT32 sys_file_read_dir(const CHAR *strDir, VOID *pUserParam, ReadDirCallBack pCallBack);

/**@fn         sys_file_read_dir_file      
 * @brief      读目录文件
 * @param[in]  strDir          查看目录
 * @param[in]  pUserParam      自定义参数
 * @param[in]  pCallBack       读数据回调接口
 * @return     成功 0  失败 -1 
 */
INT32 sys_file_read_dir_file(const CHAR *strDir, VOID *pUserParam, ReadFileCallBack pCallBack);

#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif