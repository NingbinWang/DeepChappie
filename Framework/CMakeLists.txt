file(GLOB SRC_LIST
"Notification/PingPongBuffer/*.c"
"Notification/*.cpp"
"Log/*.cpp"
#"AVB/talker/*.cpp"
#"AVB/adaptor/*.cpp"
"src/*.cpp"
"Hardware/*.cpp"
)
include_directories(
#api
"inc"
#HAL API
"./../HAL/inc"
#HAL BSP  API
"./../HAL/devices/include"
# inside include
"Notification"
"Notification/PingPongBuffer"
# avb
#"AVB/include"
#"AVB/adaptor"
#"AVB/talker"
#"AVB/manager"
#"AVB/adaptor"

"Logger"
"Hardware"
"src"
)

#target_link_libraries(Framework
#HAL
#)
add_library(Framework ${SRC_LIST})

