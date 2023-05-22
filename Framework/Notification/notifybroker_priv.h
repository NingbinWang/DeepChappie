#ifndef _NOTIFYBROKER_PRIV_H_
#define _NOTIFYBROKER_PRIV_H_

#include "DataCentor.h"
#include "sys_mem.h"
#include "logger.h"
#include <stdio.h>
#include <string.h>

/**
 * @struct    NOTIFYBROKER_PRIV_DATA_T
 * @brief     NOTIFICATION私有数据结构
*/
typedef struct 
{
  UINT32 BufferSize;
}NOTIFYBROKER_PRIV_DATA_T;

/**
 * @struct    NOTIFYBROKER_BASE_T
 * @brief    NOTIFYBROKER基础数据结构
*/
typedef struct
{
    NOTIFYBROKER_PRIV_DATA_T stPrivData;  /* 私有数据 */
} NOTIFYBROKER_BASE_T;





#endif