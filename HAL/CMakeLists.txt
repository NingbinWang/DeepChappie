file(GLOB SRC_LIST 
#compenment
"src/fdisk/*.c"
"src/cmd/*.c" 
"src/common/*.c" 
"src/file/*.c" 
"src/medium/*.c" 
"src/mem/*.c" 
"src/mutex/*.c" 
"src/net/*.c" 
"src/queue/*.c"
"src/socket/*.c"
"src/thread/*.c"
"src/time/*.c"
"src/timer/*.c"
"src/block/*.c"
"src/list/*.c"
"devices/gsensor/*.c"
"devices/serial/*.c"
)
include_directories(
#api
"inc"
#compenment 
"src/cmd" 
"src/common"
"src/file"
"src/medium"
"src/mem"
"src/mutex"
"src/net"
"src/queue"
"src/socket"
"src/thread"
"src/time"
"src/timer"
"src/block"
"src/fdisk"
#devices api
"devices/include"
#bsp
"devices/gsensor"
"devices/serial"
)

add_library(HAL ${SRC_LIST})

#搜索一个目录下的所有的
#aux_source_directory(. SRC_LIST)
#add_library(App ${SRC_LIST})

# list追加或者删除变量的值
#list(APPEND SRC_LIST app.cpp)
#list(REMOVE_ITEM SRC_LIST app.cpp)
