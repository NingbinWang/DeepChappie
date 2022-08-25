cmake_minimum_required( VERSION 3.8 )
project( Chappie )
#设置交叉编译的环境
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
#指定交叉编译的gcc  g++。如果没有配置到环境变量，此处写绝对路径
set(CMAKE_C_COMPILER "arm-ca9-linux-gnueabihf-gcc")
set(CMAKE_CXX_COMPILER "arm-ca9-linux-gnueabihf-g++")

############## 以下几条可以不设置(实测可以)，官方文档上步骤 ############
#指定交叉编译环境的目录
#set(CMAKE_FIND_ROOT_PATH "/usr/local/arm_4.9.4/arm-linux-gnueabi")
#从来不在指定目录(交叉编译)下查找工具程序。(编译时利用的是宿主的工具)
#set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
#只在指定目录(交叉编译)下查找库文件
#set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#只在指定目录(交叉编译)下查找头文件
#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
#只在指定的目录(交叉编译)下查找依赖包
#set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
###################################################################

#设置C++ 编译的参数(根据需要自己设置)
set(CMAKE_CXX_FLAGS "-std=c++11 -pthread -Wall -O0 -g3 -Wall  -fmessage-length=0 -fPIC")
set(CMAKE_C_FLAGS "-std=c99 -Wall -O0 -g3 -Wall -fmessage-length=0")
# -Wall       :  编译时显示警告信息
# -O          ： 选择编译器优化的级别，-O0 不优化  -O1 -O2 -O3 不同程度的优化
# -g          :  生成调试信息,debug会用到。调试信息的级别。-g3 
#			    尽可能的生成gdb的可以使用的调试信息。默认为-g2。信息越多，编译出的程序越大。
# -std=c++11  ： 编译所按照C++的标准
# -fmessage-length=0 ：输出信息会根据控制台的宽度自动换行，这样就能看全输出信息了
# -fPIC       ： 作用于编译阶段，告诉编译器产生与位置无关代码
#			    (Position-Independent Code)。加载到任意位置都能执行
# -DTEST	 ： 设置TEST的宏定义，另外一种设置宏定义的方式可以直接使用 add_definitions(-D TEST) 添加

# 连接库的搜索
#set(LINK_DIR "./Library/")
#link_libraries(${LINK_DIR})

# 查找库
set(LIBRARY_PATH "./Library")
set(LIBRARY_LIST libopencv_core.so)
find_library(LIBRAYS ${LIBRARY_LIST} ${LIBRARY_PATH})
if( ${LIBRAYS} STREQUAL "LIBRAYS-NOTFOUND" )
    message(STATUS "libarys is not found")
else()
    message(STATUS "found lib: ${LIBRAYS}")
    link_libraries(${LIBRAYS})
endif()
# 添加子目录
add_subdirectory(App)

# 指定生成的目标
add_executable(Chappie main.cpp)

# 添加链接库 可以是多个
target_link_libraries(Chappie 
App)

