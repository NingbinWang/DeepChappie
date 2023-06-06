file(GLOB SRC_LIST 
#compenment
"src/fdisk/*.cpp"
"src/cmd/*.cpp" 
"src/common/*.cpp" 
"src/file/*.cpp" 
"src/medium/*.cpp" 
"src/mem/*.cpp" 
"src/mutex/*.cpp" 
"src/net/*.cpp" 
"src/queue/*.cpp"
"src/socket/*.cpp"
"src/thread/*.cpp"
"src/time/*.cpp"
"src/timer/*.cpp"
"src/block/*.cpp"
"src/list/*.cpp"

#BSP
"devices/gsensor/*.cpp"
"devices/serial/*.cpp"
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
