-include Rules.mk

SHELL = /bin/bash
PYTHON := python
#编译时间
BUILD_COMPLETE_STRING ?= $(shell date "+%a, %d %b %Y %T %z")

# 目录定义
ROOT_PATH = $(TOP_BUILD_DIR)
APP_PATH = $(ROOT_PATH)/App
FRAMEWORK_PATH = $(ROOT_PATH)/Framework

#生成执行文件路径
PRODUCT_BIN_PATH = $(ROOT_PATH)

#生成执行文件 调试进程程序
TARGET_BIN = $(PRODUCT_BIN_PATH)/Chappie
TARGET_DEBUG_BIN = $(PRODUCT_BIN_PATH)/Chappie_debug

#模块搜索目录
LIB_PATH = $(ROOT_PATH)/Libs/ $(ROOT_PATH)/Libs/share
LIBS_SOURCE += $(foreach dir, $(LIB_PATH), $(wildcard $(dir)/*.a))
LIBS_SO = $(foreach dir, $(LIB_PATH), $(wildcard $(dir)/*.so))

#添加链接库路径
LD_FLAGS += $(patsubst %,-L%,$(LIB_PATH))
LD_FLAGS += $(patsubst lib%.a,-l%, $(notdir $(LIBS_SOURCE)))
LD_FLAGS += $(patsubst lib%.so,-l%, $(notdir $(LIBS_SO)))

# 搜索主代码库
SRC = $(wildcard $(APP_PATH)/src/*.c)
INC = $(wildcard $(APP_PATH)/inc/*.h)
OBJS 	= $(patsubst %.c, %.o, $(SRC))

$(TARGET_BIN) : $(OBJS)
	$(CC) -o $@ $(OBJS) $(LD_FLAGS) $(C_FLAGS)
	$(STRIP) $@
	chmod 777 $@

%.o:%.c
	$(CC) $(C_FLAGS) -c $^ -o $@ 

.PHONY: clean
#生成执行文件 调试程序

clean:
	rm $(APP_PATH)/src/*.o -rf
	rm $(TARGET_BIN)
	
	