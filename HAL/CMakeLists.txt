file(GLOB SRC_LIST "src/file/*.cpp" )
include_directories("inc")

add_library(HAL ${SRC_LIST})

#搜索一个目录下的所有的
#aux_source_directory(. SRC_LIST)
#add_library(App ${SRC_LIST})

# list追加或者删除变量的值
#list(APPEND SRC_LIST app.cpp)
#list(REMOVE_ITEM SRC_LIST app.cpp)
