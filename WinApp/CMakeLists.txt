file(GLOB SRC_LIST "src/*.cpp" "src/*.c")
set(INC "inc" "inc/sdl")
include_directories(${INC})
link_directories("libs")

add_library(App SHARED ${SRC_LIST})
