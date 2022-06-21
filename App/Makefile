# 搜索主代码库
SRC = $(wildcard $(APP_PATH)/src/*.c)
INC = $(wildcard $(APP_PATH)/inc/*.h)
OBJS 	= $(patsubst %.c, %.o, $(SRC))

%.o:%.c
	$(CC) $(C_FLAGS) -c $^ -o $@ 

$(TARGET_BIN) : $(OBJS)
	$(CC) -o $@ $(OBJS) $(LD_FLAGS) $(C_FLAGS)
	$(STRIP) $@
	chmod 777 $@