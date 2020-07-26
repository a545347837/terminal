package(
    default_visibility = [
         "//visibility:public",
    ],
)

cc_library(
    name = "common_util",
    srcs = ["common_util.cc"],
    hdrs = ["common_util.h"],
)

cc_library(
    name = "db_util",
    srcs = ["db.cc"],
    hdrs = ["db.h"],
    deps = [
            "@usr_mysql//:mysql"
    ]
)

cc_library(
    name = "util",
    srcs = ["util.cc"],
    hdrs = ["util.h"],
    deps = ["//lib/facade:terminal_proto",
            "//lib/thirt_party/bcrypt:bcrypt",
            ":db_util",
            ],
)