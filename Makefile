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
include $(ROOT_PATH)/Rules/platform.mk

#path
HAL_DIR=$(ROOT_PATH)/HAL
APP_DIR=$(ROOT_PATH)/App
FRAMEWORK_DIR=$(ROOT_PATH)/Framework



CROSS_COMPILE=$(TARGET_CROSS_HOST)- 

BUILD_ALL = HAL
BUILD_ALL += App
BUILD_ALL += Framework


all: $(BUILD_ALL)
	@$(CXX) -o $(OUTPUT_DIR)/Chappie main.cpp $(HAL_DIR)/Lib/libHAL.a $(FRAMEWORK_DIR)/Lib/libframework.a $(APP_DIR)/Lib/libapp.a
	@$(ECHO) "Finish generating images at $(BUILD_COMPLETE_STRING)"

.PHONY: App App_Clean Framework Framework_Clean HAL HAL_Clean

checkenv:
	@if [ ! -e $(OUTPUT_DIR) ]; then \
		mkdir $(OUTPUT_DIR); \
	fi

HAL: checkenv
	@$(ECHO) "##### Build HAL ####"
	make -C $(HAL_DIR)

HAL_Clean:
	@$(ECHO) "##### Build HAL clean ####"
	make -C $(HAL_DIR) clean

App: checkenv
	@$(ECHO) "##### Build app ####"
	make -C $(APP_DIR)

App_Clean:
	@$(ECHO) "##### Build app clean ####"
	make -C $(APP_DIR) clean

Framework: checkenv
	@$(ECHO) "##### Build framework ####"
	make -C $(FRAMEWORK_DIR)

Framework_Clean: 
	@$(ECHO) "##### Build framework clean ####"
	make -C $(FRAMEWORK_DIR) clean

clean: App_Clean Framework_Clean
	@$(ECHO) "RM  $(OUTPUT_DIR)"

	@$(RM)  $(OUTPUT_DIR)

distclean:clean