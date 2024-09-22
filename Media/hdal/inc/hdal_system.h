#ifndef _HDAL_SYSTEM_H_
#define _HDAL_SYSTEM_H_

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/
#include "hdal_common.h"




#define OSD_MAX_LINE                    (3)



#define INNER_SHARE_SEGMENT_ID	(120)

/* ========================================================================== */
/*                             Type Declaration                               */
/* ========================================================================== */




/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
INT hdal_Hd_Init(void);
INT hdal_SetMemCfg(HDAL_PROFUCT_ID_E id,HD_COMMON_MEM_INIT_CONFIG *pMem_cfg);
INT hdal_Module_Init(HDAL_PROFUCT_ID_E id);
INT hdal_Sensor_Reset_Pin(HDAL_PROFUCT_ID_E id);
INT hdal_Inner_Init(void);




#ifdef __cplusplus
}
#endif/*__cplusplus*/


#endif
