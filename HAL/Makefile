MODULE_SUB_DIR = src/block/
MODULE_SUB_DIR += src/cmd/
MODULE_SUB_DIR += src/common/
MODULE_SUB_DIR += src/fdisk/
MODULE_SUB_DIR += src/file/
MODULE_SUB_DIR += src/list/
MODULE_SUB_DIR += src/medium/
MODULE_SUB_DIR += src/mem/
MODULE_SUB_DIR += src/mutex/
MODULE_SUB_DIR += src/net/
MODULE_SUB_DIR += src/queue/
MODULE_SUB_DIR += src/socket/
MODULE_SUB_DIR += src/thread/
MODULE_SUB_DIR += src/time/
MODULE_SUB_DIR += src/timer/
MODULE_SUB_DIR += devices/gsensor/
MODULE_SUB_DIR += devices/serial/


MODULE_SUB_INC = inc
MODULE_SUB_INC += src/block/
MODULE_SUB_INC += src/cmd/
MODULE_SUB_INC += src/common/
MODULE_SUB_INC += src/fdisk/
MODULE_SUB_INC += src/file/
MODULE_SUB_INC += src/medium/
MODULE_SUB_INC += src/mem/
MODULE_SUB_INC += src/mutex/
MODULE_SUB_INC += src/net/
MODULE_SUB_INC += src/queue/
MODULE_SUB_INC += src/socket/
MODULE_SUB_INC += src/thread/
MODULE_SUB_INC += src/time/
MODULE_SUB_INC += src/timer/
MODULE_SUB_INC += devices/gsensor/
MODULE_SUB_INC += devices/serial/
MODULE_SUB_INC += devices/include/


MODULE_NAME = HAL
MODULE_CUR_PATH = $(CURDIR)
LD_C_FLAGS += --std=gnu99 -D _GNU_SOURCE
include $(ROOT_PATH)/Rules/platform.mk
#模块库mk  调试执行程序mk
include $(ROOT_PATH)/Rules/module.mk