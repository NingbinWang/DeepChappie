MODULE_SUB_DIR = DevMonitor/
MODULE_SUB_DIR += Hardware/
MODULE_SUB_DIR += Logger/
MODULE_SUB_DIR += Media/
MODULE_SUB_DIR += Network/
MODULE_SUB_DIR += Notification/
MODULE_SUB_DIR += Notification/PingPongBuffer/
MODULE_SUB_DIR += Storager/

MODULE_SUB_INC += inc/
MODULE_SUB_INC += DevMonitor/
MODULE_SUB_INC += Hardware/
MODULE_SUB_INC += Logger/
MODULE_SUB_INC += Media/
MODULE_SUB_INC += Network/
MODULE_SUB_INC += Notification/
MODULE_SUB_INC += Notification/PingPongBuffer/
MODULE_SUB_INC += Storager/
MODULE_SUB_INC += ./../HAL/inc/
MODULE_SUB_INC += ./../HAL/devices/include/

LD_CPP_FLAGS +=  -L $(CURDIR)/../output/lib -lHAL


MODULE_NAME = Framework
MODULE_CUR_PATH = $(CURDIR)
include $(ROOT_PATH)/Rules/platform.mk
#模块库mk  调试执行程序mk
include $(ROOT_PATH)/Rules/module.mk