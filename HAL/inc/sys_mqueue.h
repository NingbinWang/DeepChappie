/**@file    sys_mqueue.h
 * @brief       posix消息队列接口中间层宏定义接口
 * @method  1.创建队列
 *          2.发送队列
 *          3.接收队列消息
 *          4.关闭队列
 *          5.刷新队列
 *          6.查询消息队列
 */
#ifndef _SYS_MQUEUE_H_
#define _SYS_MQUEUE_H_
#include "sys_common.h"

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

/* MSG_Q_ID 仅仅一个指针值，具体指向什么各平台内部处理*/
typedef LONG  MSG_Q_ID;

#define NO_WAIT             0

#define WAIT_FOREVER        ((UINT)(-1))

/**@fn         sys_mqueue_create
 * @brief      创建一个消息队列
 * @param[in]  pStHandle 消息队列对象，入参，需非空
 * @param[in]  strName   消息队列名字
 * @param[in]  iMaxMsg   一个消息队列最大消息个数
 * @param[in]  iMsgLen   消息长度
 * @return     成功返回 0, 并且handle被修改;其他失败，参考ERROR_CODE_E
 */
INT sys_mqueue_create(MSG_Q_ID *pStHandle,const CHAR *strName, INT iMaxMsg, INT iMsgLen);

/**@fn         sys_mqueue_create
 * @brief      发送消息
 * @param[in]  pStHandle  消息队列句柄
 * @param[in]  strMsg     发送的消息
 * @param[in]  uLength    消息长度
 * @param[in]  uWaitTime  阻塞时长
 * @return     成功返回 0  错误返回 其他, 参考ERROR_CODE_E
 */
INT sys_mqueue_send(MSG_Q_ID *pStHandle, const CHAR *strMsg, UINT uLength, INT iWaitMs);

/**@fn         sys_mqueue_create
 * @brief      接受消息
 * @param[in]  pStHandle  消息队列句柄
 * @param[in]  strBuff    接受消息缓存
 * @param[in]  uBuffLen   缓存大小
 * @param[in]  uWaitTime  阻塞时长
 * @return     成功返回 0  错误返回 其他, 参考ERROR_CODE_E
 */
INT sys_mqueue_recv(MSG_Q_ID *pStHandle, CHAR *strBuff, INT iBuffLen, INT iWaitMs);

/**@fn         sys_mqueue_close
 * @brief      关闭消息队列
 * @param[in]  pStHandle 消息队列句柄
 * @return     成功返回 0 错误返回 其他, 参考ERROR_CODE_E
 */
INT sys_mqueue_close(MSG_Q_ID *pStHandle);

/**@fn         sys_mqueue_flush
 * @brief      flush消息队列
 * @param[in]  pStHandle 消息队列句柄
 * @return     成功返回 0 错误返回 其他, 参考ERROR_CODE_E
 */
INT sys_mqueue_flush(MSG_Q_ID *pStHandle);

/**@fn         sys_mqueue_query
 * @brief      查询消息队列
 * @param[in]  pStHandle       消息队列句柄
 * @param[out] uRemainCount    剩余消息个数
 * @param[out] uRemainBuff     剩余缓存大小
 * @return     成功返回 0 错误返回 其他, 参考ERROR_CODE_E
 */
INT sys_mqueue_query(MSG_Q_ID *pStHandle, UINT *puRemainCount, UINT *puRemainBuff);

#ifdef __cplusplus
}
#endif/*__cplusplus*/
#endif/*_SYS_MQUEUE_H_*/