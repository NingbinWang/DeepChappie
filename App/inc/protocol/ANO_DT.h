#ifndef _ANO_DT_H_
#define _ANO_DT_H_


//设置相关的输出模式
typedef enum ANTO_MODE_E{
    ANTO_VER           =  0x00,//版本号
    ANTO_STATUS        =  0X01,
    ANTO_SENSOR        =  0x02,
    ANTO_RDCDATA       =  0x03,
    ANTO_GPSDATA       =  0x04,
    ANTO_POWER         =  0x05,
    ANTO_MOTOR         =  0x06,
    ANTO_SENSOR2       =  0x07,
    ANTO_RES           =  0x08,
    ANTO_PARAMETERSET  =  0x09,
    ANTO_FLY_MODE      =  0x0A,
    ANTO_SPEEDID       =  0x0B,
    ANTO_RES1          =  0x0C,
    ANTO_RES2          =  0x0D,
    ANTO_RES3          =  0x0F,
    ANTO_RATE_PID      =  0x10,
    ANTO_ANGLE_PID     =  0x11,
    ANTO_HIGHT_PID     =  0x12,
    ANTO_PID4          =  0x13,
    ANTO_PID5          =  0x14,
    ANTO_PID6          =  0x15,
    ANTO_RES4          =  0x16,
    ANTO_RES5          =  0x17,
    ANTO_ES6           =  0x18,
    ANTO_RES7          =  0x19,
    ANTO_FPNUM         =  0x20,
}ANTO_MODE_E;

typedef struct GSENSOR_DATA_T
{
    short accX;
    short accY;
    short accZ;
    short gyroX;
    short gyroY;
    short gyroZ;
}GSENSOR_DATA_T;






#endif