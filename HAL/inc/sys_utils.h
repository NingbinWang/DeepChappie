/**@file    sys_utils.h
 * @brief   通用运算定义
 * @author  alex king
 * @date    2023-02-10
 * @version V1.0.0 
 *         
 */
#ifndef _SYS_UTILS_H_
#define _SYS_UTILS_H_
#include "sys_common.h"

#ifdef __cplusplus
extern "C" {
#endif


/*ROL:value循环左移bits位*/
#define SYS_ROL(value,bits) ((value << bits) | (value >> (sizeof(value)*8 -bits)))

/*ROR:value循环右移bits位*/
#define SYS_ROR(value,bits) ((value >> bits) | (value << (sizeof(value)*8 -bits)))

/*
 *@brief 判断指针是否为空，若为空，输出一段打印后break
 */
#ifndef SYS_CHECK_PTR_BREAK
#define SYS_CHECK_PTR_BREAK(p, PRINTF){ \
    if (NULL == (p)) \
    { \
        PRINTF("func %s, line %d Ptr is NULL\n", __FUNCTION__, __LINE__);\
        break;\
    } \
}
#endif

/*
 *@brief 判断返回值，若为ERROR，输出一段打印后break
 */
#ifndef SYS_CHECK_RET_BREAK
#define SYS_CHECK_RET_BREAK(iRet, PRINTF){ \
    if (ERROR == iRet) \
    { \
        PRINTF("func %s, line %d iRet is ERROR\n", __FUNCTION__, __LINE__);\
        break;\
    } \
}
#endif

/*
 *@brief 判断指针是否为空，若为空，输出一段打印后返回一个值
 */
#ifndef SYS_CHECK_PTR_RET
#define SYS_CHECK_PTR_RET(p, PRINTF, RET) do { \
    if (NULL == (p)) \
    { \
        PRINTF("func %s, line %d Ptr is NULL\n", __FUNCTION__, __LINE__);\
        return RET;\
    } \
}while (0)
#endif

/**
 * @brief 判断指针是否为空，若为空，输出一段打印后返回空
 */
#ifndef SYS_CHECK_PTR_VOID
#define SYS_CHECK_PTR_VOID(p, PRINTF) do { \
    if (NULL == (p)) \
    { \
        PRINTF("%s[%d] Ptr is NULL\n", __FUNCTION__, __LINE__);\
        return ;\
    } \
}while (0)
#endif

#define    BYTES_ALIGN(p, alignbyte)    (((ULONG)(p) + (ULONG)((alignbyte) - 1)) &~ ((alignbyte) - 1))


#ifndef ARRAY_SIZE
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))
#endif

#ifndef ROUND_UP
#define ROUND_UP(x, align)       (((int)(x) + (align - 1)) & ~(align - 1))
#endif

#ifndef ROUND_DOWN
#define ROUND_DOWN(x, align)    ((int)(x) & ~(align - 1))
#endif


/**
 * @brief 计算循环读缓冲区中的len1和len2的宏，降低代码量
 */
#ifndef SYS_BUF_LEN_UPDATE
#define SYS_BUF_LEN_UPDATE(uLen1, uLen2, uRIdx, uWIdx, uBufSize) \
do\
{\
    if(uWIdx >= uRIdx)\
    {\
        uLen1 = uWIdx - uRIdx;\
        uLen2 = 0;\
    }\
    else\
    {\
        uLen1 = uBufSize - uRIdx;\
        uLen2 = uWIdx;\
    }\
}while(0)
#endif


/**
 * @brief 计算循环写缓冲区中的len1和len2的宏，降低代码量
 */
#ifndef SYS_BUF_WRITE_LEN_UPDATE
#define SYS_BUF_WRITE_LEN_UPDATE(uLen1, uLen2, uRIdx, uWIdx, uBufSize) \
do\
{\
    if(uWIdx >= uRIdx)\
    {\
        uLen1 = uBufSize - uWIdx;\
        uLen2 = uRIdx;\
    }\
    else\
    {\
        uLen1 = uRIdx - uWIdx;\
        uLen2 = 0;\
    }\
}while(0)
#endif

/**@fn         sys_check_sum      
 * @brief      对一块内存空间进行字节和校验
 * @param[in]  pBuf             内存空间指针
 * @param[in]  iLen             长度
 * @return     成功返回校验和  
 */
UINT32 sys_check_sum(VOID *pBuf, INT32 iLen);

/**@fn           sys_htonl
 * @brief      将本地序转换成网络序
 * @param[in]  x  待转换数值
 * @return     转换后的数值
 * @note 如果平台是大端序，内部不做任何转换
 */
UINT32 sys_htonl(UINT32 x);

/**@fn         sys_ntohl
 * @brief      将网络序转换成本地序
 * @param[in]  x  待转换数值
 * @return     转换后的数值
 * @note       如果平台是大端序，内部不做任何转换
 */
UINT32 sys_ntohl(UINT32 x);

/**@fn         sys_htons
 * @brief      将本地序转换成网络序
 * @param[in]  x  待转换数值
 * @return     转换后的数值
 * @note       如果平台是大端序，内部不做任何转换
 */
UINT16 sys_htons(UINT16 x);

/**@fn        sys_ntohs
 * @brief     将网络序转换成本地序
 * @param[in] x  待转换数值
 * @return    转换后的数值
 * @note      如果平台是大端序，内部不做任何转换
 */
UINT16 sys_ntohs(UINT16 x);

/**@fn        sys_ntohll
 * @brief     将网络序转换成本地序
 * @param[in] x  待转换数值
 * @return    转换后的数值
 * @note      如果平台是大端序，内部不做任何转换
 */
UINT64 sys_ntohll(UINT64 x);

/**@fn        sys_htonll
 * @brief     将本地序转换成网络序
 * @param[in] x  待转换数值
 * @return    转换后的数值
 * @note      如果平台是大端序，内部不做任何转换
 */
UINT64 sys_htonll(UINT64 x);

/**@fn        sys_srand
 * @brief     设置产生一系列伪随机数发生器的起始点，要想把发生器重新初始化
 * @param[in] uSeed  随机数产生器的初始值
 * @return    无
 */
VOID sys_srand(UINT32 uSeed);


/**@fn        sys_rand
 * @brief     获取随机数
 * @param[in] 无
 * @return    返回随机数值
 */
INT32 sys_rand(VOID);

#ifdef __cplusplus
}
#endif

#endif