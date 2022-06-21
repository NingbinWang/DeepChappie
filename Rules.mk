-include init_build_env
# 顶层目录
TOP_BUILD_DIR = $(shell pwd)
export TOP_BUILD_DIR

#交叉编译链
export CC = $(CROSS_COMPILE)gcc
export CPP = $(CROSS_COMPILE)gcc 
export LD = $(CROSS_COMPILE)ld 
export AR = $(CROSS_COMPILE)ar
export STRIP = $(CROSS_COMPILE)strip

#--------- ENVIRONMENT SETTING --------------------
WARNING		= -Wall -Wundef -Wsign-compare -Wno-missing-braces -Wstrict-prototypes
COMPILE_OPTS = -I. -O2 -fPIC -ffunction-sections -fdata-sections
CPPFLAGS	= 
#--------- END OF ENVIRONMENT SETTING -------------