file(GLOB SRC_LIST
"Notification/PingPongBuffer/*.c"
"Notification/*.cpp"
"Log/*.cpp"
"AVB/talker/*.cpp"
"AVB/adaptor/*.cpp"
"src/*.cpp"
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
"AVB/include"
"AVB/adaptor"
"AVB/talker"
"AVB/manager"
"AVB/adaptor"

"Logger"
"src"
)




add_library(Framework ${SRC_LIST})