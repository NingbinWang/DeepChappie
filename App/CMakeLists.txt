file(GLOB SRC_LIST 
"src/*.c"
"src/can/*.c"
"src/framework/*.c"
"src/imu/*.c"
"src/media/*.c"
"src/protocol/*.c"
"src/storager/*.c"
"src/test/*.c"
"src/ui/*.c"
"src/utils/*.c"
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

