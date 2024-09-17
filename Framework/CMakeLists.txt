file(GLOB SRC_LIST
"Notification/PingPongBuffer/*.c"
"Notification/*.c"
"Logger/*.c"
"Hardware/*.c"
"DevMonitor/*.c"
"Storager/*.c"
"Network/*.c"
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

