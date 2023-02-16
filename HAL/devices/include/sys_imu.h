#ifndef _SYS_IMU_H_
#define _SYS_IMU_H_
#include "sys_common.h"

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/


/**@fn        sys_gsensor_open    
 * @brief     打开gesnor     
 * @return    成功返回 gsensor的文件描述符 错误返回 ERROR
 */
INT32 sys_gsensor_open();

/**@fn         sys_gsensor_close
 * @brief      关闭gsensor的节点
 * @param[in]  gsensor的文件描述符
 * @return     成功返回 OK 错误返回 ERROR
 */
INT32 sys_gsensor_close(const UINT32 fd);

/**@fn         sys_gsensor_getname
 * @brief      gsensor的名字
 * @param[in]  gsensor的文件描述符
 * @return      成功返回 字符串 错误返回 NULL
 */
CHAR* sys_gsensor_getname(const UINT32 fd);

/**@fn         sys_gsensor_setpolltime
 * @brief      gsensor的名字
 * @param[in]  gsensor的文件描述符
 * @param[in]  polling的时间
 * @return     成功返回 OK 错误返回 ERROR
 */
INT32 sys_gsensor_setpolltime(const UINT32 fd,const UINT32 polltime);


/**@fn        sys_gsensor_lowpower
 * @brief      gsensor的低功耗模式
 * @param[in]  gsensor的文件描述符
 * @return     成功返回 OK 错误返回 ERROR
 */
INT32 sys_gsensor_lowpower(const UINT32 fd);

/**@fn        sys_gsensor_clearint
 * @brief      gsensor的清中断
 * @param[in]  gsensor的文件描述符
 * @return     成功返回 OK 错误返回 ERROR
 */
INT32 sys_gsensor_clearint(const UINT32 fd);

/**@fn        sys_gsensor_setwakeup
 * @brief      gsensor的设置唤醒模式
 * @param[in]  gsensor的文件描述符
 * @return     成功返回 OK 错误返回 ERROR
 */
INT32 sys_gsensor_setwakeup(const UINT32 fd,const UINT32 w_threshold);



#ifdef __cplusplus
}
#endif/*__cplusplus*/


#endif
