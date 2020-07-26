# 加载http下载库
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
# 从Github上下载grpc源码包
http_archive(
		name = "com_github_grpc_grpc",
		urls = ["https://github.com/grpc/grpc/archive/v1.30.2.tar.gz"],
		strip_prefix = "grpc-1.30.2",
)

# 加載本地的Mysql 庫
new_local_repository(
    name = "usr_mysql",
    path = "/usr/",
    build_file = "lib/thirt_party/mysql/BUILD"
)

# gtest库
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
git_repository(
    name = "gtest",
    remote = "https://github.com/google/googletest",
    tag = "release-1.10.0",
)

# 加载grpc依赖
load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")
grpc_deps()

# 加载grpc扩展依赖
load("@com_github_grpc_grpc//bazel:grpc_extra_deps.bzl", "grpc_extra_deps")
grpc_extra_deps()
