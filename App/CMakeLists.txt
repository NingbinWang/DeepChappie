file(GLOB SRC_LIST 
"src/*.cpp" 

)
include_directories(
"inc"
#HAL Framework API
"./../Framework/inc"
)
add_library(App ${SRC_LIST})
target_link_libraries(App
Framework
HAL)


