package(
    default_visibility = [
         "//visibility:public",
    ],
)

load("@com_github_grpc_grpc//bazel:grpc_build_system.bzl", "grpc_proto_library")
# 生成通信文件
grpc_proto_library(
		name = "terminal_proto",
		srcs = ["terminal.proto"],
)