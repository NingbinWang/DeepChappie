#ifndef _AVTP_ADAPTOR_H_
#define _AVTP_ADAPTOR_H_

#include <arpa/inet.h>
#include <linux/if.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/timerfd.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "sys_common.h"
#include "sys_utils.h"
//#include "sys_common_inside.h"

#define _AVTP_LOG(format, ...)      printf("[AVB]"format"\r\n", ##__VA_ARGS__)
#define AVTP_INFO(format, ...)  _AVTP_LOG("[Info] "format, ##__VA_ARGS__)
#define AVTP_WARN(format, ...)  _AVTP_LOG("[Warn] "format, ##__VA_ARGS__)
#define AVTP_ERROR(format, ...) _AVTP_LOG("[Error] "format, ##__VA_ARGS__)

#ifndef ETH_P_TSN
#define ETH_P_TSN	0x22F0		/* TSN (IEEE 1722) packet	*/
#endif


/**@fn         avtp_talker_socket_create   
 * @brief      avtp talker创建套接字
 * @param[in]  iPriority 优先级
 * @param[in]  pInterfaceName 网卡名称
 * @param[in]  pDestMacAddr 目的Mac地址
 * @param[in]  struct sockaddr_ll 结构体指针
 * @return     成功返回OK 失败返回ERROR
 */
INT32 avtp_talker_socket_create(INT32 iPriority, CHAR *pInterfaceName, UINT8 *pDestMacAddr, struct sockaddr_ll *pStSockAddr);

/**@fn         avtp_socket_send   
 * @brief      发送数据
 * @param[in]  iSock 套接字
 * @param[in]  pData 发送的数据
 * @param[in]  uDataLen 数据长度
 * @param[in]  struct sockaddr_ll 结构体指针
 * @return     成功返回发送的数据长度 失败返回ERROR
 */
INT32 avtp_socket_send(INT32 iSock, UINT8 *pData, UINT32 uDataLen, struct sockaddr_ll *pStSockAddr);

/**@fn         avtp_socket_close   
 * @brief      关闭套接字
 * @param[in]  iSock 套接字
 * @param[in]  pBuf 接收缓冲区地址指针
 * @param[in]  iBufLen 接收缓冲区长度
 * @return     成功返回OK 失败返回ERROR
 */
INT32 avtp_socket_close(INT32 iSock);



#endif