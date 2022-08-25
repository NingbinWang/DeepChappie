file(GLOB SRC_LIST "src/*.cpp" "src/*.c")
include_directories("inc")

add_library(App ${SRC_LIST})
