file(GLOB SRC_LIST
"Notification/*.cpp"
"Log/*.cpp"
"AVTP/*.cpp"
)
include_directories(
#api
"inc"
#HAL API
"./../HAL/inc"
# inside include
"AVTP"
"Logger"

)




add_library(Framework ${SRC_LIST})