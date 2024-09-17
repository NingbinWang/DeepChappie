/**@file    sys_net.h
 * @brief   套接字的接口定义
 *
 */
#ifndef _SYS_NET_H_
#define _SYS_NET_H_
#include "sys_common.h"

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

/* @fn	      sys_net_set_ip	  
 * @brief	  配置IP
 * @param[in] strDevName 设备网卡名
 * @param[in] iDomain    协议族
 * @param[in] strIP      IP地址
 * @return 成功返回0;错误，返回 -1
 */
INT sys_net_set_ip(const CHAR *strDevName, INT iDomain, const CHAR *strIP);

/**@fn         sys_net_get_ip
 * @brief      获取设备IP地址
 * @param[in]  strEthName  网卡名
 * @param[in]  iDomain     协议族
 * @param[in]  strIpAddr   缓冲区地址指针
 * @param[in]  uLength     缓冲区大小长度
 * @return 成功返回0;错误，返回 -1
 */
INT sys_net_get_ip(const CHAR* strEthName, INT iDomain, CHAR *strIpAddr, UINT16 uLength);

/**@fn         sys_net_set_mac_addr
 * @brief      设置设备mac地址
 * @param[in]  strEthName  网卡名
 * @param[in]  pStMacAddr  缓冲区地址指针
 * @param[in]  uLength     缓冲区大小长度
 * @return 成功返回0;错误，返回 -1
 */
INT sys_net_set_mac_addr(const CHAR* strEthName,UINT8 *pStMacAddr,UINT16 uLength);

/**@fn         sys_net_get_mac_addr
 * @brief      获取设备mac地址
 * @param[in]  strEthName  网卡名
 * @param[in]  pStMacAddr  缓冲区地址指针
 * @param[in]  uLength     缓冲区大小长度
 * @return 成功返回0;错误，返回 -1
 */
INT sys_net_get_mac_addr(const CHAR* strEthName,UINT8 *pStMacAddr,UINT16 uLength);

/**@fn         sys_net_set_netmask
 * @brief      设置掩码
 * @param[in]  strEthName      网卡名
 * @param[in]  pStAddr         缓冲区地址指针
 * @param[in]  uLength         缓冲区大小长度
 * @return 成功返回0;错误，返回 -1
 */
INT sys_net_set_netmask(const CHAR* strEthName,UINT8 *pStAddr,UINT16 uLength);

/**@fn         sys_net_get_netmask
 * @brief      获取掩码
 * @param[in]  strEthName      网卡名
 * @param[in]  pStAddr         缓冲区地址指针
 * @param[in]  uLength         缓冲区大小长度
 * @return 成功返回0;错误，返回 -1
 */
INT sys_net_get_netmask(const CHAR* strEthName,UINT8 *pStAddr,UINT16 uLength);

/**@fn         sys_net_set_gateway
 * @brief      设置网关
 * @param[in]  strEthName      网卡名
 * @param[in]  pStAddr         缓冲区地址指针
 * @param[in]  uLength         缓冲区大小长度
 * @return 成功返回0;错误，返回 -1
 */
INT sys_net_set_gateway(const CHAR* strEthName,UINT8 *pStAddr,UINT16 uLength);

/**@fn         sys_net_get_gateway
 * @brief      获取网关
 * @param[in]  strEthName      网卡名
 * @param[in]  pStAddr         缓冲区地址指针
 * @param[in]  uLength         缓冲区大小长度
 * @return 成功返回0;错误，返回 -1
 */
INT sys_net_get_gateway(const CHAR* strEthName,UINT8 *pStAddr,UINT16 uLength);

/**@fn         sys_net_get_ip_mac
 * @brief      获取每个IP的mac地址
 * @param[in]  strIpAddr       IP地址
 * @param[in]  strNetCartName  网卡名
 * @param[out] strMac          mac地址
 * @param[in]  uLength         缓冲区长度
 * @return 成功返回0;错误，返回 -1
 */
INT sys_net_get_ip_mac(CHAR *strIpAddr ,const CHAR* strNetCartName ,UINT8* strMac ,UINT8 uLength);

/* @brief	  检测网络设备接口结点是否存在
 * @param[in] sDevName 设备名称
 * @return 成功返回 OK
 * @return 错误返回 ERROR, 不支持通过get_last_errno获取错误码
 */
INT sys_net_dev_node_check(const CHAR *strDevName);

/**@fn        sys_net_ifconfig_up
 * @brief	  启动网卡
 * @param[in] sDevName 网卡名
 * @return 成功返回0;错误，返回 -1
 */
INT sys_net_ifconfig_up(const CHAR *strName);

/**@fn        sys_net_ifconfig_down
 * @brief	  关闭网卡
 * @param[in] sDevName 网卡名
 * @return 成功返回0;错误，返回 -1
 */
INT sys_net_ifconfig_down(const CHAR *strName);

/* @fn	      sys_net_ifconfig	  
 * @brief	  配置网卡IP、网关、掩码
 * @param[in] strDevName 设备网卡名
 * @param[in] strIP      IP地址
 * @param[in] strNetMask 掩码
 * @param[in]strGateWay  网关
 * @return 成功返回0;错误，返回 -1
 */
INT sys_net_ifconfig(const CHAR *strDevName, const CHAR *strIP,const CHAR *strNetMask, const CHAR *strGateWay);

#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif/*__SYS_NET_INTERFACE_H__*/
