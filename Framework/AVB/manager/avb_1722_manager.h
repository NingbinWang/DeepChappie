#ifndef _AVB_1722_MANAGER_H_
#define _AVB_1722_MANAGER_H_

#include "sys_common.h"
#include "sys_posix.h"
#include "sys_mem.h"
#include "sys_time.h"
#include "sys_utils.h"

/**
* @enum AVB_1722_TALKER_CMD_E
* @brief 控制指令类型枚举定义
 */
typedef enum
{
    AVB_1722_TALKER_CMD_INIT,
    AVB_1722_TALKER_CMD_START,   /*开始*/
    AVB_1722_TALKER_CMD_STOP,    /*停止*/
}AVB_1722_TALKER_CMD_E;

/**
* @enum AVB_1722_TALKER_RESULT_E
* @brief 控制指令结果枚举定义
*/
typedef enum
{
    AVB_1722_TALKER_RESULT_DEFAULT,
    AVB_1722_TALKER_RESULT_SUCCESS,
    AVB_1722_TALKER_RESULT_FAILED,
}AVB_1722_TALKER_RESULT_E;

/**
* @struct AVB_1722_TALKER_MSG_T
* @brief 控制指令结构体定义
*/
typedef struct
{
    AVB_1722_TALKER_CMD_E    eCmd;
    AVB_1722_TALKER_RESULT_E eResult;//结果
    UINT32 uRes;
}AVB_1722_TALKER_MSG_T;







#endif