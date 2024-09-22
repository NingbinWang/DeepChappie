#include "hdal_capture.h"
#include "vendor_videocapture.h"

HDAL_VI_CTRL_T  gViCtrl;
static HDAL_VI_FRAME_QUEUE_INFO_T vi_qInfo[MAX_ENC_CHAN_SOC][MAX_VI_BIND_QUEUE_SIZE];

INT hdal_VI_StartChn(UINT32 chan)
{
	HD_RESULT ret = HD_OK;

	HDAL_VI_CHN_INFO_T   *pchanCfg = NULL;
   
    if(chan > (GET_ENC_CHAN_NUM))
    {
        printf("[%s:%d]hd_videocap_start failed stream_id %d is vaild max[%d]\n",__FUNCTION__, __LINE__,chan,gViCtrl.chanCnt);
        return -1;
    }
    
	
	pchanCfg = &gViCtrl.chanCfg[chan];
	
	if(pchanCfg->bEnable == FALSE)
	{
		printf("[%s:%d]This Chan %d is not enable !\n",__FUNCTION__, __LINE__,chan);
		return ret;
	}
	

	if(pchanCfg->bStart != TRUE)
	{
	    ret = hd_videocap_start(pchanCfg->PathIdS);
	    if (ret < 0)
	    {
			printf("[%s:%d]hd_videocap_start failed s32Ret = %d chan %d pstVcapHandle->u32PathIdS[i] 0x%x\n",__FUNCTION__, __LINE__,ret,chan,pchanCfg->PathIdS);
	        return -1;
	    }
	    pchanCfg->bStart = TRUE;

	}
	return ret;
}
INT hdal_VI_StopChn(UINT32 chan)
{
	HD_RESULT ret = HD_OK;

	HDAL_VI_CHN_INFO_T   *pchanCfg = NULL;
   
    if(chan > (GET_ENC_CHAN_NUM))
    {
    	printf("[%s:%d]hd_videocap_start failed stream_id %d is vaild max[%d]\n",__FUNCTION__, __LINE__,chan,gViCtrl.chanCnt);
        return -1;
    }
	
	pchanCfg = &gViCtrl.chanCfg[chan];
	
	if(pchanCfg->bEnable == FALSE)
	{
		printf("[%s:%d]This Chan %d is not enable !\n",__FUNCTION__, __LINE__,chan);
		return ret;
	}	
	

	if(pchanCfg->bStart != FALSE)
	{

	    ret = hd_videocap_stop(pchanCfg->PathIdS);
	    if (ret < 0)
	    {
	        printf("[%s:%d]hd_videocap_stop failed s32Ret = %d chan %d pstVcapHandle->u32PathIdS[i] 0x%x\n",__FUNCTION__, __LINE__,ret,chan,pchanCfg->PathIdS);
	        return -1;
	    }
		pchanCfg->bStart = FALSE;
	}
	

	return ret;
}

UINT32 hdal_VI_GetViInMode(UINT chan)
{
	UINT32 sen_mode =0;
	
	if(gViCtrl.ViCfgParam[chan].enWdr ==0)	
	{
		sen_mode = 0;
	}
	else if(gViCtrl.ViCfgParam[chan].enWdr ==1)	
	{
		sen_mode = 1;
	}
	return sen_mode;
}

HD_VIDEO_PXLFMT hdal_VI_GetViInPxl(UINT chan)
{
	HD_VIDEO_PXLFMT Pxl = 0;
	if(gViCtrl.ViCfgParam[chan].enWdr == 0)
	{
		if(gViCtrl.ViCfgParam[chan].vSensorType ==CMOS_OV_9284)
		{
			Pxl = HD_VIDEO_PXLFMT_RAW10;
		}
		if(gViCtrl.ViCfgParam[chan].vSensorType ==CMOS_OV_OS05A20)
		{
			Pxl = HD_VIDEO_PXLFMT_RAW12;
		}
		else
		{
			Pxl = HD_VIDEO_PXLFMT_YUV422;
		}	
	}
	else if(gViCtrl.ViCfgParam[chan].enWdr == 1)
	{
			Pxl = HD_VIDEO_PXLFMT_YUV422;
	}
    return Pxl;
}

HD_VIDEOCAP_SEN_FRAME_NUM hdal_VI_GetViFrmNum(UINT chan)
{
  	HD_VIDEOCAP_SEN_FRAME_NUM Frm_Num =0;
  	
  	if(gViCtrl.ViCfgParam[chan].enWdr == 0)
  	{
		Frm_Num = HD_VIDEOCAP_SEN_FRAME_NUM_1;

  	}
  	else if(gViCtrl.ViCfgParam[chan].enWdr == 1)
  	{
		Frm_Num = HD_VIDEOCAP_SEN_FRAME_NUM_2;
  	}
    return Frm_Num;
}

HD_VIDEO_PXLFMT hdal_VI_GetViOutPxl(UINT chan)
{
	HD_VIDEO_PXLFMT Pxl = 0;
	
	if(gViCtrl.ViCfgParam[chan].enWdr == 0)
	{
		if(gViCtrl.ViCfgParam[chan].vSensorType ==CMOS_OV_9284)
		{
			Pxl = HD_VIDEO_PXLFMT_RAW10;
		}	
		else if(gViCtrl.ViCfgParam[chan].vSensorType ==CMOS_OV_OS05A20)
		{
			Pxl = HD_VIDEO_PXLFMT_RAW12;
		}
		else
		{
			Pxl = HD_VIDEO_PXLFMT_YUV420;
		}	
	}
	else if(gViCtrl.ViCfgParam[chan].enWdr == 1)
	{
			Pxl = HD_VIDEO_PXLFMT_YUV420;
	}

  	
    return Pxl;
}

UINT32 hdal_VI_GetViOutDepth(UINT chan)
{
  	UINT32 Frm_Depth =0;
  	
  	if(gViCtrl.ViCfgParam[chan].enWdr == 0)
  	{
		Frm_Depth = 1;
  	}
  	else if(gViCtrl.ViCfgParam[chan].enWdr == 1)
  	{
  		Frm_Depth = 2;
  	}
    return Frm_Depth;
}

INT hdal_VI_CreateChn(UINT32 chan)
{
	HD_RESULT  ret;
	HD_VIDEOCAP_DRV_CONFIG cap_cfg ={0};
	HDAL_VI_CHN_INFO_T *pCtrl = &gViCtrl.chanCfg[chan];	 
	HD_VIDEOCAP_CTRL iq_ctl = {0};
	UINT32 ad_map = 0;
	HD_VIDEOCAP_IN video_in_param = {0};
	HD_VIDEOCAP_CROP video_crop_param = {0};
	HD_VIDEOCAP_OUT video_out_param = {0};	
	//VENDOR_VIDEOCAP_CCIR_INFO ccir_info ={0};
	//UINT32 mipi_data_lane = 2;
	//HD_VIDEOCAP_FUNC_CONFIG video_path_param = {0};

	if(pCtrl->bEnable == FALSE)
	{
        printf("[%s:%d]This Chan %d is not enable !\n",__FUNCTION__, __LINE__,chan);
		return HDAL_VI_ERR;
	}

	if(pCtrl->bOpen == TRUE)
	{
        printf("[%s:%d]This hd_videocap_open of Chan %d  has open  !\n",__FUNCTION__, __LINE__,chan);
		return HDAL_VI_ERR;
	}

	
	ret = hd_videocap_open(0, HD_VIDEOCAP_CTRL(pCtrl->DevId),&pCtrl->CtrlId);
	if(HD_OK != ret)
	{
		printf("[%s:%d]hd_videocap_open Err %d  !\n",__FUNCTION__, __LINE__,ret);
		return HDAL_VI_ERR;
	}	

	if(gViCtrl.ViCfgParam[chan].vSensorType == CMOS_OV_OS05A20)
	{
		UINT i;
		strcpy(cap_cfg.sen_cfg.sen_dev.driver_name, pCtrl->sensor_name);
		cap_cfg.sen_cfg.sen_dev.if_type = pCtrl->if_type;
		cap_cfg.sen_cfg.sen_dev.pin_cfg.pinmux.sensor_pinmux =  pCtrl->sen_init_pincfg.pinmux.sensor_pinmux;
		cap_cfg.sen_cfg.sen_dev.pin_cfg.pinmux.cmd_if_pinmux =  pCtrl->sen_init_pincfg.pinmux.cmd_if_pinmux;
		cap_cfg.sen_cfg.sen_dev.pin_cfg.pinmux.serial_if_pinmux = pCtrl->sen_init_pincfg.pinmux.serial_if_pinmux;
		cap_cfg.sen_cfg.sen_dev.pin_cfg.clk_lane_sel = pCtrl->sen_init_pincfg.clk_lane_sel;
		for (i=0; i < HD_VIDEOCAP_SEN_SER_MAX_DATALANE; i++) {
		    cap_cfg.sen_cfg.sen_dev.pin_cfg.sen_2_serial_pin_map[i] =  pCtrl->sen_init_pincfg.sen_2_serial_pin_map[i];
	    }

	}else if(gViCtrl.ViCfgParam[chan].vSensorType == CMOS_OV_9284 )
	{
		strcpy(cap_cfg.sen_cfg.sen_dev.driver_name, pCtrl->sensor_name);
		cap_cfg.sen_cfg.sen_dev.if_type = pCtrl->if_type;
		cap_cfg.sen_cfg.sen_dev.pin_cfg.pinmux.sensor_pinmux =  pCtrl->sen_init_pincfg.pinmux.sensor_pinmux;
		cap_cfg.sen_cfg.sen_dev.pin_cfg.pinmux.cmd_if_pinmux =  pCtrl->sen_init_pincfg.pinmux.cmd_if_pinmux;
		cap_cfg.sen_cfg.sen_dev.pin_cfg.ccir_msblsb_switch = pCtrl->sen_init_pincfg.ccir_msblsb_switch;
		cap_cfg.sen_cfg.sen_dev.pin_cfg.ccir_vd_hd_pin = pCtrl->sen_init_pincfg.ccir_vd_hd_pin;
	}
	else
	{
		printf("[%s:%d]error,no sensor type or board type!\n",__FUNCTION__, __LINE__);
		return HDAL_VI_ERR;
	}
	ret = hd_videocap_set(pCtrl->CtrlId, HD_VIDEOCAP_PARAM_DRV_CONFIG, &cap_cfg);	
	if (HD_OK != ret)
	{
	   printf("[%s:%d]hd_videocap_set failed Ret = %d\n",__FUNCTION__, __LINE__,ret);
	   return HDAL_VI_ERR;
	
	}

	if(gViCtrl.ViCfgParam[chan].vSensorType != CMOS_YUV422_TVI)
	{
		if(gViCtrl.ViCfgParam[chan].enWdr ==1)
		{
			
			cap_cfg.sen_cfg.shdr_map = HD_VIDEOCAP_SHDR_MAP(HD_VIDEOCAP_HDR_SENSOR1, (HD_VIDEOCAP_0|HD_VIDEOCAP_1));
			iq_ctl.func = HD_VIDEOCAP_FUNC_AE | HD_VIDEOCAP_FUNC_AWB | HD_VIDEOCAP_FUNC_SHDR;
			
		}
		else
		{
			cap_cfg.sen_cfg.shdr_map = 0;
			iq_ctl.func = HD_VIDEOCAP_FUNC_AE | HD_VIDEOCAP_FUNC_AWB;
		}
	}
	else
	{
		iq_ctl.func = 0;
	}
	ret = hd_videocap_set(pCtrl->CtrlId, HD_VIDEOCAP_PARAM_CTRL, &iq_ctl);
	if (HD_OK != ret)
	{
	   printf("[%s:%d]hd_videocap_set failed Ret = %d\n",__FUNCTION__, __LINE__,ret);
	   return HDAL_VI_ERR;
	
	}	
	
	if(gViCtrl.ViCfgParam[chan].vSensorType == CMOS_YUV422_TVI)
	{
		ad_map = VENDOR_VIDEOCAP_AD_MAP(0, 0, HD_VIDEOCAP_1);
		vendor_videocap_set(pCtrl->CtrlId, VENDOR_VIDEOCAP_PARAM_AD_MAP, &ad_map);
	    printf("[%s:%d]----------------ad_map[0x%x]	------------------\n",__FUNCTION__, __LINE__,ad_map);
	}

	/*open VI模块*/
	ret = hd_videocap_open(HD_VIDEOCAP_IN(pCtrl->DevId,0),HD_VIDEOCAP_OUT(pCtrl->DevId,0), &pCtrl->PathIdS);
	if(ret!=HD_OK)
	{
		printf("[%s:%d]hd_videocap_open %d Err !\n",__FUNCTION__, __LINE__,ret);
		return HDAL_VI_NOSUPPORT;
	}

	video_in_param.sen_mode = hdal_VI_GetViInMode(chan); // auto select sensor mode by the parameter of HD_VIDEOCAP_PARAM_OUT
	video_in_param.frc = HD_VIDEO_FRC_RATIO(gViCtrl.ViCfgParam[chan].frame_rate,1);
	video_in_param.dim.w = gViCtrl.ViCfgParam[chan].image_viW;
	video_in_param.dim.h = gViCtrl.ViCfgParam[chan].image_viH;
	video_in_param.pxlfmt = hdal_VI_GetViInPxl(chan);
	video_in_param.out_frame_num = hdal_VI_GetViFrmNum(chan);
	

	ret = hd_videocap_set(pCtrl->PathIdS, HD_VIDEOCAP_PARAM_IN, &video_in_param);
	if (HD_OK != ret)
   	{
	  printf("[%s:%d]HD_VIDEOCAP_PARAM_IN set failed s32Ret = 0x%x\n",__FUNCTION__, __LINE__,ret);
	   return HDAL_VI_ERR;
   	
  	}
  	
	video_crop_param.mode = HD_CROP_OFF;
	ret = hd_videocap_set(pCtrl->PathIdS, HD_VIDEOCAP_PARAM_IN_CROP, &video_crop_param);
	if (HD_OK != ret)
   	{
	   printf("[%s:%d]param_crop_set failed s32Ret = 0x%x\n",__FUNCTION__, __LINE__,ret);
	   return HDAL_VI_ERR;
  	}
	
	//without setting dim for no scaling, using original sensor out size
	video_out_param.pxlfmt = hdal_VI_GetViOutPxl(chan);
	video_out_param.dir = HD_VIDEO_DIR_NONE;/*镜像和旋转方式，default关闭*/
	video_out_param.depth = hdal_VI_GetViOutDepth(chan);/*额外设置buff给cappullout使用进行dump vi的YUV 数据*/
	if(gViCtrl.ViCfgParam[chan].viewMirror == VIEW_NATURAL)
	{
		video_out_param.dir = HD_VIDEO_DIR_NONE;
	}
	else if(gViCtrl.ViCfgParam[chan].viewMirror == VIEW_180FLIP)
	{
		video_out_param.dir = HD_VIDEO_DIR_MIRRORXY;
	}
	else if(gViCtrl.ViCfgParam[chan].viewMirror == VIEW_VERTICAL_MIRROR)
	{
		video_out_param.dir = HD_VIDEO_DIR_MIRRORX;
	}
	else if(gViCtrl.ViCfgParam[chan].viewMirror == VIEW_HORIZONTAL_MIRROR)
	{
		video_out_param.dir = HD_VIDEO_DIR_MIRRORY;
	}
	else
	{
		video_out_param.dir = HD_VIDEO_DIR_NONE;
	}
	
	ret = hd_videocap_set(pCtrl->PathIdS, HD_VIDEOCAP_PARAM_OUT, &video_out_param);
	if (HD_OK != ret)
   	{
	   printf("[%s:%d]param_out_set failed s32Ret = 0x%x\n",__FUNCTION__, __LINE__,ret);
	   return HDAL_VI_ERR;
   	
  	}
	pCtrl->bOpen = TRUE;
	
    return HDAL_VI_OK;
}



/*
typedef struct
{
    VI_VIEW_FLIP_E                        viewMirror;   // 镜像选择方式 默认为0
    UINT32                                image_viW; //前端原始图像有效宽
    UINT32                                image_viH; //前端原始图像有效高
    UINT32                                frame_rate; //前端帧率
    VI_SENSOR_TYPE_E                      vSensorType; //Sensor类型
    VIDEO_LENS_TYPE_E                     vLensType; //镜头类型
    VI_DATNIGHT_INFO_T                    dayNightInfo; //日夜模式   	0-day 1-night 2-自动模式 默认为2
    UINT8                                 enWdr;//宽动态开关 0:关闭 1:打开2：自动
    UINT8                                 enWdrLevel; //宽动态等级（只有在宽动态开启时生效）默认为50
    UINT8                                 uBrightnessLevel;//亮度0-100> 默认为50
    UINT8                                 uStaturationLevel;// 饱和度0-100 默认为50
    UINT8                                 uContrastLevel;//对比度0-100 默认为50
    UINT8                                 bEnableDCE;//畸变校正 默认为0
    UINT8                                 uSharpnessLevel;  //锐度0-100 默认为50
    UINT32                                uExposure;//曝光时间 [10,1000000] 微妙，默认为1/帧率
    UINT8                                 uIrisType;  //光圈类型0-DC 1-P     暂不支持此接口
    VI_WHITEBALANCE_INFO_T                uWhiteBalanceInfo; //白平衡模式 0-手动白平衡；1-自动白平衡1；3-锁定白平衡；6-日光灯；14-白炽灯；15-暖光灯；16-自然光 默认为1
    VI_NR_INFO_T                          uNRInfo; //数字降噪	  0-关闭  1-普通 2-专家 默认为1
    VI_FOG_INFO_T                         uFogInfo; //透雾模式 0 关 1 开启 2自动
    UINT8                                 uElesharkMode;//电子防抖开关
    VI_BLC_INFO_T                         uBLCInfo;//背光补偿 默认关
    UINT8                                 uHLCMode;//强光抑制 默认关
    UINT8                                 enHLCLevel; //强光抑制等级（只有在强光抑制开启时生效）默认为50    
    VI_COVER_INFO_T                       uCoverMode; //视频遮挡 默认关   
	UINT8                                 ulightover;//防补光过爆0关 1开 默认为0
	UINT8                                 ugrayscale;//灰度范围0[0-255] 1[16-235]默认为0
    UINT8                                 res[7];
} VI_CFG_PARAM_T;
*/

static INT hdal_VI_SetVIparamCfg(HDAL_PROFUCT_ID_E id,VI_CFG_PARAM_T *pviCfgParam)
{
     switch(id)
    {
        case HDAL_PROFUCT_ID_DEMO:
        {
            pviCfgParam[0].viewMirror = VIEW_NATURAL;
            pviCfgParam[0].image_viW = 2540;
            pviCfgParam[0].image_viH = 1920;
            break;
        }
        case HDAL_PROFUCT_ID_A30051:
        {
			pviCfgParam[0].viewMirror = VIEW_NATURAL;
            pviCfgParam[0].image_viW = 2540;
            pviCfgParam[0].image_viH = 1920;
			pviCfgParam[0].vSensorType = CMOS_OV_OS05A20;
			pviCfgParam[0].enWdr = 0;
			pviCfgParam[0].frame_rate = 25;

            break;
        }
        default:
           return HDAL_VI_NOSUPPORT;
    }
    return HDAL_VI_OK;
}

static INT hdal_VI_SetCfg(HDAL_PROFUCT_ID_E id,HDAL_VI_CTRL_T *pCtrl)
{
    switch(id)
    {
        case HDAL_PROFUCT_ID_A30051:
        {
			
            pCtrl->chanCnt = 1;
			pCtrl->chanCfg[0].bEnable = TRUE;   
			pCtrl->chanCfg[0].DevId = 0;
			pCtrl->chanCfg[0].bindVpsDevId = 0;
			pCtrl->chanCfg[0].PathIdS = 0;
			pCtrl->chanCfg[0].bOpen = FALSE;
			pCtrl->chanCfg[0].bStart = FALSE;
			pCtrl->chanCfg[0].sen_init_pincfg.pinmux.sensor_pinmux= 0x220;
			pCtrl->chanCfg[0].sen_init_pincfg.pinmux.cmd_if_pinmux= 0x00;
			pCtrl->chanCfg[0].sen_init_pincfg.pinmux.serial_if_pinmux= 0xf01;
			pCtrl->chanCfg[0].if_type = HD_COMMON_VIDEO_IN_MIPI_CSI;
			snprintf(pCtrl->chanCfg[0].sensor_name, HD_VIDEOCAP_SEN_NAME_LEN-1, "nvt_sen_os05a20");
			pCtrl->lanemode = HD_COMMON_VIDEO_IN_MIPI_CSI;
			pCtrl->chanCfg[0].sen_init_pincfg.clk_lane_sel = HD_VIDEOCAP_SEN_CLANE_SEL_CSI0_USE_C0;
			pCtrl->chanCfg[0].sen_init_pincfg.sen_2_serial_pin_map[0] = 0;
            pCtrl->chanCfg[0].sen_init_pincfg.sen_2_serial_pin_map[1] = 3;
            pCtrl->chanCfg[0].sen_init_pincfg.sen_2_serial_pin_map[2] = 2;
            pCtrl->chanCfg[0].sen_init_pincfg.sen_2_serial_pin_map[3] = 1;
			pCtrl->chanCfg[0].sen_init_pincfg.sen_2_serial_pin_map[4] = HD_VIDEOCAP_SEN_IGNORE;
			pCtrl->chanCfg[0].sen_init_pincfg.sen_2_serial_pin_map[5] = HD_VIDEOCAP_SEN_IGNORE;
			pCtrl->chanCfg[0].sen_init_pincfg.sen_2_serial_pin_map[6] = HD_VIDEOCAP_SEN_IGNORE;
			pCtrl->chanCfg[0].sen_init_pincfg.sen_2_serial_pin_map[7] = HD_VIDEOCAP_SEN_IGNORE;
            break;
        }
        case HDAL_PROFUCT_ID_B20066:
        {
            pCtrl->chanCnt = 1;
			pCtrl->chanCfg[0].bEnable = TRUE;   
			pCtrl->chanCfg[0].DevId = 0;
			pCtrl->chanCfg[0].bindVpsDevId = 0;
			pCtrl->chanCfg[0].PathIdS = 0;
			pCtrl->chanCfg[0].bOpen = FALSE;
			pCtrl->chanCfg[0].bStart = FALSE;
			pCtrl->lanemode = HD_COMMON_VIDEO_IN_MIPI_CSI;
            pCtrl->chanCfg[0].if_type = HD_COMMON_VIDEO_IN_P_AHD;
			pCtrl->chanCfg[0].sen_init_pincfg.pinmux.sensor_pinmux= 0x08;
			pCtrl->chanCfg[0].sen_init_pincfg.pinmux.cmd_if_pinmux= 0x01;
			snprintf(pCtrl->chanCfg[0].sensor_name, HD_VIDEOCAP_SEN_NAME_LEN-1, "nvt_sen_os05a20");
			pCtrl->chanCfg[0].sen_init_pincfg.ccir_msblsb_switch = FALSE;
			pCtrl->chanCfg[0].sen_init_pincfg.ccir_vd_hd_pin= TRUE;
            break;
        }
        default:
           return HDAL_VI_NOSUPPORT;
    }
    return HDAL_VI_OK;
}


INT hdal_VIsystem_Init(HDAL_PROFUCT_ID_E id)
{
    INT ret = -1;
	UINT i;
	CHAR   mtxName[64] = {0};
    VI_CFG_PARAM_T*  pviCfgParam = Media_Get_ViParam();
    memset((void*)&gViCtrl,0,sizeof(HDAL_VI_CTRL_T));
    memcpy(gViCtrl.ViCfgParam,pviCfgParam,sizeof(VI_CFG_PARAM_T)*MAX_ENC_CHAN_SOC);
    if(pviCfgParam->enable == 1){
        ret = hdal_VI_SetCfg(pviCfgParam->ctlid,&gViCtrl);
        if(HDAL_VI_OK != ret)
	    {
		        printf("[%s:%d]productid :%d hdal_VI_SetCfg fail =%d\n",__FUNCTION__, __LINE__,pviCfgParam->ctlid,ret);
                return  ret;
	    }
        id = HDAL_VI_NOSUPPORT;
    }else{
        ret = hdal_VI_SetCfg(id,&gViCtrl);
        if(HDAL_VI_OK != ret)
	    {
		        printf("[%s:%d]hdal_VI_SetCfg fail =%d\n",__FUNCTION__, __LINE__,ret);
                return  ret;
	    }
    }
    ret = hdal_VI_SetVIparamCfg(id,gViCtrl.ViCfgParam);
    if(HDAL_VI_OK != ret)
    {
		        printf("[%s:%d]hdal_VI_SetCfg fail =%d\n",__FUNCTION__, __LINE__,ret);
                return  ret;
	}
	for(i = 0;i < gViCtrl.chanCnt; i++)
	{
		snprintf(mtxName,64,"mtxViQ_%d",i);
		gViCtrl.chanCfg[i].mutex = hdal_Mutex_Create(mtxName);
		if(NULL == gViCtrl.chanCfg[i].mutex)
		{
		    printf("[%s:%d]Create %s Err !\n",__FUNCTION__, __LINE__,mtxName);
		   return -1;
		}
		ret = hdal_VI_CreateChn(i);
		if(HDAL_VI_OK != ret)
	    {
		    printf("[%s:%d]hdal_VI_CreateChn[%d] fail return %d\n",__FUNCTION__, __LINE__,i,ret);
	    }

	}
	return 0;

}

INT32 hdal_VI_GetChnHDFrame( UINT chan, HD_VIDEO_FRAME *pstFrameInfo, INT32 timeout)
{

	HD_RESULT  ret;
	HDAL_VI_CHN_INFO_T *pCtrl = &gViCtrl.chanCfg[chan];
	
	if(pCtrl->bStart != TRUE)
	{
		return HDAL_VI_ERR;
	}
	
	ret=hd_videocap_pull_out_buf(pCtrl->PathIdS,pstFrameInfo,timeout);
	if (HD_OK != ret)
	{
		return HDAL_VI_ERR;
	}
	return HDAL_VI_OK;
}
INT32 hdal_VI_ReleaseChnHDFrame(UINT chan, HD_VIDEO_FRAME *pstFrameInfo)
{

	HD_RESULT  ret;
	HDAL_VI_CHN_INFO_T *pCtrl = &gViCtrl.chanCfg[chan];
	
	if(pCtrl->bStart != TRUE)
	{
		return HDAL_VI_ERR;
	}
	if(pstFrameInfo == NULL)
	{
		return HDAL_VI_ERR;
	}
	ret=hd_videocap_release_out_buf(pCtrl->PathIdS,pstFrameInfo);
	if (HD_OK != ret)
	{
		printf("[%s:%d][chan%d]hd_videoproc_release_out_buf failed with %#x\n",__FUNCTION__, __LINE__,chan,ret);
		return HDAL_VI_ERR;
	}
	return HDAL_VI_OK;
}

INT32 hdal_VI_SwitchViewMirror(UINT32 chan)
{
	HD_RESULT  ret;
	HD_VIDEOCAP_OUT video_out_param = {0};
	HDAL_VI_CHN_INFO_T *pCtrl = &gViCtrl.chanCfg[chan];  

	ret = hd_videocap_get(pCtrl->PathIdS, HD_VIDEOCAP_PARAM_OUT, &video_out_param);
	if (HD_OK != ret)
	{
        printf("[%s:%d]HD_VIDEOCAP_PARAM_OUT set failed  Ret = 0x%x\n",__FUNCTION__, __LINE__,ret);
		return HDAL_VI_ERR;
	
	}
	if(gViCtrl.ViCfgParam[chan].viewMirror == VIEW_NATURAL)
	{
		video_out_param.dir = HD_VIDEO_DIR_NONE;
	}
	else if(gViCtrl.ViCfgParam[chan].viewMirror == VIEW_180FLIP)
	{
		video_out_param.dir = HD_VIDEO_DIR_MIRRORXY;
	}
	else if(gViCtrl.ViCfgParam[chan].viewMirror == VIEW_VERTICAL_MIRROR)
	{
		video_out_param.dir = HD_VIDEO_DIR_MIRRORX;
	}
	else if(gViCtrl.ViCfgParam[chan].viewMirror == VIEW_HORIZONTAL_MIRROR)
	{
		video_out_param.dir = HD_VIDEO_DIR_MIRRORY;
	}
	else
	{
		video_out_param.dir = HD_VIDEO_DIR_NONE;
	}	
	
	ret = hd_videocap_set(pCtrl->PathIdS, HD_VIDEOCAP_PARAM_OUT, &video_out_param);
	if (HD_OK != ret)
	{
        printf("[%s:%d]HD_VIDEOCAP_PARAM_OUT set failed  Ret = 0x%x\n",__FUNCTION__, __LINE__,ret);
		return HDAL_VI_ERR;
	
	}
	return HDAL_VI_OK;
}




INT32 hdal_VI_SwitchFps(UINT32 chan)
{
	HD_RESULT  ret;
	HD_VIDEOCAP_IN video_in_param = {0};
	HDAL_VI_CHN_INFO_T *pCtrl = &gViCtrl.chanCfg[chan];	 

	ret = hd_videocap_get(pCtrl->PathIdS, HD_VIDEOCAP_PARAM_IN, &video_in_param);
	if (HD_OK != ret)
	{
		printf("[%s:%d]HD_VIDEOCAP_PARAM_IN get failed Ret = 0x%x\n",__FUNCTION__, __LINE__,ret);
		return HDAL_VI_ERR;
	}
	
	video_in_param.frc = HD_VIDEO_FRC_RATIO(gViCtrl.ViCfgParam[chan].frame_rate,1);

	ret = hd_videocap_set(pCtrl->PathIdS, HD_VIDEOCAP_PARAM_IN, &video_in_param);
	if (HD_OK != ret)
   	{
        printf("[%s:%d]HD_VIDEOCAP_PARAM_IN set failed  Ret = 0x%x\n",__FUNCTION__, __LINE__,ret);
		return HDAL_VI_ERR;
  	}
    return HDAL_VI_OK;
}
#define PHY2VIRT_MAIN(pa) (vir_addr_video + ((pa) - phy_addr_video))
static UINT32 phy_addr_video, vir_addr_video;
INT32 hdal_VI_GetChnFrame(UINT chan, MEDIA_VIDEO_FRAME_T *pstFrameInfo, INT32 timeout)
{
	UINT i;    
	HD_RESULT	ret;
	HD_VIDEO_PXLFMT PxlFmt; 
	HDAL_VI_FRAME_QUEUE_INFO_T  *q;
	HDAL_VI_CHN_INFO_T *pCtrl = &gViCtrl.chanCfg[chan];
	
	
	UINT y_size = 0,uv_size = 0;
	
	if((NULL == pstFrameInfo) || (chan >= MAX_ENC_CHAN_SOC))
	{
		printf("[%s:%d]Invalid Param  pstFrameInfo=%p chan=%d\n",__FUNCTION__, __LINE__,pstFrameInfo,chan);
		return HDAL_VI_INVALID_PARAM;
	}
	
	if(TRUE != pCtrl->bEnable)
	{
		return HDAL_VI_ERR_HARDWARE_NOT_EXIST;
	}

	hdal_Mutex_lock(pCtrl->mutex);

	for(i = 0; i < MAX_VI_BIND_QUEUE_SIZE; i++)
	{
		q = &vi_qInfo[chan][i];
		if(0 == q->bUsed)
		{
			break;
		}
	}
	
	if(i >= MAX_VI_BIND_QUEUE_SIZE)
	{
		printf("[%s:%d]Queue is fulle !\n",__FUNCTION__, __LINE__);
		hdal_Mutex_unlock(pCtrl->mutex);
		return HDAL_VI_ERR_UNDEFINED;
	}
	ret= hd_videocap_pull_out_buf(pCtrl->PathIdS,&q->viFrameInfo, timeout);
	if (HD_OK != ret)
	{
		// 非阻塞获取帧时不打印，避免打印过多
		if(0 != timeout)
		{
			printf("[%s:%d]timeout PathIdS[0] [0x%x][chan%d]failed with %d\n",__FUNCTION__, __LINE__,pCtrl->PathIdS,chan,ret);
		}
		hdal_Mutex_unlock(pCtrl->mutex);
		return HDAL_VI_ERR;
	}

	phy_addr_video = hd_common_mem_blk2pa(q->viFrameInfo.blk); // Get physical addr
	if (phy_addr_video == 0) 
	{
		printf("[%s:%d]hd_common_mem_blk2pa error !!\r\n",__FUNCTION__, __LINE__);
		hdal_Mutex_unlock(pCtrl->mutex);
		return HDAL_VI_ERR;		
	}
	
	pstFrameInfo->video_header.image_width = q->viFrameInfo.dim.w;
	pstFrameInfo->video_header.image_height = q->viFrameInfo.dim.h;
	pstFrameInfo->video_header.pts = q->viFrameInfo.timestamp;
	
	if(HD_VIDEO_PXLFMT_YUV422 == q->viFrameInfo.pxlfmt)
	{
		PxlFmt = HD_VIDEO_PXLFMT_YUV422;
		pstFrameInfo->video_header.format = MEDIA_FORMAT_Y_PLANAR_VU_PACK_422;
		y_size = q->viFrameInfo.dim.h * q->viFrameInfo.loff[0];
		uv_size = y_size;		
	}
	else if(HD_VIDEO_PXLFMT_YUV420 == q->viFrameInfo.pxlfmt)
	{
		PxlFmt = HD_VIDEO_PXLFMT_YUV420;
		pstFrameInfo->video_header.format = MEDIA_FORMAT_Y_PLANAR_VU_PACK_420;
		y_size = q->viFrameInfo.dim.h * q->viFrameInfo.loff[0];
		uv_size = y_size / 2;			
	}
	else if((HD_VIDEO_PXLFMT_RAW12>>16) == (q->viFrameInfo.pxlfmt>>16)) //HD_VIDEO_PXLFMT_RAW12
	{
		PxlFmt = HD_VIDEO_PXLFMT_RAW12;
		pstFrameInfo->video_header.format = MEDIA_FORMAT_RAW12;
		y_size = q->viFrameInfo.dim.h * q->viFrameInfo.loff[0];
		uv_size = 0;			
	}
	else if((HD_VIDEO_PXLFMT_RAW10>>16) == (q->viFrameInfo.pxlfmt>>16)) //HD_VIDEO_PXLFMT_RAW12
	{
		PxlFmt = HD_VIDEO_PXLFMT_RAW10;
		pstFrameInfo->video_header.format = MEDIA_FORMAT_RAW10;
		y_size = q->viFrameInfo.dim.h * q->viFrameInfo.loff[0];
		uv_size = 0;
			
	}
	else if((HD_VIDEO_PXLFMT_RAW8>>16) == (q->viFrameInfo.pxlfmt>>16)) //HD_VIDEO_PXLFMT_RAW12
	{
		PxlFmt = HD_VIDEO_PXLFMT_RAW8;
		pstFrameInfo->video_header.format = MEDIA_FORMAT_RAW8;
		y_size = q->viFrameInfo.dim.h * q->viFrameInfo.loff[0];
		uv_size = 0;	
	}	
	else 
	{
		printf("[%s:%d]q->viFrameInfo.pxlfmt =0x%x !!\r\n",__FUNCTION__, __LINE__,q->viFrameInfo.pxlfmt);
		hdal_Mutex_unlock(pCtrl->mutex);
		return HDAL_VI_NOSUPPORT;
	}
	
	vir_addr_video = (UINT32)hd_common_mem_mmap(HD_COMMON_MEM_MEM_TYPE_CACHE, phy_addr_video,DBGINFO_BUFSIZE()+VDO_YUV_BUFSIZE(q->viFrameInfo.dim.w, q->viFrameInfo.dim.h, PxlFmt));
	if (vir_addr_video == 0) 
	{
		printf("[%s:%d]memory map error !!\r\n",__FUNCTION__, __LINE__);
		hdal_Mutex_unlock(pCtrl->mutex);
		return HDAL_VI_ERR;			
	}	
	
	//注意: YUV420 [0][1]分别为Y和UV的虚拟地址，[2][3]分别为Y和UV的物理地址
	pstFrameInfo->yuv_frame.virAddr[0] =(unsigned char *)PHY2VIRT_MAIN(q->viFrameInfo.phy_addr[0]);
	pstFrameInfo->yuv_frame.virAddr[1] =(unsigned char *)PHY2VIRT_MAIN(q->viFrameInfo.phy_addr[1]);
	
	pstFrameInfo->yuv_frame.phyAddr[0] = (unsigned char *)q->viFrameInfo.phy_addr[0];
	pstFrameInfo->yuv_frame.phyAddr[1] = (unsigned char *)q->viFrameInfo.phy_addr[1];
	
	pstFrameInfo->yuv_frame.stride[0] = q->viFrameInfo.loff[0];
	pstFrameInfo->yuv_frame.stride[1] = q->viFrameInfo.loff[1];
	

	printf(" y_size[%d] uv_size[%d]\n",y_size,uv_size);
	q->bUsed = 1;
	
	hdal_Mutex_unlock(pCtrl->mutex);
	
	return HDAL_VI_OK;
}



INT32 Hdal_VI_ReleaseChnFrame(UINT chan, MEDIA_VIDEO_FRAME_T *pstFrameInfo)
{
	UINT i;    
	HD_RESULT	ret;
	HD_VIDEO_PXLFMT PxlFmt; 
	HDAL_VI_FRAME_QUEUE_INFO_T  *q;
	HDAL_VI_CHN_INFO_T *pCtrl = &gViCtrl.chanCfg[chan];
	
	if((NULL == pstFrameInfo) || (chan >= MAX_ENC_CHAN_SOC))
	{
		printf("[%s:%d]Invalid Param  pstFrameInfo=%p chan=%d\n",__FUNCTION__, __LINE__,pstFrameInfo,chan);
		return HDAL_VI_INVALID_PARAM;
	}
	
	if(1 != pCtrl->bEnable)
	{
		return HDAL_VI_ERR_HARDWARE_NOT_EXIST;
	}
	hdal_Mutex_lock(pCtrl->mutex);
	
	for(i = 0; i < MAX_VI_BIND_QUEUE_SIZE; i++)
	{
		q = &vi_qInfo[chan][i];
		if((TRUE == q->bUsed) && (pstFrameInfo->video_header.pts == q->viFrameInfo.timestamp))
		{
			break;
		}
	}
	
	if(i >= MAX_VI_BIND_QUEUE_SIZE)
	{
		printf("[%s:%d]Queue is not bind !\n",__FUNCTION__, __LINE__);
		hdal_Mutex_unlock(pCtrl->mutex);
		return HDAL_VI_ERR_UNDEFINED;
	}
	
	if(HD_VIDEO_PXLFMT_YUV422 == q->viFrameInfo.pxlfmt)
	{
		PxlFmt = HD_VIDEO_PXLFMT_YUV422;	
	}
	else if(HD_VIDEO_PXLFMT_YUV420 == q->viFrameInfo.pxlfmt)
	{
		PxlFmt = HD_VIDEO_PXLFMT_YUV420;	
	}
	else if((HD_VIDEO_PXLFMT_RAW12>>16) == (q->viFrameInfo.pxlfmt>>16)) //HD_VIDEO_PXLFMT_RAW12
	{
		PxlFmt = HD_VIDEO_PXLFMT_RAW12;		
	}
	else if((HD_VIDEO_PXLFMT_RAW10>>16) == (q->viFrameInfo.pxlfmt>>16))
	{
		PxlFmt = HD_VIDEO_PXLFMT_RAW10;
			
	}
	else if((HD_VIDEO_PXLFMT_RAW8>>16) == (q->viFrameInfo.pxlfmt>>16))
	{
		PxlFmt = HD_VIDEO_PXLFMT_RAW8;	
	}	
	else 
	{
		
		printf("[%s:%d]q->viFrameInfo.pxlfmt =0x%x\n",__FUNCTION__, __LINE__,q->viFrameInfo.pxlfmt);
		hdal_Mutex_unlock(pCtrl->mutex);
		return HDAL_VI_NOSUPPORT;
	}


	ret = hd_common_mem_munmap((void *)vir_addr_video,DBGINFO_BUFSIZE()+VDO_YUV_BUFSIZE(q->viFrameInfo.dim.w, q->viFrameInfo.dim.h, PxlFmt));
    if (HD_OK != ret)
	{
		printf("[%s:%d]hd_common_mem_munmap failed with %d\n",__FUNCTION__, __LINE__,ret);	

	}		
	ret = hd_videocap_release_out_buf(pCtrl->PathIdS,&q->viFrameInfo);
	 if (HD_OK != ret)
	{
		printf("[%s:%d]HI_MPI_VI_ReleaseChnFrame failed with %d\n",__FUNCTION__, __LINE__,ret);		
		hdal_Mutex_unlock(pCtrl->mutex);
		return HDAL_VI_ERR;
	}
	
	q->bUsed = 0;
	q->viFrameInfo.timestamp = 0;
	
	hdal_Mutex_unlock(pCtrl->mutex);
	
	return HDAL_VI_OK;
}
