#include "hdal_priv.h"

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



static INT sensor_reset_pin(char* gpionum)
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
             return FAIL;
       }
 
         // 写入需要导出的gpio
         len = strlen(gpionum);
         if (len != write(fd, gpionum, len)) 
         {
             printf("[%s:%d]write error",__FUNCTION__, __LINE__);
             close(fd);
             return FAIL;
         }
 
         // 导出成功，关闭文件
         close(fd); 
     }
 
     // 配置为输出模式
     if (gpio_config("direction", "out"))
     {
        return FAIL;
     }
    // 极性设置 
     if (gpio_config("active_low", "0"))
     {
         return FAIL;
     }
     // 控制 GPIO 输出高低电平 
     if (gpio_config("value", "1"))
     {
         return FAIL;
     }
     usleep(3000);
    if (gpio_config("value", "0"))
     {
         return FAIL;
     }
    usleep(3000);
    if (gpio_config("value", "1"))
     {
         return FAIL;
     }
     return OK;

}



static HD_RESULT set_cap_cfg(HD_PATH_ID *p_video_cap_ctrl, HD_OUT_ID _out_id, UINT32 *p_cap_pin_map, UINT32 cap_shdr_map, char *p_driver_name)
{
	HD_RESULT ret = HD_OK;
	HD_VIDEOCAP_DRV_CONFIG cap_cfg = {0};
	HD_PATH_ID video_cap_ctrl = 0;
	HD_VIDEOCAP_CTRL iq_ctl = {0};
	UINT32 i;
	strncpy(cap_cfg.sen_cfg.sen_dev.driver_name, p_driver_name, (HD_VIDEOCAP_SEN_NAME_LEN - 1));
	cap_cfg.sen_cfg.sen_dev.if_type = HD_COMMON_VIDEO_IN_MIPI_CSI;
	cap_cfg.sen_cfg.sen_dev.pin_cfg.pinmux.sensor_pinmux =  0x220; //PIN_SENSOR_CFG_MIPI | PIN_SENSOR_CFG_MCLK
	cap_cfg.sen_cfg.sen_dev.pin_cfg.pinmux.serial_if_pinmux = 0xf01;//PIN_MIPI_LVDS_CFG_CLK2 | PIN_MIPI_LVDS_CFG_DAT0|PIN_MIPI_LVDS_CFG_DAT1 | PIN_MIPI_LVDS_CFG_DAT2 | PIN_MIPI_LVDS_CFG_DAT3
	cap_cfg.sen_cfg.sen_dev.pin_cfg.pinmux.cmd_if_pinmux = 0x00;
	cap_cfg.sen_cfg.sen_dev.pin_cfg.clk_lane_sel = HD_VIDEOCAP_SEN_CLANE_SEL_CSI0_USE_C0;
	for (i=0; i < HD_VIDEOCAP_SEN_SER_MAX_DATALANE; i++) {
		cap_cfg.sen_cfg.sen_dev.pin_cfg.sen_2_serial_pin_map[i] = *(p_cap_pin_map+i);
	}
	cap_cfg.sen_cfg.shdr_map = cap_shdr_map;

	ret = hd_videocap_open(0, _out_id, &video_cap_ctrl); //open this for device control
	if (ret != HD_OK) {
		return ret;
	}
	ret |= hd_videocap_set(video_cap_ctrl, HD_VIDEOCAP_PARAM_DRV_CONFIG, &cap_cfg);
	iq_ctl.func = HD_VIDEOCAP_FUNC_AE | HD_VIDEOCAP_FUNC_AWB;
	if (cap_shdr_map) {
		iq_ctl.func |= HD_VIDEOCAP_FUNC_SHDR;
	}
	ret |= hd_videocap_set(video_cap_ctrl, HD_VIDEOCAP_PARAM_CTRL, &iq_ctl);
	*p_video_cap_ctrl = video_cap_ctrl;
	return ret;
}

static HD_RESULT set_cap_param(HD_PATH_ID video_cap_path, HD_DIM *p_dim, HD_VIDEOCAP_SEN_FRAME_NUM frame_num)
{
	HD_RESULT ret = HD_OK;
	{//select sensor mode, manually or automatically
		HD_VIDEOCAP_IN video_in_param = {0};
		video_in_param.sen_mode = HD_VIDEOCAP_SEN_MODE_AUTO; //auto select sensor mode by the parameter of HD_VIDEOCAP_PARAM_OUT
		video_in_param.frc = HD_VIDEO_FRC_RATIO(25,1);
		video_in_param.dim.w = p_dim->w;
		video_in_param.dim.h = p_dim->h;
		video_in_param.pxlfmt = SEN_OUT_FMT;
		video_in_param.out_frame_num = frame_num;

		ret = hd_videocap_set(video_cap_path, HD_VIDEOCAP_PARAM_IN, &video_in_param);
		//printf("set_cap_param MODE=%d\r\n", ret);
		if (ret != HD_OK) {
			return ret;
		}
	}
	#if 1 //no crop, full frame
	{
		HD_VIDEOCAP_CROP video_crop_param = {0};

		video_crop_param.mode = HD_CROP_OFF;
		ret = hd_videocap_set(video_cap_path, HD_VIDEOCAP_PARAM_IN_CROP, &video_crop_param);
		//printf("set_cap_param CROP NONE=%d\r\n", ret);
		if (ret != HD_OK) {
			return ret;
		}
	}
	#else //HD_CROP_ON
	{
		HD_VIDEOCAP_CROP video_crop_param = {0};

		video_crop_param.mode = HD_CROP_ON;
		video_crop_param.win.rect.x = 0;
		video_crop_param.win.rect.y = 0;
		video_crop_param.win.rect.w = 1920/2;
		video_crop_param.win.rect.h= 1080/2;
		video_crop_param.align.w = 4;
		video_crop_param.align.h = 4;
		ret = hd_videocap_set(video_cap_path, HD_VIDEOCAP_PARAM_IN_CROP, &video_crop_param);
		//printf("set_cap_param CROP ON=%d\r\n", ret);
	}
	#endif
	{
		HD_VIDEOCAP_OUT video_out_param = {0};

		//without setting dim for no scaling, using original sensor out size
		video_out_param.pxlfmt = CAP_OUT_FMT;
		if (frame_num > 1) {
			//just convert RAW to SHDR RAW format
			//e.g. HD_VIDEO_PXLFMT_RAW8 -> HD_VIDEO_PXLFMT_RAW8_SHDR2
			video_out_param.pxlfmt &=~ 0x0F000000;
			video_out_param.pxlfmt |= (frame_num << 24);
		}
		video_out_param.dir = HD_VIDEO_DIR_NONE;
		video_out_param.depth = CAP_OUT_Q_DEPTH*frame_num;
		ret = hd_videocap_set(video_cap_path, HD_VIDEOCAP_PARAM_OUT, &video_out_param);
		//printf("set_cap_param OUT=%d\r\n", ret);
	}

	{
		HD_VIDEOCAP_FUNC_CONFIG video_path_param = {0};

		video_path_param.ddr_id = RAW_BUFFER_DDR_ID;
		ret = hd_videocap_set(video_cap_path, HD_VIDEOCAP_PARAM_FUNC_CONFIG, &video_path_param);
		//printf("set_cap_param PATH_CONFIG=0x%X\r\n", ret);
	}
	return ret;
}

static HD_RESULT hdal_mem_init(UINT32 sen_hdr, UINT32 *p_blk_size)
{
	HD_RESULT              ret;
	HD_COMMON_MEM_INIT_CONFIG mem_cfg = {0};

	// config common pool (cap)
	mem_cfg.pool_info[0].type = HD_COMMON_MEM_COMMON_POOL;
	mem_cfg.pool_info[0].blk_size = DBGINFO_BUFSIZE()+VDO_RAW_BUFSIZE(VDO_SIZE_W, VDO_SIZE_H,CAP_OUT_FMT)
														+VDO_CA_BUF_SIZE(CA_WIN_NUM_W, CA_WIN_NUM_H)
														+VDO_LA_BUF_SIZE(LA_WIN_NUM_W, LA_WIN_NUM_H);
	mem_cfg.pool_info[0].blk_cnt = 2 + CAP_OUT_Q_DEPTH;
	if (sen_hdr) {
		mem_cfg.pool_info[0].blk_cnt *= 2;
	}
	mem_cfg.pool_info[0].ddr_id = RAW_BUFFER_DDR_ID;
	#if 0
	// config common pool (main)
	mem_cfg.pool_info[1].type = HD_COMMON_MEM_COMMON_POOL;
	mem_cfg.pool_info[1].blk_size = DBGINFO_BUFSIZE()+VDO_YUV_BUFSIZE(VDO_SIZE_W, VDO_SIZE_H, HD_VIDEO_PXLFMT_YUV420);
	mem_cfg.pool_info[1].blk_cnt = 3;
	mem_cfg.pool_info[1].ddr_id = DDR_ID0;
	#endif
	ret = hd_common_mem_init(&mem_cfg);
	*p_blk_size = mem_cfg.pool_info[0].blk_size;
	return ret;
}



static HD_RESULT init_module(void)
{
	HD_RESULT ret;
	if ((ret = hd_videocap_init()) != HD_OK)
		return ret;
	return HD_OK;
}

static HD_RESULT open_module(VIDEO_LIVEVIEW_T *p_stream)
{
	HD_RESULT ret;
	// set videocap config
	ret = set_cap_cfg(&p_stream->cap_ctrl, HD_VIDEOCAP_0_CTRL, p_stream->cap_pin_map, p_stream->cap_shdr_map, p_stream->cap_drv_name);
	if (ret != HD_OK) {
		printf("set cap-cfg fail=%d\n", ret);
		return HD_ERR_NG;
	}
	if ((ret = hd_videocap_open(HD_VIDEOCAP_0_IN_0, HD_VIDEOCAP_0_OUT_0, &p_stream->cap_path)) != HD_OK)
		return ret;

	return HD_OK;
}

static HD_RESULT close_module(VIDEO_LIVEVIEW_T *p_stream)
{
	HD_RESULT ret;

	if ((ret = hd_videocap_close(p_stream->cap_path)) != HD_OK)
		return ret;
	return HD_OK;
}

static HD_RESULT exit_module(void)
{
	HD_RESULT ret;

	if ((ret = hd_videocap_uninit()) != HD_OK)
		return ret;
	return HD_OK;
}

static HD_RESULT hdal_mem_exit(void)
{
	HD_RESULT ret = HD_OK;
	hd_common_mem_uninit();
	return ret;
}

VIDEO_LIVEVIEW_T stream[2] = {0}; //0: shdr main stream, 1: shdr sub stream
//hdal init

INT hdal_init(unsigned long sen_hdr)
{
    HD_RESULT ret;
    int i;
  
    sensor_reset_pin("69");
    // init hdal
    ret = hd_common_init(0);
	if (ret != HD_OK) {
        printf("[%s:%d]common fail=%d\n",__FUNCTION__, __LINE__,ret);
		return FAIL;
	}
    // init memory
	ret = hdal_mem_init(sen_hdr, &stream[0].blk_size);
	if (ret != HD_OK) {
         printf("[%s:%d]mem fail=%d\n",__FUNCTION__, __LINE__,ret);
		return FAIL;
	}
   	// init all modules
	ret = init_module();
	if (ret != HD_OK) {
         printf("[%s:%d]init_module fail=%d\n",__FUNCTION__, __LINE__,ret);
		return FAIL;
	}

    stream[0].wait_ms = 0;
    
	// open video liview modules (main)
	for (i = 0; i < HD_VIDEOCAP_SEN_SER_MAX_DATALANE; i++) {
		stream[0].cap_pin_map[i] = HD_VIDEOCAP_SEN_IGNORE;
	}
    stream[0].cap_pin_map[0] = 0;
    stream[0].cap_pin_map[1] = 3;
    stream[0].cap_pin_map[2] = 2;
    stream[0].cap_pin_map[3] = 1;
    snprintf(stream[0].cap_drv_name, HD_VIDEOCAP_SEN_NAME_LEN-1, SENSOR_NAME);
    if (sen_hdr) {
		stream[0].cap_shdr_map = HD_VIDEOCAP_SHDR_MAP(HD_VIDEOCAP_HDR_SENSOR1, (HD_VIDEOCAP_0|HD_VIDEOCAP_1));
	} else {
		stream[0].cap_shdr_map = 0;
	}
    ret = open_module(&stream[0]);
	if (ret != HD_OK) {
        printf("[%s:%d]open_module=%d\n",__FUNCTION__, __LINE__,ret);
		return FAIL;
	}
    // set videocap parameter (main)
	stream[0].cap_dim.w = VDO_SIZE_W; //assign by user
	stream[0].cap_dim.h = VDO_SIZE_H; //assign by user
	if (sen_hdr) {
		stream[0].cap_frame_num = HD_VIDEOCAP_SEN_FRAME_NUM_2;
	} else {
		stream[0].cap_frame_num = HD_VIDEOCAP_SEN_FRAME_NUM_1;
	}
    ret = set_cap_param(stream[0].cap_path, &stream[0].cap_dim, stream[0].cap_frame_num);
	if (ret != HD_OK) {
        printf("[%s:%d]set cap fail=%d\n",__FUNCTION__, __LINE__,ret);
		return FAIL;
	}
    // start capture modules (main)
	ret = hd_videocap_start(stream[0].cap_path);
	if (ret != HD_OK) {
        printf("[%s:%d]start fail=%d\n",__FUNCTION__, __LINE__,ret);
		return FAIL;
	}
	// let cap_raw_thread start to work
	stream[0].flow_start = 1;
    return OK;
}


INT hdal_exit(void)
{
    HD_RESULT ret;
    // stop capture modules (main)
	hd_videocap_stop(stream[0].cap_path);
    	// close capture modules (main)
	ret = close_module(&stream[0]);
	if (ret != HD_OK) {
        printf("[%s:%d]close fail=%d\n",__FUNCTION__, __LINE__,ret);
	}

	// uninit all modules
	ret = exit_module();
	if (ret != HD_OK) {
        printf("[%s:%d]exit fail=%d\n",__FUNCTION__, __LINE__,ret);
	}

	// uninit memory
	ret = hdal_mem_exit();
	if (ret != HD_OK) {
        printf("[%s:%d]mem fail=%d\n",__FUNCTION__, __LINE__,ret);
	}

	// uninit hdal
	ret = hd_common_uninit();
	if (ret != HD_OK) {
        printf("[%s:%d]common fail=%d\n",__FUNCTION__, __LINE__,ret);
	}
    return OK;
}