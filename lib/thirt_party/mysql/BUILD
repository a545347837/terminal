package(
    default_visibility = [
        "//visibility:public",
    ],
)

cc_library(
    name = "mysql",
    srcs = glob(["lib64/mysql/libmysqlclient.so.18"]),
    hdrs = glob(["includ/mysql/**"]),
    includes = ["include/mysql"],
)
