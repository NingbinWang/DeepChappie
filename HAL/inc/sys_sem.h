/**@file    sys_sem.h
 * @brief       posix信号量接口中间层宏定义接口
 * @method  1.创建信号量
 *          2.释放一个信号量资源
 *          3.获取一个信号量资源
 *          4.销毁信号量
 */
#ifndef _SYS_SEM_H_
#define _SYS_SEM_H_

#include "sys_common.h"

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

/* semaphore options */
#define SEM_EMPTY               0
#define SEM_FULL                1

/* SEM_ID 仅仅一个指针值，具体指向什么各平台内部处理*/
typedef LONG  SEM_ID;

/**@fn         sys_sem_create
 * @brief      创建信号量
 * @param[in]  pStSem        信号量句柄指针
 * @param[in]  strName       信号量名称
 * @param[in]  uInitValue    信号量初始值
 * @return     成功返回 0    错误返回 其他, 参考ERROR_CODE_E
 */
INT sys_sem_create(SEM_ID *pStSem, const CHAR *strName, UINT uInitValue);

/**@fn         sys_sem_post
 * @brief      释放一个信号量资源
 * @param[in]  pStSem信号量句柄
 * @return     成功返回 0 错误返回 其他, 参考ERROR_CODE_E
 */
INT sys_sem_post(SEM_ID *pStSem);

/**@fn         sys_sem_wait
 * @brief      获取一个信号量资源
 * @param[in]  pStSem信号量句柄
 * @param[in]  uWaitTime 阻塞时长
 * @return     成功返回 0 错误返回 其他, 参考ERROR_CODE_E
 * @see
 * @note
 */
INT sys_sem_wait(SEM_ID *pStSem, UINT uWaitTime);

/**@fn         sys_sem_destroy
 * @brief      销毁信号量
 * @param[in]  pStSem信号量句柄
 * @return     成功返回 0 错误返回 其他, 参考ERROR_CODE_E
 * @see
 * @note
 */
INT sys_sem_destroy(SEM_ID *pStSem);

#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif/*_SYS_SEM_H_*/