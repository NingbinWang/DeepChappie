file(GLOB SRC_LIST
"Notification/PingPongBuffer/*.cpp"
"Notification/*.cpp"
"Logger/*.cpp"
"Hardware/*.cpp"
"DevMonitor/*.cpp"
"Storager/*.cpp"
"Network/*.cpp"
)
include_directories(
#api
"inc"
#HAL API
"./../HAL/inc"
#HAL BSP  API
"./../HAL/devices/include"
#Media API
"./../Media/inc"
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
"Storager"
"Network"
)

#target_link_libraries(Framework
#HAL
#)
add_library(Framework ${SRC_LIST})

