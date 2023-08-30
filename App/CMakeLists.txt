file(GLOB SRC_LIST 
"src/*.cpp"
"src/can/*.cpp"
"src/imu/*.cpp"
"src/media/*.cpp"
"src/protocol/*.cpp"
"src/utils/*.cpp"
"src/storager/*.cpp"
"src/framework/*.cpp"
"src/test/*.cpp"
)
include_directories(
"inc"
"inc/imu"
"inc/can"
"inc/media"
"inc/protocol"
"inc/utils"
"inc/framework"
"inc/storager"
"inc/other"
#HAL Framework API
"./../Framework/inc"
"./../HAL/inc"
"./../Media/inc"
"inc/test"

)
add_library(App ${SRC_LIST})

