#ifndef _HDAL_COMMON_H_
#define _HDAL_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/wait.h>

#include "hdal.h"
#include "hd_debug.h"
#include "hd_type.h"
#include "hdal_interface.h"

/* ========================================================================== */
/*                             Type Declaration                               */
/* ========================================================================== */

//header
#define DBGINFO_BUFSIZE()	(0x200)
//RAW
#define VDO_RAW_BUFSIZE(w, h, pxlfmt)   (ALIGN_CEIL_4((w) * HD_VIDEO_PXLFMT_BPP(pxlfmt) / 8) * (h))
//YUV
#define VDO_YUV_BUFSIZE(w, h, pxlfmt)	ALIGN_CEIL_4(((w) * (h) * HD_VIDEO_PXLFMT_BPP(pxlfmt)) / 8)
//CA for AWB
#define VDO_CA_BUF_SIZE(win_num_w, win_num_h) ALIGN_CEIL_4((win_num_w * win_num_h << 3) << 1)
//LA for AE
#define VDO_LA_BUF_SIZE(win_num_w, win_num_h) ALIGN_CEIL_4((win_num_w * win_num_h << 1) << 1)

#define HD_VIDEOPROC_CFG				0x000f0000	//vprc
#define HD_VIDEOPROC_CFG_STRIP_MASK		0x00000007  //vprc stripe rule mask: (default 0)
#define HD_VIDEOPROC_CFG_STRIP_LV1		0x00000000  //vprc "0: cut w>1280, GDC =  on, 2D_LUT off after cut (LL slow)
#define HD_VIDEOPROC_CFG_STRIP_LV2		0x00010000  //vprc "1: cut w>2048, GDC = off, 2D_LUT off after cut (LL fast)
#define HD_VIDEOPROC_CFG_STRIP_LV3		0x00020000  //vprc "2: cut w>2688, GDC = off, 2D_LUT off after cut (LL middle)(2D_LUT best)
#define HD_VIDEOPROC_CFG_STRIP_LV4		0x00030000  //vprc "3: cut w> 720, GDC =  on, 2D_LUT off after cut (LL not allow)(GDC best)
#define HD_VIDEOPROC_CFG_DISABLE_GDC	HD_VIDEOPROC_CFG_STRIP_LV2
#define HD_VIDEOPROC_CFG_LL_FAST		HD_VIDEOPROC_CFG_STRIP_LV2
#define HD_VIDEOPROC_CFG_2DLUT_BEST		HD_VIDEOPROC_CFG_STRIP_LV3
#define HD_VIDEOPROC_CFG_GDC_BEST		HD_VIDEOPROC_CFG_STRIP_LV4	

//novatek
#define MAX_VI_CHAN_PER_NVT		(8)
#define MAX_ENC_CHAN_PER_NVT	(MAX_VI_CHAN_PER_NVT * 2)         /*Novatek中包含最大编码通道(主子*vi_cnt )*/
#define MAX_DEC_CHAN_PER_NVT	(0)             /*Novatek中包含最大解码通道个数*/
#define MAX_VO_CHAN_PER_NVT	(0)             /*Novatek中包含最大显示通道个数*/
#define MAX_USB_CHAN_PER_NVT	(0)             /*定义USB摄像头通道个数*/






/*定义USB摄像头通道个数*/
#define MAX_USB_CHAN_SOC  MAX_USB_CHAN_PER_NVT

#define VO_MAX_AREA_NUM	(2)    /*最大为3不超过VID的数量*/

#define MAX_JPEG_CHAN_SOC       (MAX_VI_CHAN_PER_SOC)


#define MAX_TASK_NUM	(32)
#define MAX_SEM_NUM	(100)
#define MAX_EVENT_NUM	(50)
#define MAX_MUTEX_NUM	(50)


//demo config++++
#define VDO_SIZE_W     2560
#define VDO_SIZE_H     1920

#define CA_WIN_NUM_W		32
#define CA_WIN_NUM_H		32
#define LA_WIN_NUM_W		32
#define LA_WIN_NUM_H		32
#define CAP_OUT_FMT		HD_VIDEO_PXLFMT_RAW12
//demo config----

#define GET_ENC_CHAN_NUM       2

typedef enum
{
	CORE_0 = 0,
	CORE_1,
	CORE_2,
	CORE_3,
	CORE_4,
	CORE_5,
	CORE_6,
	CORE_7,
	CORE_8,
	CORE_9,
	MAX_CORE_NUM = 10
}MAX_SOC_CORE_E;

// HDAL内部参数
typedef struct
{
    UINT                    bootInfo[MAX_CORE_NUM];
    /*DSP 内部使用参数*/
    //CMD_SHARE_BUF           cmdShareBuf;        /*DSP->DSP 命令缓冲，DSP内部使用*/
   //DSP_MEM_SEC             dspMemSec[MAX_DSP_MEM_SEC_NUM];/*DSP共享缓存，DSP内部使用*/
    UINT                    dspMemSecCnt;
    UINT                    dspProcessId[2];
    UINT                    debugPin[27];
    BOOL                    algLoad;
    /*linux启动完成*/
    UINT32                  SysStart;	
}HDAL_INNER_PARAM_T;

#define HDAL_MUTEX_MAGIC 0x123456
typedef struct
{
	UINT32 magic;
   // UINT32 index;
	CHAR name[32];
	pthread_mutex_t mutex;
} HDAL_MUTEX_T;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */


HDAL_MUTEX_T* hdal_Mutex_Create(CHAR *name);
INT hdal_Mutex_lock(HDAL_MUTEX_T* phdalMutexHandle);
INT hdal_Mutex_unlock(HDAL_MUTEX_T* phdalMutexHandle);




#ifdef __cplusplus
}
#endif/*__cplusplus*/


#endif
