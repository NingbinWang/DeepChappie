MODULE_SUB_DIR = src/

MODULE_SUB_INC = inc/
MODULE_SUB_INC += ./../HAL/inc/

#MODULE_SUB_INC += src/
#MODULE_SUB_INC += ./../HAL/inc/
HDAL=hdal
ifeq ($(TARGET_MEDIA),$(HDAL))
MODULE_SUB_DIR += hdal/
MODULE_SUB_INC += hdal/
MODULE_SUB_INC += hdal/include/
HDAL_LIB_PATH = $(MODULE_CUR_PATH)/hdal/Libs
#-ldl -lm -lpthread -lrt --std=c99 -D _GNU_SOURCE
WARNING		= -Wall -Wundef -Wsign-compare -Wno-missing-braces -Wstrict-prototypes -Werror
COMPILE_OPTS	=  -I. -O2 -fPIC -ffunction-sections -fdata-sections -D__LINUX
PLATFORM_CFLAGS = -march=armv7-a -mtune=cortex-a9 -mfpu=neon -mfloat-abi=hard -ftree-vectorize -fno-builtin -fno-common -Wformat=1 -D_BSP_NA51055_  --std=gnu99 

LD_C_FLAGS += $(PLATFORM_CFLAGS) $(COMPILE_OPTS) $(WARNING)  -L$(HDAL_LIB_PATH) -lhdal

#LD_C_FLAGS += $(PLATFORM_CFLAGS) $(COMPILE_OPTS)  -L$(HDAL_LIB_PATH) -lhdal
endif


MODULE_NAME = Media
MODULE_CUR_PATH = $(CURDIR)

include $(ROOT_PATH)/Rules/platform.mk
#模块库mk  调试执行程序mk
include $(ROOT_PATH)/Rules/module.mk