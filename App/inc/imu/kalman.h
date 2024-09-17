#ifndef _KALMAN_H_
#define _KALMAN_H_

/**
* @struct KALMAN_FILTER_T
* @brief 卡尔曼滤波结构体
*/

typedef struct
{
	float LastP;//上次估算协方差，不可以为0
	float NowP;//当前估算协方差
	float out;//卡尔曼滤波器输出
	float Kg;//卡尔曼增益
	float Q;//过程噪声协方差
	float R;//观察噪声协方差
}KALMAN_FILTER_T;



#endif