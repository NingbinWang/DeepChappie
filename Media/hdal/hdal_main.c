#include "hdal_priv.h"


//HDAL_INNER_PARAM_T* phdalInnerParam = NULL;
//hdal init
int hdal_system_init(HDAL_PROFUCT_ID_E id)
{
	INT ERR; 
	HD_COMMON_MEM_INIT_CONFIG pMem_cfg ={0};
	ERR = hdal_Inner_Init();
	if (ERR != OK) {
         printf("[%s:%d]hdal_Inner_Init=%d\n",__FUNCTION__, __LINE__,ERR);
		return FAIL;
	}
   // phdalInnerParam->bootInfo[CORE_0] = 0x100;
    ERR = hdal_Sensor_Reset_Pin(id);
	if (ERR != OK) {
         printf("[%s:%d]hdal_Inner_Init=%d\n",__FUNCTION__, __LINE__,ERR);
		return FAIL;
	}
	//phdalInnerParam->bootInfo[CORE_0] = 0x101;
    // init hdal
    ERR = hdal_Hd_Init();
	if (ERR != OK) {
         printf("[%s:%d]hdal_hd_Init=%d\n",__FUNCTION__, __LINE__,ERR);
		return FAIL;
	}
	//phdalInnerParam->bootInfo[CORE_0] = 0x102;
    // init memory
	ERR = hdal_SetMemCfg(id,&pMem_cfg);
	if (ERR != OK) {
         printf("[%s:%d]hdal_SetMemCfg=%d\n",__FUNCTION__, __LINE__,ERR);
		return FAIL;
	}
	//phdalInnerParam->bootInfo[CORE_0] = 0x103;
   	// init all modules
	ERR = hdal_Module_Init(id);
	if (ERR != OK) {
         printf("[%s:%d]hdal_Module_Init fail=%d\n",__FUNCTION__, __LINE__,ERR);
		return FAIL;
	}
  //  phdalInnerParam->bootInfo[CORE_0] = 0x104;
	return OK;
}

//hdal vi init
int hdal_vi_init(HDAL_PROFUCT_ID_E id)
{
	INT ERR;
	ERR = hdal_VIsystem_Init(id);
    if (ERR != OK) {
         printf("[%s:%d]hdal_VIsystem_Init fail=%d\n",__FUNCTION__, __LINE__,ERR);
		return FAIL;
	}
	return OK;
}

int hdal_cmd_control(HDAL_CMD_E cmd,HDAL_CMD_DATA_T* pdata)
{
	return 0;
}