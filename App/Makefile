MODULE_SUB_DIR = src/
MODULE_SUB_DIR += src/can/
MODULE_SUB_DIR += src/framework/
MODULE_SUB_DIR += src/imu/
MODULE_SUB_DIR += src/media/
MODULE_SUB_DIR += src/protocol/
MODULE_SUB_DIR += src/storager/
MODULE_SUB_DIR += src/ui/
MODULE_SUB_DIR += src/utils/


MODULE_SUB_INC = inc/
MODULE_SUB_INC += inc/can/
MODULE_SUB_INC += inc/framework/
MODULE_SUB_INC += inc/imu/
MODULE_SUB_INC += inc/media/
MODULE_SUB_INC += inc/protocol/
MODULE_SUB_INC += inc/storager/
MODULE_SUB_INC += inc/utils/


#外部的一些头文件
MODULE_SUB_INC += ./../Framework/inc/
MODULE_SUB_INC += ./../HAL/inc/
MODULE_SUB_INC += ./../Media/inc/
#MODULE_SUB_INC += ./../Media/hdal/include/
MODULE_SUB_INC += ./../uvc_lib/inc/

LD_C_FLAGS += --std=gnu99 -D _GNU_SOURCE


MODULE_NAME = App
MODULE_CUR_PATH = $(CURDIR)
include $(ROOT_PATH)/Rules/platform.mk
#模块库mk  调试执行程序mk
include $(ROOT_PATH)/Rules/module.mk
