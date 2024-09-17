DEEPCHAPPIE_VERSION = 1.0.0
# common
CURDIR = $(shell pwd)
export ROOT_PATH = $(CURDIR)

#misc
BUILD_COMPLETE_STRING ?= $(shell date "+%a, %d %b %Y %T %z")
UID := $(shell id -u)
GID := $(shell id -g)


OUTPUT_DIR= $(CURDIR)/output


# media view

TARGET_MEDIA = hdal
#export $(TARGET_MEDIA)

# cross host
TARGET_CROSS_HOST = /home/alex/workspace/novatek/nt9852x_sdk_2.05.009/toolchain/arm-ca9-linux-gnueabihf-6.5/bin/arm-ca9-linux-gnueabihf
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

LD_C_FLAGS   +=  -ldl -lm -lpthread -lrt  -std=c99
LD_CPP_FLAGS +=  -ldl -lm -lpthread -lrt  -lstdc++  #C++参数

HDAL_PATH_LIB := -Xlinker "-("\
			$(wildcard $(MEDIA_DIR)/hdal/Libs/*.a) \
			-Xlinker "-)"

INC_PATH = $(APP_DIR)/inc



BUILD_ALL = HAL
BUILD_ALL += App
BUILD_ALL += Framework
BUILD_ALL += Media


.PHONY: App App_Clean Framework Framework_Clean HAL HAL_Clean Media Media_Clean

all: $(BUILD_ALL)
	$(CC) -c main.c -L $(OUTPUT_DIR)/lib -I $(INC_PATH) -lApp
	find $(OUTPUT_DIR)/lib -name "*.a" |xargs -i $(AR) -x {}
	$(AR) -qcs $(OUTPUT_DIR)/libChappie.a *.o
	$(CC) -o $(OUTPUT_DIR)/Chappie $(LD_C_FLAGS) -L $(OUTPUT_DIR)/ -lChappie
	$(RM)  *.o -rf
	$(ECHO) "Finish generating images at $(BUILD_COMPLETE_STRING)"
	

checkenv:
	@if [ ! -e $(OUTPUT_DIR)/lib ]; then \
		mkdir $(OUTPUT_DIR); \
		mkdir $(OUTPUT_DIR)/lib; \
	fi
	find $(LIB_DIR) -name "*.so" |xargs -i cp {} $(OUTPUT_DIR)/lib/
	find $(LIB_DIR) -name "*.a" |xargs -i cp {} $(OUTPUT_DIR)/lib/

HAL: checkenv
	@$(ECHO) "##### Build HAL ####"
	make -C $(HAL_DIR)
	if [ -f $(HAL_DIR)/Lib/libHAL.a ]; then \
		cp $(HAL_DIR)/Lib/libHAL.a $(OUTPUT_DIR)/lib/; \
	fi

HAL_Clean:
	@$(ECHO) "##### Build HAL clean ####"
	make -C $(HAL_DIR) clean

App: checkenv
	@$(ECHO) "##### Build app ####"
	make -C $(APP_DIR)
	@if [ -f $(APP_DIR)/Lib/libApp.a ]; then \
		cp $(APP_DIR)/Lib/libApp.a $(OUTPUT_DIR)/lib/; \
	fi

App_Clean:
	@$(ECHO) "##### Build app clean ####"
	make -C $(APP_DIR) clean

Framework: checkenv
	@$(ECHO) "##### Build framework ####"
	make -C $(FRAMEWORK_DIR)
	@if [ -f $(FRAMEWORK_DIR)/Lib/libFramework.a ]; then \
		cp $(FRAMEWORK_DIR)/Lib/libFramework.a $(OUTPUT_DIR)/lib/; \
	fi

Framework_Clean: 
	@$(ECHO) "##### Build framework clean ####"
	make -C $(FRAMEWORK_DIR) clean

Media: checkenv
	@$(ECHO) "##### Build Media ####"
	@make -C $(MEDIA_DIR) TARGET_MEDIA=$(TARGET_MEDIA)
	if [ -f $(MEDIA_DIR)/Lib/libMedia.a ]; then \
		cp $(MEDIA_DIR)/Lib/libMedia.a $(OUTPUT_DIR)/lib/; \
	fi
	if [ -f $(MEDIA_DIR)/hdal/Libs/libhdal.a ]; then \
		cp $(MEDIA_DIR)/hdal/Libs/*.a $(OUTPUT_DIR)/lib/; \
	fi

Media_Clean: 
	@$(ECHO) "##### Build media clean ####"
	make -C $(MEDIA_DIR) clean

clean: App_Clean Framework_Clean HAL_Clean Media_Clean
	@$(RM)  *.o -rf
	@$(ECHO) "RM  $(OUTPUT_DIR)"
	@$(RM)  $(OUTPUT_DIR)
