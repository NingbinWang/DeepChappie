#ifndef _HDAL_INTERFACE_H_
#define _HDAL_INTERFACE_H_
#include <sys/ipc.h>
#include <sys/shm.h>


/* ========================================================================== */
/*                             Type Declaration                               */
/* ========================================================================== */
typedef enum
{
    HDAL_PROFUCT_ID_DEMO = 0,              
    HDAL_PROFUCT_ID_A30051 = 1,
    HDAL_PROFUCT_ID_B20066 = 2,
} HDAL_PROFUCT_ID_E;

typedef enum
{
    HDAL_CMD_NULL = 0,
} HDAL_CMD_E;

typedef struct 
{
    int chan;
} HDAL_CMD_DATA_T;


/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
//函数实现均在hdal_main.c中
//customz api
int hdal_Mem_Malloc(unsigned int *phy_addr, void **virt_addr, unsigned int size);




int hdal_system_init(HDAL_PROFUCT_ID_E id); //完成hdal的hd_common_init 
int hdal_vi_init(HDAL_PROFUCT_ID_E id);
int hdal_cmd_control(HDAL_CMD_E cmd,HDAL_CMD_DATA_T* pdata);


//int hdal_exit(void);





#endif