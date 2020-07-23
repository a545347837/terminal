project(grpc-cmake-example)
cmake_minimum_required(VERSION 3.2)
# 编译级别
add_compile_options(-std=c++11)
set(GOOGLETEST_VERSION 1.10.0)

list(APPEND CMAKE_PREFIX_PATH "/opt/grpc" "/opt/protobuf")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# 自动找包
find_package(Protobuf REQUIRED)
find_package(GRPC REQUIRED)

set(PROTOS
        ${CMAKE_CURRENT_SOURCE_DIR}/protos/terminal.proto
)

set(PROTO_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/proto-src)

# 测试用例文件夹
set(TEST_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# 创建文件夹
file(MAKE_DIRECTORY ${PROTO_SRC_DIR})
include_directories(
        ${PROTO_SRC_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 在build目录生成grpc的服务文件等
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_SRC_DIR} ${PROTOS})
grpc_generate_cpp(GRPC_SRCS GRPC_HDRS ${PROTO_SRC_DIR} ${PROTOS})

# 构建服务端
include_directories(
        "${TEST_SRC_DIR}/googletest/include"
        "/usr/include/mysql"
)
link_directories(
        ${TEST_SRC_DIR}/googletest/include
        /usr/lib64/mysql
)
add_subdirectory(${TEST_SRC_DIR}/googletest)
link_libraries(mysqlclient)
aux_source_directory( ${CMAKE_CURRENT_SOURCE_DIR}/src DIR_LIB_SRCS)
add_executable(app_server
        ${CMAKE_CURRENT_SOURCE_DIR}/app_server.cpp
        ${PROTO_SRCS}
        ${GRPC_SRCS}
        ${DIR_LIB_SRCS}
        )
target_link_libraries(app_server
        gRPC::grpc++_reflection
        protobuf::libprotobuf
        mysqlclient
)

# 构建客户端
aux_source_directory(./src DIR_LIB_SRCS)
add_executable(app_client
    ${CMAKE_CURRENT_SOURCE_DIR}/app_client.cpp
    ${PROTO_SRCS}
    ${GRPC_SRCS}
        )
target_link_libraries(app_client
    gRPC::grpc++_reflection
    protobuf::libprotobuf
)

# 构建gtest测试用例
aux_source_directory( ${CMAKE_CURRENT_SOURCE_DIR}/tests TESTLIB_SRCS)
add_executable(main
        ${PROTO_SRCS}
        ${GRPC_SRCS}
        ${DIR_LIB_SRCS}
        ${TESTLIB_SRCS}
        )
target_link_libraries(main
        gRPC::grpc++_reflection
        protobuf::libprotobuf
        mysqlclient
        gtest
        )
