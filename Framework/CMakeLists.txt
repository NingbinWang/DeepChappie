file(GLOB SRC_LIST
"Notification/PingPongBuffer/*.c"
"Notification/*.cpp"
"Log/*.cpp"
"AVB/*.cpp"
"AVB/talker/*.cpp"
"src/*.cpp"
)
include_directories(
#api
"inc"
"inc/AVB"
#HAL API
"./../HAL/inc"
#HAL BSP  API
"./../HAL/devices/include"
# inside include
"Notification"
"Notification/PingPongBuffer"
# avb
"AVB/adapter"
"AVB/talker"


"Logger"
"src"
)




add_library(Framework ${SRC_LIST})