cmake_minimum_required(VERSION 3.18)
project(terminal)
set(CMAKE_CXX_STANDARD 14)

include_directories("/usr/include/mysql")
include_directories("./client/include")
include_directories("./server/include")

link_directories(/usr/lib64/mysql)
link_libraries(mysqlclient pthread)

add_subdirectory(client)
add_subdirectory(server)

add_executable(app_client app_client.cpp)
add_executable(app_server app_server.cpp)

target_link_libraries(app_client ClientLib)
target_link_libraries(app_server ServerLib mysqlclient pthread)