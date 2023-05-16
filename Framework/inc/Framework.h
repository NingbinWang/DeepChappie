#ifndef _FRAMEWORK_H_
#define _FRAMEWORK_H_
#include "Gsensor.h"
#include "Devmonitor.h"
#include "Serial.h"

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

void Framework_Init(void);
int Framework_work(void);

IGsensor_manager* Framework_GetGsensorinterface(void);
IDevMonitor* Framework_GetDevMonitorinrterface(void);


#ifdef __cplusplus
}
#endif/*__cplusplus*/


#endif