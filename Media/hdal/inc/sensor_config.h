#ifndef _SENSOR_CONFIG_H_
#define _SENSOR_CONFIG_H_

#include "hdal.h"

#define VDO_SIZE_W     2560
#define VDO_SIZE_H     1920

#define CA_WIN_NUM_W		32
#define CA_WIN_NUM_H		32
#define LA_WIN_NUM_W		32
#define LA_WIN_NUM_H		32

#define SEN_OUT_FMT		HD_VIDEO_PXLFMT_RAW12
#define CAP_OUT_FMT		HD_VIDEO_PXLFMT_RAW12


#define CAP_OUT_Q_DEPTH  1

#define RAW_BUFFER_DDR_ID DDR_ID0

#define SENSOR_NAME "nvt_sen_os05a20"


typedef struct {
	// (1)
	HD_VIDEOCAP_SYSCAPS cap_syscaps;
	HD_PATH_ID cap_ctrl;
	HD_PATH_ID cap_path;

	HD_DIM  cap_dim;
	CHAR cap_drv_name[HD_VIDEOCAP_SEN_NAME_LEN];
	UINT32 cap_pin_map[HD_VIDEOCAP_SEN_SER_MAX_DATALANE];
	UINT32 cap_shdr_map;
	HD_VIDEOCAP_SEN_FRAME_NUM cap_frame_num;
	HD_DIM  proc_max_dim;
	// (4) user pull
	pthread_t  cap_thread_id;
	UINT32     cap_exit;
	UINT32     cap_snap;
	UINT32     flow_start;
	UINT32     blk_size;
	INT32    wait_ms;
} VIDEO_LIVEVIEW_T;


#endif