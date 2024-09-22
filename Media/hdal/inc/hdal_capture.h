#ifndef _HDAL_CAPTURE_H_
#define _HDAL_CAPTURE_H_

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/
#include "hdal_common.h"
#include "Media_vi.h"
#include "Media_frame.h"

/* ========================================================================== */
/*                             Type Declaration                               */
/* ========================================================================== */
#define MAX_VI_BIND_QUEUE_SIZE     (5)
typedef enum
{
    HDAL_VI_OK = 0,              
    HDAL_VI_ERR = -1,
    HDAL_VI_NOSUPPORT= -2,
    HDAL_VI_INVALID_PARAM= -3,
    HDAL_VI_ERR_HARDWARE_NOT_EXIST = -4,
    HDAL_VI_ERR_UNDEFINED = -5,
} HDAL_VI_ERR_E;

typedef struct
{
	BOOL         	bEnable;
	HD_PATH_ID 		CtrlId;     	/*该chn pathId*/
    HD_PATH_ID 		PathIdS;     /*该chn pathId*/
	BOOL            bOpen;	   		/*设备chn open状态*/
    BOOL            bStart;	   		/*设备chn start状态*/
    unsigned int 	bindVpsDevId;     /*绑定到哪个vps dev*/
    unsigned int	DevId;    	 /*VI devID*/
    HD_COMMON_VIDEO_IN_TYPE if_type;
    HD_VIDEOCAP_SEN_INIT_PIN_CFG sen_init_pincfg;
    CHAR sensor_name[HD_VIDEOCAP_SEN_NAME_LEN-1];
    HDAL_MUTEX_T*         mutex;
} HDAL_VI_CHN_INFO_T;
 

typedef struct
{    
	UINT                 lanemode;
    UINT                 chanCnt;
    HDAL_VI_CHN_INFO_T   chanCfg[MAX_ENC_CHAN_SOC];
    VI_CFG_PARAM_T       ViCfgParam[MAX_ENC_CHAN_SOC];
} HDAL_VI_CTRL_T;

typedef struct
{
    UINT           bUsed;
    HD_VIDEO_FRAME viFrameInfo;
} HDAL_VI_FRAME_QUEUE_INFO_T;



INT hdal_VIsystem_Init(HDAL_PROFUCT_ID_E id);
INT hdal_VI_StartChn(UINT32 chan);
INT hdal_VI_StopChn(UINT32 chan);

INT32 hdal_VI_GetChnHDFrame( UINT chan, HD_VIDEO_FRAME *pstFrameInfo, INT32 timeout);
INT32 hdal_VI_ReleaseChnHDFrame(UINT chan, HD_VIDEO_FRAME *pstFrameInfo);
INT32 hdal_VI_SwitchViewMirror(UINT32 chan);
INT32 hdal_VI_SwitchFps(UINT32 chan);

#ifdef __cplusplus
}
#endif/*__cplusplus*/


#endif




