#include "app_config.h"
#include "app_storager.h"
#include "app.h"
#include "framework.h"
#include "ANO_FUNC.h"

#include "app_media.h"
#include <pthread.h>
#include <unistd.h>

#include "hal_uvc.h"

App_Defaultconf_t defaultconf;

static int GetBuildTime(void)
{ 
	printf("----------------------------------------\n");
	printf("Build at %s : %s \n",__DATE__,__TIME__);
	printf("----------------------------------------\n");		
	return 0;
}



int app_init()
{
   ANO_Init();
   return 0;
}
UINT32 MediaStartVideoCB(HAL_UVAC_VID_DEV_CNT_E vidDevIdx, HAL_UVAC_STRM_INFO_T *pStrmInfo)
{
  return 0;
}
UINT32 MediaStopVideoCB(UINT32 isClosed)
{
  return 0;
}
/**@fn	       devmonitor_init_interface	  
 * @brief	   初始化对外接口
 * @param[in]  无
 * @return	   成功返回IDevMonitor操作指针  失败返回NULL;
 */
int hal_uvc_init_interface(IHal_UVC *handle)
{   
    int ret = -1;
    if(NULL == handle)
    {
        printf("init interface func error\n");
        return ret;
    }
    handle->xUvacStartVideoCB = MediaStartVideoCB;
    handle->xUvacStopVideoCB = MediaStopVideoCB;
    return 0;
}

/*
0.init_haluvc_component 初始化组件
1.hal_uvc_get_instance 得到组件实例
2.hal_uvc_getneedmemsize 获取UVC必须要的内存大小
3.hd_common_mem_alloc mmz申请内存 初始化uvc_viraddr uvc_phyaddr
4.注册IHal_UVC的各个函数，并实现其中得到功能
5.hal_uvc_init
*/
/*
int app_uvc_init()
{
   IHal_UVC *pIHaluvc_handle= NULL;
   unsigned int uvc_memsize;
   hal_uvc_init_component();
   pIHaluvc_handle= hal_uvc_get_instance();
   uvc_memsize = hal_uvc_getneedmemsize();
   halmedia_mem_alloc((unsigned int)&(pIHaluvc_handle->uvc_phyaddr),(void**)&(pIHaluvc_handle->uvc_viraddr),uvc_memsize);
   hal_uvc_init_interface(pIHaluvc_handle);
   hal_uvc_init(pIHaluvc_handle);
   return 0;
}
*/
int app_main()
{
  GetBuildTime();
  //app_defaultconfig(&defaultconf);//loading ini
  //Framework_Init(&defaultconf);
  app_init();
  app_mediainit();
 // app_uvc_init();
  while (1)
  {
    sleep(1);
  }
  hal_uvc_exit();
  return 0;
}