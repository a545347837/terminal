cmake_minimum_required(VERSION 3.18)
project(terminal)
set(CMAKE_CXX_STANDARD 14)
set(GOOGLETEST_VERSION 1.10.0)
include_directories("/usr/include/mysql")
include_directories("./client/include")
include_directories("./server/include")
include_directories("./googletest/include")
include_directories("./googlemock/include")

link_directories(/usr/lib64/mysql)
link_directories(./googletest/include)
link_directories(./googlemock/include)

link_libraries(mysqlclient pthread)

add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(googletest)

add_executable(app_client app_client.cpp )
add_executable(app_server app_server.cpp)
add_executable(test_client ./tests/client_test.cpp)
add_executable(test_server ./tests/server_test.cpp)

target_link_libraries(app_client ClientLib)
target_link_libraries(app_server ServerLib mysqlclient pthread)
target_link_libraries(test_client gtest ClientLib)
target_link_libraries(test_server gtest ServerLib mysqlclient pthread)
