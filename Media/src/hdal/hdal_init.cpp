#include "hdal_init_priv.h"


/**@fn        sys_hdal_common_init    
 * @brief     初始化hdal的common模块     
 * @return    成功返回 OK 错误返回 ERROR
 */
HD_RESULT sys_hdal_common_init()
{
    HD_RESULT ret;
   	// init hdal
	ret = hd_common_init(0);
	if (ret != HD_OK) {
		printf("hd_common_init error ret=%d\n", ret);
	}
    return ret;
}


/**@fn        sys_hdal_mem_init    
 * @brief     初始化hdal的内存模块     
 * @return    成功返回 OK 错误返回 ERROR
 */

HD_RESULT sys_hdal_mem_init(HD_COMMON_MEM_INIT_CONFIG mem_cfg)
{
	HD_RESULT              ret;
	ret = hd_common_mem_init(&mem_cfg);
	return ret;
}



