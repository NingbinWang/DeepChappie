#include "hdal_system.h"


HDAL_INNER_PARAM_T *phdalInnerParam = NULL;

static CHAR gpio_path[100];
static INT gpio_config(const CHAR *attr,const CHAR *val)
{
    CHAR file_path[100];
    INT len;
    INT fd;

    // 拼接gpio控制文件地址
     sprintf(file_path,"%s/%s",gpio_path,attr); 
    // 打开gpio文件失败则返回
    if(0 > (fd = open(file_path,O_WRONLY)))
    {
         printf("[%s:%d]open %s error \n", __FUNCTION__, __LINE__,file_path);
         return fd;
    }
    
    // 计算字符串长度
     len = strlen(val);
    // 字符串长度与写入长度对比，一致则写入成功，否则返回-1
     if(len != write(fd,val,len))
     {
         printf("[%s:%d]write fail len=%d,val=%s\n", __FUNCTION__, __LINE__,len,val);
         close(fd);
         return -1;
     }

    // 关闭文件
     close(fd);
    return 0;
}

INT hdal_Reset_Pin(CHAR* gpionum)
{
    // 拼接gpio目录地址
    sprintf(gpio_path,"/sys/class/gpio/gpio%s",gpionum);
    // 查询目录是否存在，不存在则使用export导出
     if(access(gpio_path,F_OK))
     {
         INT fd;
         INT len;
         // 打开gpio导出配置文件
         if (0 > (fd = open("/sys/class/gpio/export", O_WRONLY))) 
        {
             printf("[%s:%d]open error",__FUNCTION__, __LINE__);
             return -1;
        }
         // 写入需要导出的gpio
         len = strlen(gpionum);
         if (len != write(fd, gpionum, len)) 
         {
             printf("[%s:%d]write error",__FUNCTION__, __LINE__);
             close(fd);
             return -1;
         }
 
         // 导出成功，关闭文件
         close(fd); 
     }
 
     // 配置为输出模式
     if (gpio_config("direction", "out"))
     {
        return -1;
     }
    // 极性设置 
     if (gpio_config("active_low", "0"))
     {
         return -1;
     }
     // 控制 GPIO 输出高低电平 
     if (gpio_config("value", "1"))
     {
         return -1;
     }
     usleep(3000);
    if (gpio_config("value", "0"))
     {
         return -1;
     }
    usleep(3000);
    if (gpio_config("value", "1"))
     {
         return -1;
     }
     return 0;

}

INT hdal_Sensor_Reset_Pin(HDAL_PROFUCT_ID_E id)
{
    int ret = 0;
    switch (id)
    {
       case HDAL_PROFUCT_ID_A30051:
         {
           ret =  hdal_Reset_Pin("69");
           if(ret != 0 )
           {
             printf("[%s:%d]reset sensor 69 pin id:=%d\n",__FUNCTION__, __LINE__,id);
             return ret;
           }
           break;
         }
       default :
		{
			printf("[%s:%d]not need reset sensor pin id:=%d\n",__FUNCTION__, __LINE__,id);
        }
    }
    return ret;
}

INT hdal_Hd_Init(void)
{
	HD_RESULT ret;  
	 // init hdal
    ret = hd_common_init(0);
	if (ret != HD_OK) {
        printf("[%s:%d]common fail=%d\n",__FUNCTION__, __LINE__,ret);
		return -1;
	}

	ret = hd_common_sysconfig(HD_VIDEOPROC_CFG, 0, HD_VIDEOPROC_CFG_DISABLE_GDC, 0);
	if (ret != HD_OK) {
        printf("[%s:%d]hd_common_sysconfig fail=%d\n",__FUNCTION__, __LINE__,ret);
		return -1;
	}
	return 0;
}

INT hdal_SetMemCfg(HDAL_PROFUCT_ID_E id ,HD_COMMON_MEM_INIT_CONFIG *pMem_cfg)
{
	HD_RESULT ret;
	
	switch (id)
    {
		case HDAL_PROFUCT_ID_DEMO:
		{
		   pMem_cfg->pool_info[0].type = HD_COMMON_MEM_COMMON_POOL;
	       pMem_cfg->pool_info[0].blk_size = DBGINFO_BUFSIZE()+VDO_RAW_BUFSIZE(VDO_SIZE_W, VDO_SIZE_H,CAP_OUT_FMT)
														+VDO_CA_BUF_SIZE(CA_WIN_NUM_W, CA_WIN_NUM_H)
														+VDO_LA_BUF_SIZE(LA_WIN_NUM_W, LA_WIN_NUM_H);
	      pMem_cfg->pool_info[0].blk_cnt = 3;
	      pMem_cfg->pool_info[0].ddr_id = DDR_ID0;
		  break;
		}
        case HDAL_PROFUCT_ID_A30051:
		{
		   pMem_cfg->pool_info[0].type = HD_COMMON_MEM_COMMON_POOL;
	       pMem_cfg->pool_info[0].blk_size = DBGINFO_BUFSIZE()+VDO_RAW_BUFSIZE(VDO_SIZE_W, VDO_SIZE_H,CAP_OUT_FMT)
														+VDO_CA_BUF_SIZE(CA_WIN_NUM_W, CA_WIN_NUM_H)
														+VDO_LA_BUF_SIZE(LA_WIN_NUM_W, LA_WIN_NUM_H);
	      pMem_cfg->pool_info[0].blk_cnt = 3;
	      pMem_cfg->pool_info[0].ddr_id = DDR_ID0;
		  break;
		}
        case HDAL_PROFUCT_ID_B20066:
        {
			int i = 0;
			pMem_cfg->pool_info[i].type = HD_COMMON_MEM_COMMON_POOL;
            pMem_cfg->pool_info[i].ddr_id = DDR_ID0;
			pMem_cfg->pool_info[i].blk_cnt = 16 ;
			pMem_cfg->pool_info[i].blk_size = DBGINFO_BUFSIZE()+VDO_YUV_BUFSIZE(1920, 1080, HD_VIDEO_PXLFMT_YUV420);
			i++;//i=2;
			pMem_cfg->pool_info[i].type = HD_COMMON_MEM_COMMON_POOL;
            pMem_cfg->pool_info[i].ddr_id = DDR_ID0;
			pMem_cfg->pool_info[i].blk_cnt = 4 ;
			pMem_cfg->pool_info[i].blk_size = DBGINFO_BUFSIZE()+VDO_YUV_BUFSIZE(ALIGN_CEIL_64(854), ALIGN_CEIL_64(480), HD_VIDEO_PXLFMT_YUV420);
			i++;//i=2;
			pMem_cfg->pool_info[i].type = HD_COMMON_MEM_COMMON_POOL;
            pMem_cfg->pool_info[i].ddr_id = DDR_ID0;
			pMem_cfg->pool_info[i].blk_cnt = 4 ;
			pMem_cfg->pool_info[i].blk_size = DBGINFO_BUFSIZE()+VDO_YUV_BUFSIZE(1280, 800, HD_VIDEO_PXLFMT_YUV420);

			i++;//i=3;	
			pMem_cfg->pool_info[i].type = HD_COMMON_MEM_COMMON_POOL;
            pMem_cfg->pool_info[i].ddr_id = DDR_ID0;
			pMem_cfg->pool_info[i].blk_cnt = 3 ;
			pMem_cfg->pool_info[i].blk_size = DBGINFO_BUFSIZE()+VDO_YUV_BUFSIZE(ALIGN_CEIL_64(400), ALIGN_CEIL_64(228), HD_VIDEO_PXLFMT_YUV420);
      		i++;//i=6;AO
			/* vdec buffer for bs pushing in */
			pMem_cfg->pool_info[i].type = HD_COMMON_MEM_USER_POOL_BEGIN;
			pMem_cfg->pool_info[i].blk_size = 0x100000;
			pMem_cfg->pool_info[i].blk_cnt = 1;
			pMem_cfg->pool_info[i].ddr_id = DDR_ID0;
			i++;//i=6;
        	/* dummy buffer, not for audio module */
            pMem_cfg->pool_info[i].type = HD_COMMON_MEM_COMMON_POOL;
            pMem_cfg->pool_info[i].blk_size = 0x1000;
            pMem_cfg->pool_info[i].blk_cnt = 1;
            pMem_cfg->pool_info[i].ddr_id = DDR_ID0;
        	i++;//i=7;
            /* user buffer for bs pushing in */
            pMem_cfg->pool_info[i].type = HD_COMMON_MEM_USER_POOL_BEGIN;
            pMem_cfg->pool_info[i].blk_size = 0x10000;
            pMem_cfg->pool_info[i].blk_cnt = 2;
            pMem_cfg->pool_info[i].ddr_id = DDR_ID0;            
         	i++;//i=8;
            pMem_cfg->pool_info[i].type = HD_COMMON_MEM_OSG_POOL;
		    pMem_cfg->pool_info[i].blk_size = 1280*48*4*(GET_ENC_CHAN_NUM * 3)*OSD_MAX_LINE;
		    pMem_cfg->pool_info[i].blk_cnt = 1;
		    pMem_cfg->pool_info[i].ddr_id = DDR_ID0;
			i++;//i=10;OSD		
			pMem_cfg->pool_info[i].type = HD_COMMON_MEM_OSG_POOL;
			pMem_cfg->pool_info[i].blk_size = 512*256*4*(GET_ENC_CHAN_NUM * 3);
			pMem_cfg->pool_info[i].blk_cnt = 1;
			pMem_cfg->pool_info[i].ddr_id = DDR_ID0;		    
			break;
        }	
        default :
		{
			printf("[%s:%d]not support id:=%d\n",__FUNCTION__, __LINE__,id);
            return  -1;
        }
    }
	ret = hd_common_mem_init(pMem_cfg);
	if (HD_OK != ret) 
	{
		printf("[%s:%d]hd_common_mem_init fail=%d\n",__FUNCTION__, __LINE__,ret);
		return -1;
	}
	return 0;
}

INT hdal_Module_Init(HDAL_PROFUCT_ID_E id)
{
	HD_RESULT ret;
	
	switch (id)
	{
		case HDAL_PROFUCT_ID_DEMO:
		{
			ret = hd_videocap_init();
			if (HD_OK != ret) 
	    	{
			   printf("[%s:%d]hd_videocap_init fail=%d\n",__FUNCTION__, __LINE__,ret);
			   return -1;
	    	}
			break;
		}
		case HDAL_PROFUCT_ID_A30051:
		{
	    	ret = hd_videocap_init();
			if (HD_OK != ret) 
	    	{
			   printf("[%s:%d]hd_videocap_init fail=%d\n",__FUNCTION__, __LINE__,ret);
			   return -1;
	    	}
		
			ret = hd_videoproc_init();
			if (HD_OK != ret) 
	    	{
			   printf("[%s:%d]hd_videoproc_init fail=%d\n",__FUNCTION__, __LINE__,ret);
			   return -1;
	    	}

			ret = hd_videoenc_init();
			if (HD_OK != ret) 
	    	{
			   printf("[%s:%d]hd_videoenc_init fail=%d\n",__FUNCTION__, __LINE__,ret);
			   return -1;
	    	}

			ret = hd_gfx_init();
			if (HD_OK != ret) 
	    	{
			   printf("[%s:%d]hd_gfx_init fail=%d\n",__FUNCTION__, __LINE__,ret);
			   return -1;
	    	}
			break;
		}
		case HDAL_PROFUCT_ID_B20066:
		{
	    	ret = hd_videocap_init();
			if (HD_OK != ret) 
	    	{
			   printf("[%s:%d]hd_videocap_init fail=%d\n",__FUNCTION__, __LINE__,ret);
			   return -1;
	    	}
		
			ret = hd_videoproc_init();
			if (HD_OK != ret) 
	    	{
			   printf("[%s:%d]hd_videoproc_init fail=%d\n",__FUNCTION__, __LINE__,ret);
			   return -1;
	    	}

			ret = hd_videoenc_init();
			if (HD_OK != ret) 
	    	{
			   printf("[%s:%d]hd_videoenc_init fail=%d\n",__FUNCTION__, __LINE__,ret);
			   return -1;
	    	}

			ret = hd_gfx_init();
			if (HD_OK != ret) 
	    	{
			   printf("[%s:%d]hd_gfx_init fail=%d\n",__FUNCTION__, __LINE__,ret);
			   return -1;
	    	}
			break;
	    }
		default :
		{
			printf("[%s:%d]not support id:=%d\n",__FUNCTION__, __LINE__,id);
            return  -1;
        }
	}
	return 0;
}

INT hdal_Inner_Init(void)
{
	int shmId = 0;
	/* 创建DSP内部共享内存 */
	shmId = shmget((key_t)INNER_SHARE_SEGMENT_ID,sizeof(HDAL_INNER_PARAM_T),IPC_CREAT|0666);
	if(-1 == shmId)
	{
		printf("[%s:%d]error shmKey =%d\n",__FUNCTION__, __LINE__,shmId);
        return  -1;
	}
	phdalInnerParam =(HDAL_INNER_PARAM_T *)shmat(shmId, NULL, 0);
	if ((void *)(-1) == (void *)(phdalInnerParam))
	{
		printf("[%s:%d]!!! shmat HDAL_INNER_PARAM_T SIZE=0x%x failed to exit !!!\n",__FUNCTION__, __LINE__,sizeof(HDAL_INNER_PARAM_T));
        return  -1;
	}
	return 0; 
}

int hdal_Mem_Malloc(unsigned int *phy_addr, void **virt_addr, unsigned int size)
{
	HD_RESULT ret;
	ret = hd_common_mem_alloc("usbmovie", (UINT32 *)phy_addr, virt_addr, (UINT32)size, DDR_ID0);
	if(ret != HD_OK)
	{
		printf("[%s:%d]hd_common_mem_alloc failed ret = %d\n",__FUNCTION__, __LINE__,ret);
		return -1;
	}
	return 0;
}


HDAL_MUTEX_T* hdal_Mutex_Create(CHAR *name)
{
	  pthread_mutexattr_t  attr;
	  HDAL_MUTEX_T *hadl_mutex= (HDAL_MUTEX_T *)malloc(sizeof(HDAL_MUTEX_T));
	  if (pthread_mutexattr_init(&attr)!= 0)
	  {
		printf("[%s:%d]pthread_mutexattr_init error.\n",__FUNCTION__, __LINE__);
		return NULL;
	  }
    
	if (pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP) != 0)
	{
		printf("[%s:%d]pthread_mutexattr_settype error.\n",__FUNCTION__, __LINE__);
		return NULL;
	}
    
	if (pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT) != 0)
	{
		printf("[%s:%d]pthread_mutexattr_settype error.\n",__FUNCTION__, __LINE__);
		return NULL;
	}
    
	if (pthread_mutex_init(&hadl_mutex->mutex, &attr)!= 0)
	{
		printf("[%s:%d]OsalMutex_create error.\n",__FUNCTION__, __LINE__);
		return NULL;
	}
	strcpy((char *)hadl_mutex->name, name);
	hadl_mutex->magic = HDAL_MUTEX_MAGIC;
	return hadl_mutex;
}

INT hdal_Mutex_lock(HDAL_MUTEX_T* phdalMutexHandle)
{
	if(!phdalMutexHandle)
	{
		printf("[%s:%d]phdalMutexHandle is NULL\n",__FUNCTION__, __LINE__);
		return -1;
	}
	if (phdalMutexHandle->magic != HDAL_MUTEX_MAGIC)
	{
		printf("[%s:%d]hdal_Mutex_lock param error.\n",__FUNCTION__, __LINE__);
		return -1;
	}
	if (pthread_mutex_lock(&phdalMutexHandle->mutex))
	{
		printf("[%s:%d]hdal_Mutex_lockerror.\n",__FUNCTION__, __LINE__);
        return -1;
	}
	return 0;
}

INT hdal_Mutex_unlock(HDAL_MUTEX_T* phdalMutexHandle)
{
	if(!phdalMutexHandle)
	{
		printf("[%s:%d]phdalMutexHandle is NULL\n",__FUNCTION__, __LINE__);
		return -1;
	}
	if (phdalMutexHandle->magic != HDAL_MUTEX_MAGIC)
	{
		printf("[%s:%d]hdal_Mutex_unlock param error.\n",__FUNCTION__, __LINE__);
		return -1;
	}
	if (pthread_mutex_unlock(&phdalMutexHandle->mutex))
	{
		printf("[%s:%d]hdal_Mutex_unlockerror.\n",__FUNCTION__, __LINE__);
        return -1;
	}
	return 0;
}

INT hdal_Mutex_delete(HDAL_MUTEX_T* phdalMutexHandle)
{
	if(!phdalMutexHandle)
	{
		printf("[%s:%d]phdalMutexHandle is NULL\n",__FUNCTION__, __LINE__);
		return -1;
	}
	free(phdalMutexHandle);
	return 0;
}