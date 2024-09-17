file(GLOB SRC_LIST 
"src/*.c"
"hdal/*.c"
)
include_directories(
#api
"inc"
"hdal/include"
"hdal"
"src"
)
add_library(Media ${SRC_LIST})
