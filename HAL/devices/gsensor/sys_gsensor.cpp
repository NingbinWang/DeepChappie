/*
 * @file sys_gsensor.cpp
 * @brief 
 *
 * @author BSP  
 * @date 2020/04/19  
 *
 * @note 下面的note和warning为可选项目  
 * @note 这里填写本文件的详细功能描述和注解  
 * @note 历史记录：  
 * @note V1.0.0 创建  
 *
 * @warning 这里填写本文件相关的警告信息  
 * encoding UTF-8  
 */
#include "sys_gsensor_priv.h"
#include "sys_imu.h"

/**@fn        sys_gsensor_open    
 * @brief     打开gesnor     
 * @return    成功返回 gsensor的文件描述符 错误返回 ERROR
 */
INT32 sys_gsensor_open()
{
   INT32 fd = -1;
   fd = open(GYRO_DEVICE_NAME,O_RDWR);
   if(fd < 0)
   {
     SYS_COMMON_ERROR("open gsensor failure.fd = %d \n",fd);
     return ERROR;
   }
   return fd;
}

/**@fn         sys_gsensor_close
 * @brief      关闭gsensor的节点
 * @param[in]  gsensor的文件描述符
 * @return     成功返回 OK 错误返回 ERROR
 */
INT32 sys_gsensor_close(const UINT32 fd)
{
    INT32 ret = -1;
    if(fd < 0)
    {
      SYS_COMMON_ERROR("gsensor fd  error \n");
      return ERROR;
    }
    ret  = close(fd);
    if(ret < 0)
    {
      SYS_COMMON_ERROR("close gsensor failure.fd = %d \n",fd);
      return ERROR;
    }
    return OK;
}

/**@fn         sys_gsensor_getname
 * @brief      gsensor的名字
 * @param[in]  gsensor的文件描述符
 * @return      成功返回 字符串 错误返回 NULL
 */
CHAR* sys_gsensor_getname(const UINT32 fd)
{
    struct gyro_info gyroInfo;
    INT32 ret = -1;
    if ( fd < 0)
    {
      SYS_COMMON_ERROR("gsensor fd  error %d,or name is NULL \n",fd);
      return NULL;
    }
    memset(&gyroInfo, 0, sizeof(struct gyro_info));
	ret = ioctl(fd, HAT_GYRO_INIT, &gyroInfo);
	if (0 > ret)
	{
		SYS_COMMON_ERROR("get gsensor id failure.\n");			
		return NULL;
	}
	return gyroInfo.name;
}


/**@fn         sys_gsensor_setpolltime
 * @brief      gsensor的名字
 * @param[in]  gsensor的文件描述符
 * @param[in]  polling的时间
 * @return     成功返回 OK 错误返回 ERROR
 */
INT32 sys_gsensor_setpolltime(const UINT32 fd,const UINT32 polltime)
{
   struct gyro_config gyro_cfg;
   INT32 ret = -1;
   memset(&gyro_cfg, 0, sizeof(struct gyro_config));
   gyro_cfg.poll_tmr = polltime;
   ret = ioctl(fd, HAT_GYRO_TMR_SET, &gyro_cfg);
   if (0 > ret)
   {
		SYS_COMMON_ERROR("HAT_GYRO_TMR_SET set failure.\n");
        return ERROR;
   }
   return OK;
}

/**@fn        sys_gsensor_lowpower
 * @brief      gsensor的低功耗模式
 * @param[in]  gsensor的文件描述符
 * @return     成功返回 OK 错误返回 ERROR
 */
INT32 sys_gsensor_lowpower(const UINT32 fd)
{
  INT32 ret = -1;
  ret = ioctl(fd, HAT_GYRO_LOW_POWER, 0);
  if (0 > ret)
   {
		SYS_COMMON_ERROR("HAT_GYRO_TMR_SET set failure.\n");
        return ERROR;
   }
   return OK;
}

/**@fn        sys_gsensor_lowpower
 * @brief      gsensor的正常模式
 * @param[in]  gsensor的文件描述符
 * @return     成功返回 OK 错误返回 ERROR
 */
INT32 sys_gsensor_normalpower(const UINT32 fd)
{
  INT32 ret = -1;
  ret = ioctl(fd, HAT_GYRO_NORMAL_POWER, 0);
  if (0 > ret)
   {
		SYS_COMMON_ERROR("HAT_GYRO_TMR_SET set failure.\n");
        return ERROR;
   }
   return OK;
}
/**@fn        sys_gsensor_clearint
 * @brief      gsensor的清中断
 * @param[in]  gsensor的文件描述符
 * @return     成功返回 OK 错误返回 ERROR
 */
INT32 sys_gsensor_clearint(const UINT32 fd)
{
  INT32 ret = -1;
  ret = ioctl(fd, HAT_GYRO_CLEAR_INT, 0);
  if (0 > ret)
   {
		  SYS_COMMON_ERROR("HAT_GYRO_TMR_SET set failure.\n");
      return ERROR;
   }
   return ret;
}


/**@fn        sys_gsensor_setwakeup
 * @brief      gsensor的设置唤醒模式
 * @param[in]  gsensor的文件描述符
 * @return     成功返回 OK 错误返回 ERROR
 */
INT32 sys_gsensor_setwakeup(const UINT32 fd,const UINT32 w_threshold)
{
  INT32 ret = -1;
  struct gyro_config gyro_cfg;
  memset(&gyro_cfg, 0, sizeof(struct gyro_config));
  gyro_cfg.wom = WOM_ENABLE;
  gyro_cfg.w_threshold = w_threshold;
  ret = ioctl(fd, HAT_GYRO_WOM_SET, &gyro_cfg);
  if (0 > ret)
   {
		  SYS_COMMON_ERROR("HAT_GYRO_TMR_SET set failure.\n");
      return ERROR;
   }
   return OK;
}
