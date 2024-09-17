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
#include "sys_gsensor.h"

/**@fn        sys_gsensor_open    
 * @brief     打开gesnor     
 * @return    成功返回 gsensor的文件描述符 错误返回 ERROR
 */
INT sys_gsensor_open()
{
   INT fd = -1;
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
INT sys_gsensor_close(const UINT fd)
{
    INT ret = -1;
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


/**@fn         sys_gsensor_setpolltime
 * @brief      gsensor的名字
 * @param[in]  gsensor的文件描述符
 * @param[in]  polling的时间
 * @return     成功返回 OK 错误返回 ERROR
 */
INT sys_gsensor_setpolltime(const UINT fd,const UINT polltime)
{
   struct gyro_config gyro_cfg;
   INT ret = -1;
    if(fd < 0)
    {
      SYS_COMMON_ERROR("gsensor fd  error \n");
      return ERROR;
    }
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
INT sys_gsensor_lowpower(const UINT fd)
{
  INT ret = -1;
  if(fd < 0)
  {
      SYS_COMMON_ERROR("gsensor fd  error \n");
      return ERROR;
  }
  /*
  ret = ioctl(fd, HAT_GYRO_LOW_POWER, 0);
  if (0 > ret)
  {
		SYS_COMMON_ERROR("HAT_GYRO_TMR_SET set failure.\n");
        return ERROR;
   }
   */
   return OK;
}

/**@fn        sys_gsensor_nomalpower
 * @brief      gsensor的正常模式
 * @param[in]  gsensor的文件描述符
 * @return     成功返回 OK 错误返回 ERROR
 */
INT sys_gsensor_normalpower(const UINT fd)
{
  INT ret = -1;
  if(fd < 0)
  {
      SYS_COMMON_ERROR("gsensor fd  error \n");
      return ERROR;
  }
  /*
  ret = ioctl(fd, HAT_GYRO_NORMAL_POWER, 0);
  if (0 > ret)
   {
		SYS_COMMON_ERROR("HAT_GYRO_TMR_SET set failure.\n");
        return ERROR;
   }
   */
   return OK;
}
/**@fn        sys_gsensor_clearint
 * @brief      gsensor的清中断
 * @param[in]  gsensor的文件描述符
 * @return     成功返回 OK 错误返回 ERROR
 */
INT sys_gsensor_clearint(const UINT fd)
{
  INT ret = -1;
  if(fd < 0)
  {
      SYS_COMMON_ERROR("gsensor fd  error \n");
      return ERROR;
  }
  /*
  ret = ioctl(fd, HAT_GYRO_CLEAR_INT, 0);
  if (0 > ret)
   {
		  SYS_COMMON_ERROR("HAT_GYRO_TMR_SET set failure.\n");
      return ERROR;
   }
   */
   return ret;
}


/**@fn        sys_gsensor_setwakeup
 * @brief      gsensor的设置唤醒模式
 * @param[in]  gsensor的文件描述符
 * @return     成功返回 OK 错误返回 ERROR
 */
INT sys_gsensor_setwakeup(const UINT fd,const UINT w_threshold)
{
  INT ret = -1;
  struct gyro_config gyro_cfg;
  if(fd < 0)
  {
      SYS_COMMON_ERROR("gsensor fd  error \n");
      return ERROR;
  }
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

/**@fn        sys_gesnsor_getdata
 * @brief      gsensor的获取数据
 * @param[in]  gsensor的文件描述符
 * @return     成功返回 OK 错误返回 ERROR
 */

INT sys_gsensor_getdata(const UINT fd,sensor_t * sensordata)
{
   INT ret = -1;
   struct gyro_value gyro_val;
   if(fd < 0)
   {
      SYS_COMMON_ERROR("gsensor fd  error \n");
      return ERROR;
   }
   memset(&gyro_val,0,sizeof(struct gyro_value));
   ret = read(fd,(char *)&gyro_val, sizeof(struct gyro_value));
   if (sizeof(struct gyro_value) != ret)
   {
		  SYS_COMMON_ERROR("read data error failure.\n");
      return ERROR;
   }
   sensordata->accx=gyro_val.accel_x;
   sensordata->accy=gyro_val.accel_y;
   sensordata->accz=gyro_val.accel_z;
   sensordata->gyrox=gyro_val.gyro_x;
   sensordata->gyroy=gyro_val.gyro_y;
   sensordata->gyroz=gyro_val.gyro_z;
   return OK;

}

/**@fn        sys_gsensor_getinfo
 * @brief      gsensor的获取芯片信息
 * @param[in]  gsensor的文件描述符
 * @return     成功返回 OK 错误返回 ERROR
 */

INT sys_gsensor_getinfo(const UINT fd,sensorinfo_t * info)
{
  INT ret = ERROR;
  struct gyro_info gsensorinfo;
  if(fd < 0)
  {
      SYS_COMMON_ERROR("gsensor fd  error \n");
      return ERROR;
  }
  memset(&gsensorinfo, 0, sizeof(struct gyro_info));
 
  ret = ioctl(fd, HAT_GYRO_INIT, &gsensorinfo);
  if (0 > ret)
   {
		  SYS_COMMON_ERROR("HAT_GYRO_INIT get failure.\n");
      return ERROR;
   }
  memcpy(info->gyroname ,gsensorinfo.name,strlen(gsensorinfo.name));
  switch (gsensorinfo.type)
  {
  case GYRO_TYPE_DEFAULT:
    info->AXIS = 0;
    break;
  case GYRO_BYTE_THREE_AXIS:
    info->AXIS = 3;
    break;
  case GYRO_TYPE_SIX_AXIS:
    info->AXIS = 6;
    break;
  case GYRO_TYPE_NINE_AXIS:
    info->AXIS = 9;
    break;
  default:
    info->AXIS = 0;
    break;
  }
   return OK;
}
/**@fn        sys_gsensor_getconfig
 * @brief      gsensor的获取量程
 * @param[in]  gsensor的文件描述符
 * @return     成功返回 OK 错误返回 ERROR
 */

INT sys_gsensor_getconfig(const UINT fd,sensorinfo_t * info)
{
  INT ret = -1;
 struct gyro_config gyro_cfg;
  if(fd < 0)
  {
      SYS_COMMON_ERROR("gsensor fd  error \n");
      return ERROR;
  }
  memset(&gyro_cfg, 0, sizeof(struct gyro_config));
 
  ret = ioctl(fd, HAT_GYRO_READ_CONFIG, &gyro_cfg);
  if (0 > ret)
   {
		  SYS_COMMON_ERROR("HAT_GYRO_INIT get failure.\n");
      return ERROR;
   }
   info->accrng = gyro_cfg.accel_rng;
   info->gyrorng = gyro_cfg.gyro_rng;
   return OK;
}

/**@fn        sys_gsensor_setconfig
 * @brief      gsensor的设置量程
 * @param[in]  gsensor的文件描述符
 * @return     成功返回 OK 错误返回 ERROR
 */

INT sys_gsensor_setconfig(const UINT fd,const UINT accrng)
{
  INT ret = -1;
 struct gyro_config gyro_cfg;
  if(fd < 0)
  {
      SYS_COMMON_ERROR("gsensor fd  error \n");
      return ERROR;
  }
  memset(&gyro_cfg, 0, sizeof(struct gyro_config));
  gyro_cfg.accel_rng = accrng;
  ret = ioctl(fd, HAT_GYRO_SET_RNG, &gyro_cfg);
  if (0 > ret)
   {
		  SYS_COMMON_ERROR("HAT_GYRO_INIT get failure.\n");
      return ERROR;
   }
   return OK;
}

/**@fn        sys_gsensor_getparam
 * @brief      gsensor的获取数据值
 * @param[in]  gsensor的文件描述符
 * @return     成功返回 OK 错误返回 ERROR
 */

INT sys_gsensor_getparam(const UINT fd,sensorinfo_t * info)
{
  INT ret = ERROR;
  struct gyro_param gsensorparam;
  if(fd < 0)
  {
      SYS_COMMON_ERROR("gsensor fd  error \n");
      return ERROR;
  }
  memset(&gsensorparam, 0, sizeof(struct gyro_param));
 /*
  ret = ioctl(fd, HAT_GYRO_GET_PARAM, &gsensorparam);
  if (0 > ret)
   {
		  SYS_COMMON_ERROR("HAT_GYRO_INIT get failure.\n");
      return ERROR;
   }
   info->accvalidnum = gsensorparam.accvalidnum;
   info->gyrovalidnum = gsensorparam.gyrovalidnum;
   info->acccoef = gsensorparam.acccoef;
   info->gyrocoef = gsensorparam.gyrocoef;
   */
   return OK;
}
