/**@file sys_block.h
 * @brief  
 * 
 * @author   alex king
 * @date     2023-02-16
 * @version  1.0
 * 
 * @note ///Description here 
 * @note History:        
 * @note     <author>    <time>    <version >   <desc>
 * @note     alex king   2023-02-16  v1.0.0     create
 * @warning  
 */
#ifndef _SYS_BLOCK_H_
#define _SYS_BLOCK_H_


#include "sys_common.h"


#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

/**@fn        sys_block_size 
 * @brief     获取节点大小      
 * @param[in] dev block节点
 * @return    成功返回节点大小 失败返回0
 */
UINT64 sys_block_size(const CHAR *dev);

/**@fn      sys_block_read
 * @brief   进行读数据
 * @param   [char * ]dev:   块设备节点
 * @param   [in ]offset:    分区偏移位置，要求block对齐
 * @param   [out ]buf:       回读的数据缓存区
 * @param   [in ]len:       待回读长度
 * @return  成功返回OK 失败返回ERROR
 */
INT32 sys_block_read(const CHAR *dev,UINT32 offset,CHAR *buf,int len);


/**@fn      sys_block_write
 * @brief   进行读数据
 * @param   [char * ]dev:   块设备节点
 * @param   [in ]offset:    分区偏移位置，要求对齐
 * @param   [in ]buf:       待写入的数据
 * @param   [in ]len:       待写入的数据长度
 * @return  成功返回OK 失败返回ERROR
 */
INT32 sys_block_write(char *dev, unsigned int offset, char *buf, int len);

/**@fn      sys_block_format
 * @brief   进行格式化整个分区
 * @param   [in ]dev:块设备节点
 * @param   [out]N/A
 * @return  成功返回OK 失败返回ERROR
 */
INT32 sys_block_format(const char *dev);


/**@fn      sys_block_avail_space_size
 * @brief   判断BLOCK分区的可用空间大小
 * @param   [in ]dev:块设备节点
 * @param   [out]N/A
 * @return  数据长度
 */
UINT32 sys_block_avail_space_size(const char *dev);

/**@fn      sys_block_chk_bank_page
 * @brief   判断是否是bank，即是否全0xff
 * @param   [in ]buf:   检测的数据
 * @param   [out]len:   对应的数据长度
 * @return  TRUE:  是bank page
 * @return  FALSE:  不是是bank page
 */
static BOOL sys_block_chk_bank_page(UINT8 *buf, UINT32 len);

/**@fn      sys_block_valid_data_len
 * @brief   判断BLOCK分区的有效数据长度
 * @param   [in ]dev:块设备节点
 * @param   [out]N/A
 * @return  数据长度 失败返回ERROR
 */
UINT32 sys_block_valid_data_len(const CHAR *dev);

/**@fn      sys_block_copy
 * @brief   进行分区拷贝
 * @param   [in ]src_dev:    源块设备节点
 * @param   [in ]dst_dev:    目的设备节点
 * @return  成功返回OK 失败返回ERROR
 */
INT32 sys_block_copy(const char *src_dev, const char *dst_dev);

/**@fn      sys_block_cmp
 * @brief   进行分区数据比较，支持跳坏块
 * @param   [in ]src_dev:    源块设备节点
 * @param   [in ]dst_dev:    目的设备节点
 * @return  =0: 数据一致
 * @return  !0: 数据不一致
 */
INT32 sys_block_cmp(const char *src_dev, const char *dst_dev);

/**@fn        sys_block_verboseset 
 * @brief     设置是否打印百分比
 * @param[in] enable
 */
VOID sys_block_verboseset(const INT32 enable);



#ifdef __cplusplus
}
#endif/*__cplusplus*/


#endif
