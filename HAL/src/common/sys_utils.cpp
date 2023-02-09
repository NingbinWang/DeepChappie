/*
 * @Author: king  alex
 * @Date: 2023-02-10

 */
#include"sys_utils.h"
#include <arpa/inet.h>

#define LITTLE_ENDIAN_ORDER   1
#define BIG_ENDIAN_ORDER      2
/**@fn	       sys_check_sum	  
 * @brief	   对一块内存空间进行字节和校验
 * @param[in]  pBuf             内存空间指针
 * @param[in]  iLen             长度
 * @return	   成功返回校验和  
 */
UINT32 sys_check_sum(VOID *pBuf, INT32 iLen)
{
    UINT32 uSum = 0;
    INT32 iIndex = 0;
    char *pTmp = (char *)pBuf;
    for (iIndex = 0; iIndex < iLen; iIndex++)
    {
        uSum += (int)pTmp[iIndex] & 0xff;
    }
    return uSum;
}



/**@fn         sys_htonl
 * @brief      将本地序转换成网络序
 * @param[in]  x  待转换数值
 * @return     转换后的数值
 * @note 如果平台是大端序，内部不做任何转换
 */
UINT32 sys_htonl(UINT32 x)
{
    return htonl(x);
}

/**@fn         sys_ntohl
 * @brief      将网络序转换成本地序
 * @param[in]  x  待转换数值
 * @return     转换后的数值
 * @note       如果平台是大端序，内部不做任何转换
 */
UINT32 sys_ntohl(UINT32 x)
{
  return ntohl(x);
}

/**@fn         sys_htons
 * @brief      将本地序转换成网络序
 * @param[in]  x  待转换数值
 * @return     转换后的数值
 * @note       如果平台是大端序，内部不做任何转换
 */
UINT16 sys_htons(UINT16 x)
{
    return htons(x);
}

/**@fn        sys_ntohs
 * @brief     将网络序转换成本地序
 * @param[in] x  待转换数值
 * @return    转换后的数值
 * @note      如果平台是大端序，内部不做任何转换
 */
UINT16 sys_ntohs(UINT16 x)
{
    return ntohs(x);
}

/**@fn        sys_ntohll
 * @brief     将网络序转换成本地序
 * @param[in] x  待转换数值
 * @return    转换后的数值
 * @note      如果平台是大端序，内部不做任何转换
 */
UINT64 sys_ntohll(UINT64 x)
{
    return (((UINT64)sys_ntohl((UINT32)((x << 32) >> 32) )) << 32) | (UINT32)sys_ntohl ((UINT32)(x >> 32));
}

/**@fn        sys_htonll
 * @brief     将本地序转换成网络序
 * @param[in] x  待转换数值
 * @return    转换后的数值
 * @note      如果平台是大端序，内部不做任何转换
 */
UINT64 sys_htonll(UINT64 x)
{
    return (((UINT64)sys_htonl((UINT32)((x << 32) >> 32) )) << 32) | (UINT32)sys_htonl ((UINT32)(x >> 32));
}

/**@fn        sys_srand
 * @brief     设置产生一系列伪随机数发生器的起始点，要想把发生器重新初始化
 * @param[in] uSeed  随机数产生器的初始值
 * @return    无
 */
VOID sys_srand(UINT32 uSeed)
{
    srand(uSeed);
}
/**@fn        sys_rand
 * @brief     获取随机数
 * @param[in] 无
 * @return    返回随机数值
 */
INT32 sys_rand(VOID)
{
   return rand();
}