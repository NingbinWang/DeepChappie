#include "sys_block_priv.h"
#include "sys_block.h"
/* 用于该模块的进度打印 */
int sys_block_verbose = 0;

/**@fn        sys_block_size 
 * @brief     获取节点大小      
 * @param[in] dev block节点
 * @return    成功返回节点大小 失败返回0
 */
UINT64 sys_block_size(const CHAR *dev)
{
    INT32 fd = -1;
    INT32 ret = 0;
    UINT64 size = 0ull;
    fd = open(dev,O_RDONLY);
    if(0 > fd)
    {
        SYS_COMMON_ERROR("open %s fail[%s]\n", dev,strerror(errno));
        return 0;
    }
    ret = ioctl(fd, BLKGETSIZE64, &size);
    if(0 > ret)
    {
        SYS_COMMON_ERROR("ioctl %s fail[%s]\n", dev, strerror(errno));
    }
    if(0 < fd)
    {
        close(fd);
        fd = -1;
    }
    return size;
}

/**@fn      sys_block_read
 * @brief   进行读数据
 * @param   [char * ]dev:   块设备节点
 * @param   [in ]offset:    分区偏移位置，要求block对齐
 * @param   [out ]buf:       回读的数据缓存区
 * @param   [in ]len:       待回读长度
 * @return  成功返回OK 失败返回ERROR
 */
INT32 sys_block_read(const CHAR *dev,UINT32 offset,CHAR *buf,int len)
{
    INT32 fd = -1;
    INT32 ret = -1;
    fd = open(dev, O_RDWR);
    if(fd < 0)
    {
        SYS_COMMON_ERROR("open dev %s fail[%d].\n", dev, errno);
        return ERROR;
    }

    ret = pread(fd, buf, len, offset);
    if(ret != len)
    {
        SYS_COMMON_ERROR("block %s read fail[%d].\n", dev, errno);
        close(fd);
        return -ENODEV;
    }
    if(0 < fd)
    {
        close(fd);
        fd = -1;
    }
    return OK;
}

/**@fn      sys_block_write
 * @brief   进行读数据
 * @param   [char * ]dev:   块设备节点
 * @param   [in ]offset:    分区偏移位置，要求对齐
 * @param   [in ]buf:       待写入的数据
 * @param   [in ]len:       待写入的数据长度
 * @return  成功返回OK 失败返回ERROR
 */
INT32 sys_block_write(char *dev, unsigned int offset, char *buf, int len)
{
    INT32 fd = -1;
    INT32 ret = -1;

    fd = open(dev, O_RDWR);
    if(fd < 0)
    {
        SYS_COMMON_ERROR("open dev %s fail[%d].\n", dev, errno);
        return ERROR;
    }

    ret = pwrite(fd, buf, len, offset);
    if(ret != len)
    {
        SYS_COMMON_ERROR("block %s write fail[%d].\n", dev, errno);
        close(fd);
        return -ENODEV;
    }
    if(0 < fd)
    {
        close(fd);
        fd = -1;
    }
    return OK;
}

/**@fn      sys_block_format
 * @brief   进行格式化整个分区
 * @param   [in ]dev:块设备节点
 * @param   [out]N/A
 * @return  成功返回OK 失败返回ERROR
 */
INT32 sys_block_format(const char *dev)
{
    INT32 fd = -1;
    INT32 ret = -1;
    UINT64 offset = 0ull;
    UINT64 size = 0ull;
    INT32 percent = 0;
    INT32 percent_complete = -1;
    CHAR *tmpbuf = NULL;
    UINT32 formatlen = 0;

    size = sys_block_size(dev);
    if(size < 0 || size == 0)
    {
        SYS_COMMON_ERROR("at_block_size dev[%s] fail\n", dev);
        return ERROR;
    }
    fd = open(dev, O_RDWR);
    if(fd < 0)
    {
        SYS_COMMON_ERROR("open dev %s fail[%d].\n", dev, errno);
        return ERROR;
    }
    tmpbuf = (CHAR *)sys_mem_malloc(SYS_BLOCK_TMPLEN);
     if(tmpbuf == NULL)
    {
        SYS_COMMON_ERROR("malloc tmplen fail[%d]\n", errno);
        close(fd);
        fd = -1;
        return ERROR;
    }
    memset(tmpbuf, 0xff, SYS_BLOCK_TMPLEN);
    for (offset = 0; offset < size; offset += SYS_BLOCK_TMPLEN)
    {
        formatlen = ((UINT32)(size - offset) < SYS_BLOCK_TMPLEN) ? (UINT32)(size - offset) : SYS_BLOCK_TMPLEN;
        ret = pwrite(fd, tmpbuf, formatlen, offset);
        if(ret != formatlen)
        {
           SYS_COMMON_ERROR("pwrite offset[0x%llx] fail[%d]\n", offset, errno);
           free(tmpbuf);
           tmpbuf = NULL;
           close(fd);
           fd = -1;
           return ERROR;
        }
        /* 计算完成的百分比 */
        percent = (int)((offset + formatlen) * 100 / size);
        /* 打印进度条，为了减少串口打印频率，完成整数百分比时才打印 */
        if ((percent != percent_complete) && sys_block_verbose)
        {
            percent_complete = percent;
            SYS_COMMON_INFO("\rErasing %s at 0x%lx -- %3d%% complete."
                , dev
                , (unsigned long)(offset + formatlen)
                , (int)percent);
            fflush(stdout);
        }
    }
    SYS_COMMON_INFO("\n");
    if (offset >= size)
    {
        ret = OK;
    }

    if (tmpbuf)
    {
        free(tmpbuf);
        tmpbuf = NULL;
    }
    if (fd > 0)
    {
        close(fd);
        fd = -1;
    }
    return ret;
}

/**@fn      sys_block_avail_space_size
 * @brief   判断BLOCK分区的可用空间大小
 * @param   [in ]dev:块设备节点
 * @param   [out]N/A
 * @return  数据长度
 */
UINT32 sys_block_avail_space_size(const char *dev)
{
    return (UINT32)sys_block_size(dev);
}

/**@fn      sys_block_chk_bank_page
 * @brief   判断是否是bank，即是否全0xff
 * @param   [in ]buf:   检测的数据
 * @param   [out]len:   对应的数据长度
 * @return  TRUE:  是bank page
 * @return  FALSE:  不是是bank page
 */
static BOOL sys_block_chk_bank_page(UINT8 *buf, UINT32 len)
{
    UINT32 i = 0;

    if ((buf == NULL) || !len)
        return FALSE;

    for (i = 0; i < len; i++)
    {
        if (buf[i] != 0xff)
            return FALSE;
    }

    return TRUE;
}


/**@fn      sys_block_valid_data_len
 * @brief   判断BLOCK分区的有效数据长度
 * @param   [in ]dev:块设备节点
 * @param   [out]N/A
 * @return  数据长度 失败返回ERROR
 */
UINT32 sys_block_valid_data_len(const CHAR *dev)
{
    INT32 fd = -1;
    INT32 ret = -1;
    UINT64 offset = 0ull;
    UINT64 size = 0ull;
    CHAR *tmpbuf = NULL;
    UINT32 readlen = 0;
    size = sys_block_size(dev);
    if(size < 0 || size == 0)
    {
        SYS_COMMON_ERROR("at_block_size dev[%s] fail\n", dev);
        return ERROR;
    }

    fd = open(dev, O_RDWR);
    if(fd < 0)
    {
        SYS_COMMON_ERROR("open dev %s fail[%d].\n", dev, errno);
        return ERROR;
    }
    tmpbuf = (CHAR *)sys_mem_malloc(SYS_BLOCK_TMPLEN);
     if(tmpbuf == NULL)
    {
        SYS_COMMON_ERROR("malloc tmplen fail[%d]\n", errno);
        close(fd);
        fd = -1;
        return ERROR;
    }
    memset(tmpbuf, 0xff, SYS_BLOCK_TMPLEN);

    for (offset = 0; offset < size; offset += SYS_BLOCK_TMPLEN)
    {
        readlen = ((UINT32)(size - offset) < SYS_BLOCK_TMPLEN) ? (UINT32)(size - offset) : SYS_BLOCK_TMPLEN;
        ret = pread(fd, tmpbuf, readlen, offset);
        if((UINT32)ret != readlen)
        {
           SYS_COMMON_ERROR("pwrite offset[0x%llx] fail[%d]\n", offset, errno);
           free(tmpbuf);
           tmpbuf = NULL;
           close(fd);
           fd = -1;
           return ERROR;
        }
        if (sys_block_chk_bank_page((UINT8 *)tmpbuf, 16))
        {
            break;
        }
    }
    if (offset > size)
    {
        offset = size;
    }
    if (tmpbuf)
    {
        free(tmpbuf);
        tmpbuf = NULL;
    }
    if (fd > 0)
    {
        close(fd);
        fd = -1;
    }
    return (UINT32)offset;
}

/**@fn      sys_block_copy
 * @brief   进行分区拷贝
 * @param   [in ]src_dev:    源块设备节点
 * @param   [in ]dst_dev:    目的设备节点
 * @return  成功返回OK 失败返回ERROR
 */
INT32 sys_block_copy(const char *src_dev, const char *dst_dev)
{
    INT32 ret = 0;
    INT32 fd_src = 0;
    INT32 fd_dst = 0;
    UINT64 offset = 0ull;
    UINT64 src_size = 0ull;
    UINT64 dst_size = 0ull;
    CHAR *tmpbuf = NULL;
    UINT32 readlen = 0;
    INT32 percent;
    INT32 percent_complete = -1;

    src_size = sys_block_size(src_dev);
    if(src_size < 0 || src_size == 0)
    {
        SYS_COMMON_ERROR("sys_block_size src_dev[%s] fail\n", src_dev);
        return ERROR;
    }
    dst_size = sys_block_size(dst_dev);
    if(src_size < 0 || src_size == 0)
    {
        SYS_COMMON_ERROR("sys_block_size dst_dev[%s] fail\n", dst_dev);
        return ERROR;
    }
    if(src_size != dst_size)
    {
        SYS_COMMON_ERROR("src_size[0x%llx] != dst_size[0x%llx]\n", src_size, dst_size);
        return ERROR;
    }

    fd_src = open(src_dev, O_RDWR);
    if(fd_src < 0)
    {
        SYS_COMMON_ERROR("open %s fail[%d]\n", src_dev, errno);
        return ERROR;
    }
    fd_dst = open(dst_dev, O_RDWR);
    if(fd_dst < 0)
    {
        SYS_COMMON_ERROR("open %s fail[%d]\n", fd_dst, errno);
        return ERROR;
    }

    tmpbuf =(CHAR *)sys_mem_malloc(SYS_BLOCK_TMPLEN);
     if(tmpbuf == NULL)
    {
        SYS_COMMON_ERROR("malloc tmplen fail[%d]\n", errno);
        close(fd_dst);
        close(fd_src);
        return ERROR;
    }

    for (offset = 0; offset < src_size; offset += SYS_BLOCK_TMPLEN)
    {
        readlen = ((UINT32)(src_size - offset) < SYS_BLOCK_TMPLEN) ? (UINT32)(src_size - offset) : SYS_BLOCK_TMPLEN;
        ret = pread(fd_src, tmpbuf, readlen, offset);
        if((UINT32)ret != readlen)
        {
           SYS_COMMON_ERROR("pread offset[0x%llx] fail[%d]\n", offset, errno);
           free(tmpbuf);
           tmpbuf = NULL;
           close(fd_dst);
           close(fd_src);
           return ERROR;
        }
        ret = pwrite(fd_dst, tmpbuf, readlen, offset);
        if((UINT32)ret != readlen)
        {
           SYS_COMMON_ERROR("pwrite offset[0x%llx] fail[%d]\n", offset, errno);
           free(tmpbuf);
           tmpbuf = NULL;
           close(fd_dst);
           close(fd_src);
           return ERROR;
        }

        if (sys_block_chk_bank_page((UINT8 *)tmpbuf, 16))
        {
            ret = 0;
            offset = src_size;
            readlen = 0;
        }
        /* 计算完成的百分比 */
        percent = (int)((offset + readlen) * 100 / src_size);
        /* 打印进度条，为了减少串口打印频率，完成整数百分比时才打印 */
        if ((percent != percent_complete) && sys_block_verbose)
        {
            percent_complete = percent;
            SYS_COMMON_INFO("\rCopying %s => %s at 0x%lx -- %3d%% complete."
                , src_dev, dst_dev
                , (unsigned long)(offset + readlen)
                , (int)percent);
            fflush(stdout);
        }
    }
    SYS_COMMON_INFO("\n");
    if (offset > src_size)
    {
        offset = src_size;
        ret = OK;
    }
     if (tmpbuf)
    {
        free(tmpbuf);
        tmpbuf = NULL;
    }
    if (fd_src > 0)
    {
        close(fd_src);
        fd_src = -1;
    }
    if (fd_dst > 0)
    {
        close(fd_dst);
        fd_dst = -1;
    }
    return ret;
}

/**@fn      sys_block_cmp
 * @brief   进行分区数据比较，支持跳坏块
 * @param   [in ]src_dev:    源块设备节点
 * @param   [in ]dst_dev:    目的设备节点
 * @return  OK: 数据一致 
 * @return  ERROR: 数据不一致
 */
INT32 sys_block_cmp(const char *src_dev, const char *dst_dev)
{
    INT32 ret = 0;
    INT32 fd_src = 0;
    INT32 fd_dst = 0;
    UINT64 offset = 0ull;
    UINT64 src_size = 0ull;
    UINT64 dst_size = 0ull;
    CHAR *tmpbuf1 = NULL;
    CHAR *tmpbuf2 = NULL;
    UINT32 readlen = 0;

      src_size = sys_block_size(src_dev);
    if(src_size < 0 || src_size == 0)
    {
        SYS_COMMON_ERROR("sys_block_size src_dev[%s] fail\n", src_dev);
        return ERROR;
    }
    dst_size = sys_block_size(dst_dev);
    if(src_size < 0 || src_size == 0)
    {
        SYS_COMMON_ERROR("sys_block_size dst_dev[%s] fail\n", dst_dev);
        return ERROR;
    }
    if(src_size != dst_size)
    {
        SYS_COMMON_ERROR("src_size[0x%llx] != dst_size[0x%llx]\n", src_size, dst_size);
        return ERROR;
    }

    fd_src = open(src_dev, O_RDWR);
    if(fd_src < 0)
    {
        SYS_COMMON_ERROR("open %s fail[%d]\n", src_dev, errno);
        return ERROR;
    }
    fd_dst = open(dst_dev, O_RDWR);
    if(fd_dst < 0)
    {
        SYS_COMMON_ERROR("open %s fail[%d]\n", fd_dst, errno);
        return ERROR;
    }

    tmpbuf1 = (CHAR *)sys_mem_malloc(SYS_BLOCK_TMPLEN);
     if(tmpbuf1 == NULL)
    {
        SYS_COMMON_ERROR("malloc tmplen1 fail[%d]\n", errno);
        close(fd_dst);
        close(fd_src);
        return ERROR;
    }

    tmpbuf2 = (CHAR *)sys_mem_malloc(SYS_BLOCK_TMPLEN);
     if(tmpbuf2 == NULL)
    {
        SYS_COMMON_ERROR("malloc tmplen2 fail[%d]\n", errno);
        free(tmpbuf1);
         tmpbuf1 = NULL;
        close(fd_dst);
        close(fd_src);
        return ERROR;
    }

    for (offset = 0; offset < src_size; offset += SYS_BLOCK_TMPLEN)
    {
        readlen = ((UINT32)(src_size - offset) < SYS_BLOCK_TMPLEN) ? (UINT32)(src_size - offset) : SYS_BLOCK_TMPLEN;
        ret = pread(fd_src, tmpbuf1, readlen, offset);
        if((UINT32)ret != readlen)
        {
           SYS_COMMON_ERROR("pread offset[0x%llx] fail[%d]\n", offset, errno);
           free(tmpbuf1);
           free(tmpbuf2);
           tmpbuf1 = NULL;
           tmpbuf2 = NULL;
           close(fd_dst);
           close(fd_src);
           return ERROR;
        }
        if (sys_block_chk_bank_page((UINT8 *)tmpbuf1, 16))
        {
            break;
        }
        ret = pread(fd_dst, tmpbuf2, readlen, offset);
        if((UINT32)ret != readlen)
        {
           SYS_COMMON_ERROR("pread offset[0x%llx] fail[%d]\n", offset, errno);
           free(tmpbuf1);
           free(tmpbuf2);
           tmpbuf1 = NULL;
           tmpbuf2 = NULL;
           close(fd_dst);
           close(fd_src);
           return ERROR;
        }
        ret = memcmp(tmpbuf1, tmpbuf2, readlen);
        if (ret)
        {
            /* 返回，说明数据不一致 */
            SYS_COMMON_ERROR("block_cmp [%s,%llu] != [%s,%llu]\n", src_dev, offset, dst_dev, offset);
            free(tmpbuf1);
           free(tmpbuf2);
           tmpbuf1 = NULL;
           tmpbuf2 = NULL;
           close(fd_dst);
           close(fd_src);
           return ERROR;
        }
    }
    if (offset > src_size)
    {
        offset = src_size;
        ret = OK;
    }
    if (tmpbuf1)
    {
        free(tmpbuf1);
        tmpbuf1 = NULL;
    }
    if (tmpbuf2)
    {
        free(tmpbuf2);
        tmpbuf2 = NULL;
    }
    if (fd_dst > 0)
    {
        close(fd_dst);
        fd_dst = -1;
    }
    if (fd_src > 0)
    {
        close(fd_src);
        fd_src = -1;
    }
    return ret;
}

