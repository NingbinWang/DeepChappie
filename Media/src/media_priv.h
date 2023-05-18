#ifndef _MEDIA_INIT_PRIV_H_
#define _MEDIA_INIT_PRIV_H_

#include "media_type.h"
#include "Media.h"



//视频输入配置参数
typedef enum
{
    VIEW_NATURAL = 0,              //不镜像
    VIEW_90FLIP,                   //90度旋转
    VIEW_180FLIP,                  //180度旋转或中心镜像
    VIEW_270FLIP,                  //270度旋转
    VIEW_VERTICAL_MIRROR,          //垂直镜像，即图像左右翻转
    VIEW_HORIZONTAL_MIRROR,        //水平镜像，即图像上下翻转
    VIEW_90FLIP_VERTICAL_MIRROR,   //90度旋转加垂直镜像
    VIEW_90FLIP_HORIZONTAL_MIRROR, //90度旋转加水平镜像
    VIEW_FLIP_MAX,
} VI_VIEW_FLIP_E;

typedef enum
{
    CMOS_SONY_IMX322 = 0,/*sony  Sensor类型*/
	CMOS_SONY_IMX185,
	CMOS_SONY_IMX178,
	CMOS_SONY_IMX415,
    CMOS_APTINA_AR0230 = 100, /*aptina  Sensor类型*/
	CMOS_APTINA_AR0330,
	CMOS_APTINA_AR0237,
	CMOS_APTINA_AR0238,
    CMOS_APTINA_AR0140,
    CMOS_APTINA_AR0132, 
    CMOS_OV_4689 = 200, /*ov Sensor类型*/
	CMOS_OV_5658,
    CMOS_OV_OS05A2,
	CMOS_OV_2775,
	CMOS_OV_9281,
    CMOS_PANASONIC_MN34120 = 300, /*panasonic Sensor类型*/
	CMOS_PANASONIC_MN34220,
	CMOS_PANASONIC_MN34425,
    CMOS_JX_F22 = 400,
	CMOS_SC_500AI = 500,
	CMOS_SC_8238 = 501,
	CMOS_YUV422_TVI = 1000,
	VI_SENSOR_TYPE_MIN = INT_MINI,
	VI_SENSOR_TYPE_MAX = INT_MAXI
}VI_SENSOR_TYPE_E;

typedef enum
{
    LENS_STF28S2F 		 = 0x00, 
	LENS_SYC70757A		 = 0x01, 
    LENS_STF02V2F        = 0x02, 
	LENS_STF04S2F		 = 0x03,
	LENS_SEF08S2F		 = 0x04,
	LENS_STF28S2F_5M 	 = 0x05,
	LENS_TYPE_MIN 		= INT_MINI,
	LENS_TYPE_MAX 		= INT_MAXI
} VIDEO_LENS_TYPE_E;


typedef struct
{
	UINT32 dayNightMode; //白天黑夜模式切换 0:白天 1:夜晚 2：自动
	UINT8 sensitive; //灵敏度0-7
	UINT8 filtime;	//过滤时间0-120
} VI_DATNIGHT_INFO_T;
typedef struct
{
	UINT8 NRMode; //降噪开关 0:关闭 1:普通 2:专家
	UINT8 NRLeve; //降噪等级（普通降噪等级）0-100
	UINT8 SpaNRLeve; //降噪等级（空域降噪等级）0-100
	UINT8 TemNRLeve; //降噪等级（时域降噪等级）0-100
} VI_NR_INFO_T;
typedef struct
{
	UINT8 uWhiteBalanceMode; //白平衡模式 0:手动 1:自动白平衡1 3:锁定白平衡 6:日光灯14：白炽灯15：暖光灯16：自然光
	UINT8 R_GAIN; //白平衡等级（手动模式生效）0-100
	UINT8 B_GAIN;
} VI_WHITEBALANCE_INFO_T;
typedef struct
{
	UINT8 FogMode; //透雾模式
    UINT8 FogLeavel; //透雾等级
} VI_FOG_INFO_T;

typedef struct
{
    unsigned int x; //图像中心水平坐标
    unsigned int y; //图像中心垂直坐标
    unsigned int w; //图像窗口宽最大不能超过VIDEO_PARAM_CFG结构体中image_viW, 最小64
    unsigned int h; //图像窗口高最大不能超过VIDEO_PARAM_CFG结构体中image_viH, 最小32
} VI_RECT_T;

typedef struct
{
	unsigned int idx;//区域id
    unsigned int x; //图像中心水平坐标
    unsigned int y; //图像中心垂直坐标
    unsigned int w; //图像窗口宽最大不能超过VIDEO_PARAM_CFG结构体中image_viW, 最小64
    unsigned int h; //图像窗口高最大不能超过VIDEO_PARAM_CFG结构体中image_viH, 最小32
} VI_Cover_T;

typedef struct
{
	UINT8 BLCMode;//背光补偿模式 0:关闭 1:上 2:下 3：左，4：右5：中6：自定义
    VI_RECT_T	blc_window_rec; //补偿区域（自定义模式生效）
} VI_BLC_INFO_T;
typedef struct
{
	/**< 遮挡块数 */
	UINT8 CoverNum;
	/**< 遮挡区域（自定义模式生效） */
    VI_Cover_T	cover_window_rec[4];
} VI_COVER_INFO_T;

typedef struct
{
    VI_VIEW_FLIP_E viewMirror;   // 镜像选择方式 默认为0
    UINT32 image_viW; //前端原始图像有效宽
    UINT32 image_viH; //前端原始图像有效高
    UINT32 frame_rate; //前端帧率
    VI_SENSOR_TYPE_E vSensorType; //Sensor类型
    VIDEO_LENS_TYPE_E vLensType; //镜头类型
    VI_DATNIGHT_INFO_T dayNightInfo; //日夜模式   	0-day 1-night 2-自动模式 默认为2
    UINT8 enWdr;//宽动态开关 0:关闭 1:打开2：自动
    UINT8 enWdrLevel; //宽动态等级（只有在宽动态开启时生效）默认为50
    UINT8 uBrightnessLevel;//亮度0-100> 默认为50
    UINT8 uStaturationLevel;// 饱和度0-100 默认为50
    UINT8 uContrastLevel;//对比度0-100 默认为50
    UINT8 bEnableDCE;//畸变校正 默认为0
    UINT8 uSharpnessLevel;  //锐度0-100 默认为50
    UINT32 uExposure;//曝光时间 [10,1000000] 微妙，默认为1/帧率
    UINT8 uIrisType;  //光圈类型0-DC 1-P     暂不支持此接口
    VI_WHITEBALANCE_INFO_T uWhiteBalanceInfo; //白平衡模式 0-手动白平衡；1-自动白平衡1；3-锁定白平衡；6-日光灯；14-白炽灯；15-暖光灯；16-自然光 默认为1
    VI_NR_INFO_T uNRInfo; //数字降噪	  0-关闭  1-普通 2-专家 默认为1
    VI_FOG_INFO_T uFogInfo; //透雾模式 0 关 1 开启 2自动
    UINT8 uElesharkMode;//电子防抖开关-暂不支持
    VI_BLC_INFO_T uBLCInfo;//背光补偿 默认关
    UINT8 uHLCMode;//强光抑制 默认关
    UINT8 enHLCLevel; //强光抑制等级（只有在强光抑制开启时生效）默认为50    
    VI_COVER_INFO_T uCoverMode; //视频遮挡 默认关   
	UINT8 ulightover;//防补光过爆0关 1开 默认为0
	UINT8 ugrayscale;//灰度范围0[0-255] 1[16-235]默认为0
    UINT8 res[7];
} VI_CFG_PARAM_T;

/***************************** ROI设置参数**********************************/
#define MAX_ROI_RGN_CNT (10)             /*ROI区域最大一共8个*/

#define ROI_MODE_NULL	(0u)             /*ROI关闭*/
#define ROI_MODE_STATIC	(1u << 0)        /*静态区域,类似视频遮盖*/
#define ROI_MODE_PLATE	(1u << 1)        /*车牌区域 暂时不支持*/
#define ROI_WINDOW_PERSENT_MAX           (1000)
typedef struct
{
	BOOL bRegionEnable;			/*该region使能开关*/
    UINT32 level;                 /* 等级参数*/
    UINT32 param;                 /* 预留参数*/
    INT32  deltaQp;             /* 相对编码质量 （正负31以内）绝对质量配置对用户不友好 */
    UINT16  x;                   /* 坐标及宽高，以1000*1000为全屏大小*/
    UINT16  y;
    UINT16  w;
    UINT16  h;
	UINT8 res[4];
} CFG_REGION_T;

typedef struct
{
    UINT32 mode;                  /* 关联模式，按比特位定义，考虑到以后有可能关联多种源*/
	/*静态区域ROI配置*/
	UINT32 regionCnt;                     /*静态区域个数*/
    CFG_REGION_T region[MAX_ROI_RGN_CNT]; /*静态区域配置*/
} ROI_CONFIG_T;

/*DSP编码通道运行状态*/
typedef struct
{
    volatile UINT32   viFps;          /*采集帧率*/
    volatile UINT32   osdStatus;      /*osd是否叠加*/
    volatile UINT32   viFrm;          /*采集帧数*/
    volatile UINT32   viLostFrm;      /*采集丢帧数目累加*/
    volatile UINT32   viW;            /*采集的宽高*/
    volatile UINT32   viH;            /*采集的宽高*/
    volatile UINT32   encFps;         /*编码帧率*/
    volatile UINT32   encFrm;         /*已编码帧数*/
    volatile UINT32   encLostFrm;     /*编码丢帧数目累加*/
    volatile UINT32   encBitRate;     /*编码比特率*/
    volatile UINT32   encW;           /*编码宽高*/
    volatile UINT32   encH;           /*编码宽高*/
    volatile UINT32   audioFrm;       /*音频已编码帧数*/
    volatile UINT32   audioLostFrm;   /*音频编码丢帧数目累加*/
    volatile UINT32   enable;         /*通道是否使能*/
    volatile UINT32   stremType;      /*视频流类型*/
    volatile UINT32   bHaveSingal;      /*是否有信号*/
    volatile UINT32   RecPoolFrmLost;   /*视频录像丢帧数目累加*/
    volatile UINT32   NetPoolFrmLost;   /*网传视频录像丢帧数目累加*/
    volatile UINT32   PsNetPoolFrmLost; /* PS网传视频录像丢帧数目累加*/
    volatile UINT32   res[5];
} ENC_STATUS;

/**@struct    MEDIA_PRIV_DATA_T
 * @brief     私有数据结构
 */
typedef struct
{
    BOOL  bExist;
    UINT8 uRes[3];
    INT32 iFd;            /* 操作的文件描述符 */
}MEDIA_PRIV_DATA_T;


/**@struct    DEV_MEDIA_BASE_T
 * @brief     MEDIA基础数据结构
 */
typedef struct
{
    IMedia            stInterface; /* media接口 */
    MEDIA_PRIV_DATA_T   stPrivData;  /* 私有数据 */
}MEDIA_BASE_T;



#endif