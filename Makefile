DEEPCHAPPIE_VERSION = 1.0.0
# common
CURDIR = $(shell pwd)
export ROOT_PATH = $(CURDIR)

#misc
BUILD_COMPLETE_STRING ?= $(shell date "+%a, %d %b %Y %T %z")
UID := $(shell id -u)
GID := $(shell id -g)


OUTPUT_DIR= $(CURDIR)/output

# cross host
TARGET_CROSS_HOST = 
export $(TARGET_CROSS_HOST)

# rules
include $(ROOT_PATH)/linux.mk

#path
HAL_DIR=$(ROOT_PATH)/HAL
APP_DIR=$(ROOT_PATH)/App
FRAMEWORK_DIR=$(ROOT_PATH)/Framework
MEDIA_DIR=$(ROOT_PATH)/Media
LIB_DIR=$(ROOT_PATH)/Library


CROSS_COMPILE=$(TARGET_CROSS_HOST)- 

LD_C_FLAGS   += -mno-unaligned-access -fno-aggressive-loop-optimizations  -ffunction-sections -fdata-sections -ftree-vectorize -Wall -Wno-unused-function -Wno-unused-variable -Wno-unused-but-set-variable -mcpu=cortex-a9 -mfloat-abi=hard -mfpu=neon-vfpv4  -O2 -fPIC -g -Wl,-gc-sections
LD_CPP_FLAGS +=  -ldl -lm -lpthread -lrt  -lstdc++  #C++²ÎÊý

PATH_LIB := -Xlinker "-("\
			$(wildcard $(MEDIA_DIR)/hdal/lib/*.a) \
			-Xlinker "-)"

INC_PATH = $(APP_DIR)/inc



BUILD_ALL = HAL
BUILD_ALL += App
BUILD_ALL += Framework
BUILD_ALL += Media

all: $(BUILD_ALL)
	$(CXX) main.cpp -o $(OUTPUT_DIR)/Chappie  -I $(INC_PATH)   $(LD_C_FLAGS) $(LD_CPP_FLAGS) -L $(OUTPUT_DIR)/lib  -lHAL -lFramework -lMedia  -lApp
	@$(ECHO) "Finish generating images at $(BUILD_COMPLETE_STRING)"

.PHONY: App App_Clean Framework Framework_Clean HAL HAL_Clean

checkenv:
	@if [ ! -e $(OUTPUT_DIR)/lib ]; then \
		mkdir $(OUTPUT_DIR); \
		mkdir $(OUTPUT_DIR)/lib; \
	fi

HAL: checkenv
	@$(ECHO) "##### Build HAL ####"
	make -C $(HAL_DIR)
	if [ -a $(HAL_DIR)/Lib/libHAL.a ]; then \
		cp $(HAL_DIR)/Lib/libHAL.a $(OUTPUT_DIR)/lib/; \
	fi

HAL_Clean:
	@$(ECHO) "##### Build HAL clean ####"
	make -C $(HAL_DIR) clean

App: checkenv
	@$(ECHO) "##### Build app ####"
	make -C $(APP_DIR)
	if [ -a $(APP_DIR)/Lib/libApp.a ]; then \
		cp $(APP_DIR)/Lib/libApp.a $(OUTPUT_DIR)/lib/; \
	fi

App_Clean:
	@$(ECHO) "##### Build app clean ####"
	make -C $(APP_DIR) clean

Framework: checkenv
	@$(ECHO) "##### Build framework ####"
	make -C $(FRAMEWORK_DIR)
	if [ -a $(FRAMEWORK_DIR)/Lib/libFramework.a ]; then \
		cp $(FRAMEWORK_DIR)/Lib/libFramework.a $(OUTPUT_DIR)/lib/; \
	fi

Framework_Clean: 
	@$(ECHO) "##### Build framework clean ####"
	make -C $(FRAMEWORK_DIR) clean

clean: App_Clean Framework_Clean HAL_Clean
	@$(ECHO) "RM  $(OUTPUT_DIR)"

	@$(RM)  $(OUTPUT_DIR)

distclean:clean