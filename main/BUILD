cc_binary(
    name = "app_server",
    srcs = ["app_server.cc"],
    deps = [
        "//lib/dao:user_info_dao",
        "//lib/domain:user_info",
        "//lib/facade:terminal_proto",
        "//lib/service:terminal_service_impl",
        "//lib/util:common_util",
        "//lib/util:db_util",
        "//lib/util:server_util",
        "@com_github_grpc_grpc//:grpc++",
        "@usr_mysql//:mysql",
    ],
)

cc_binary(
    name = "app_client",
    srcs = ["app_client.cc"],
    deps = [
        "//lib/facade:terminal_proto",
        "//lib/util:common_util",
        "@com_github_grpc_grpc//:grpc++",
    ],
)
