#include"sys_mem_priv.h"

/**@brief 申请一块固定大小的内存，内部初始化为0
 * @param uSize 需要申请的内存大小
 * @return 成功返回内存指针，失败返回NULL
 */
VOID *sys_mem_malloc(UINT32 uSize)
{   
    void *pData = NULL;
    pData = malloc(uSize);
    if(NULL == pData)
    {
        return NULL;
    }
    memset(pData,0,uSize);
    return pData;
}

/*
 * @brief 申请一块大小为uMemBlock * uSize 的内存，内部初始化为0
 * @param [IN]uMemBlock  申请多少个块
 * @param [IN]uSize 每个单元的大小
 * @return 成功返回内存指针，失败返回NULL
 */
VOID *sys_mem_calloc(UINT32 uMemBlock, UINT32 uSize)
{   
    void *pData = NULL;
    pData = calloc(uMemBlock , uSize);
    if(NULL == pData)
    {
        return NULL;
    }
    return pData;
}

/**@brief 改变指针ptr所指内存块大小
 * @param uSize 调整后的大小，语义同man realloc
 * @return 成功返回内存指针，失败返回NULL
 * @note 如果返回失败，原指针还是有效的
 */
VOID *sys_mem_realloc(VOID *ptr, UINT32 uSize)
{   
    void *pData = NULL;
    pData = realloc(ptr,uSize);
    if(NULL == pData)
    {
        return NULL;
    }
    return pData;
}

/**@brief 申请一块带对齐的内存块，内部初始化为0
 * @param uAlign 需要按多少字节对齐
 * @param uSize 需要申请的内存大小
 * @return 成功返回内存指针，失败返回NULL
 */
VOID *sys_mem_align(UINT32 uAlign, UINT32 uSize)
{
    INT32 iCount = 0;
    UINT32 uAlignSize = 0;
    void *pData = NULL;
    if (uAlign != 0)
    {
        iCount = uSize/uAlign + ((uSize%uAlign)?1:0);
        uAlignSize = iCount*uAlign;
    }
    else
    {
        uAlignSize = uSize;
    }
    pData = malloc(uAlignSize);
    if(!pData)
    {
        return NULL;
    }
    memset(pData,0,uAlignSize);
    return pData;
}

/*
 * @brief 释放内存
 * @param  [IN]func     函数名
 * @param  [IN]line     行号
 * @param  [IN]ptr 需要释放的内存指针
 * @return 无
 */
VOID sys_mem_free(VOID *ptr)
{
    if(ptr == NULL)
    {
        return;
    }
    free(ptr);
    ptr = NULL;
    return;
}

/**@brief 内存拷贝
 * @param  [IN]pDst     目标地址
 * @param  [IN]pDst     源地址
 * @param  [IN]uLength  拷贝长度
 * @return 无
 */
VOID *sys_mem_copy(VOID *pDst, VOID *pSrc, UINT32 uLength)
{
    if(NULL == pDst || NULL == pSrc)
    {
        SYS_COMMON_ERROR("invalid param input\n");
        return NULL;
    }
    memcpy(pDst, pSrc, uLength);
    return pDst;
}