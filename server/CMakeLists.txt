include_directories("/usr/include/mysql")
AUX_SOURCE_DIRECTORY(./src DIR_LIB_SRCS)
add_library(ServerLib ${DIR_LIB_SRCS})