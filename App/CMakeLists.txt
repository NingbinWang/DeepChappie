file(GLOB SRC_LIST 
"src/*.cpp"
"src/can/*.cpp"
"src/imu/*.cpp"
"src/media/*.cpp"
"src/protocol/*.cpp"
"src/ui/*.cpp"
"src/utils/*.cpp"
"src/storager/*.cpp"
)
include_directories(
"inc"
"inc/imu"
"inc/can"
"inc/media"
"inc/protocol"
"inc/utils"
#HAL Framework API
"./../Framework/inc"
"./../HAL/inc"
"./../Media/inc"

)
add_library(App ${SRC_LIST})
target_link_libraries(App
Framework
HAL
MEDIA)


