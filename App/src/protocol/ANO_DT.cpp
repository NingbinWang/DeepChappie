#include "ANO_DT.h"
#include "Gsensor.h"
#include "Framework.h"
#include <string.h>

/**@fn	      ANTO_Send	  
 * @brief	  匿名协议栈发送
 * @param[out] N/A
 * @param[in]  mode ANTO的模式
 * @return	   
 */
void ANTO_Send(const ANTO_MODE_E mode)
{
    unsigned int i = 0;
    unsigned int len = 2;
    short Anto[12];
    IGsensor_manager *gsensor;
    Sensordata_t data;
    GSENSOR_DATA_T antodata;
    gsensor = Framework_GetGsensorinterface();
    
    switch (mode)
    {
    case ANTO_SENSOR:
        gsensor->GetData(gsensor,&data);
        antodata.accX = data.accx;
        antodata.accY = data.accy;
        antodata.accZ = data.accz;
        antodata.gyroX = 0;
        antodata.gyroY = 0;
        antodata.gyroZ = 0;
        memcpy(&Anto[2],&antodata,sizeof(GSENSOR_DATA_T));
        len = 18;
        break;
    
    default:
        break;
    }


    
}