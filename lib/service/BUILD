package(
    default_visibility = [
        "//visibility:public",
    ],
)

cc_library(
    name = "connection_pool",
    srcs = ["connection_pool.cc"],
    hdrs = ["connection_pool.h"],
)

cc_library(
    name = "terminal_service_impl",
    hdrs = ["terminal_service_impl.hpp"],
    deps = [
        ":connection_pool",
        "//lib/dao:user_info_dao",
        "//lib/domain:user_info",
        "//lib/util:server_util",
    ],
)
