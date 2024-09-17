#include "ANO_DT.h"
#include "Gsensor.h"
#include "ANO_FUNC.h"
#include <stdio.h>
#include <string.h>

/**@fn	      ANTO_Send	  
 * @brief	  匿名协议栈发送
 * @param[out] N/A
 * @param[in]  mode ANTO的模式
 * @return	   
 */
int ANTO_Send(const ANTO_MODE_E mode,int sockfd)
{
    unsigned short len = 2;
    int i = 0;
    GSENSOR_DATA_T data;
    short anto[12];
    char* pt=(char *)anto;
    switch (mode)
    {
    case ANTO_SENSOR:
        memset(&data,0,sizeof(GSENSOR_DATA_T));
        data.accX = 11;
        data.accY = 22;
        data.accZ = 33;
        data.gyroX = 44;
        data.gyroY = 55;
        data.gyroZ = 66;
        memcpy(&anto[2],(short*)&data,sizeof(GSENSOR_DATA_T));
        anto[8]=0;
        anto[9]=0;
        anto[10]=0;
        len = 18;
        break;
    
    default:
        break;
    }
    anto[0]=0xAAAA;
    anto[1]=len | mode << 8;
    pt[len+4] = 0xaa+0xaa;
    for(i=2;i < len+4;i+=2)
    {
        pt[i]^=pt[i+1];
        pt[i+1] ^= pt[i];
        pt[i] ^= pt[i+1];
        pt[len+4] += (pt[i]+pt[i+1]);
    }

    ANO_udp_send(sockfd,anto,(int)(len+5));
    return len;
}


int ANTO_Recive(const ANTO_MODE_E mode,int sockfd)
{
    return 0;
}