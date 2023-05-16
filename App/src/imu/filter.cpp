#include "filter.h"

void Kalman1Creater(KALMAN_FILTER_T *pKalman,const float fLastP,const float fQ,const float fR)
{
	pKalman->LastP = fLastP;
	pKalman->Q = fQ;
	pKalman->R= fR;
}

void Kalman1Filter(KALMAN_FILTER_T *pKalman,const float input)
{
    //预测协方差方程：k时刻系统估算协方差 = k-1时刻的系统协方差 + 过程噪声协方差
	pKalman->NowP = pKalman->LastP + pKalman->Q;
    //卡尔曼增益方程：卡尔曼增益 = k时刻系统估算协方差 / （k时刻系统估算协方差 + 观测噪声协方差）
	pKalman->Kg = pKalman->NowP / (pKalman->NowP + pKalman->R);
     //更新最优值方程：k时刻状态变量的最优值 = 状态变量的预测值 + 卡尔曼增益 * （测量值 - 状态变量的预测值）
	pKalman->out = pKalman->out + pKalman->Kg * (input - pKalman->out);//因为这一次的预测值就是上一次的输出值
    //更新协方差方程: 本次的系统协方差付给 kfp->LastP 为下一次运算准备。
	pKalman->LastP = (1-pKalman->Kg) * pKalman->NowP ;
}


