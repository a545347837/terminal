cc_binary(
		name = "app_server",
		srcs = ["app_server.cc"],
		deps = [
			    '//lib/facade:terminal_proto',
	            '//lib/util:common_util',
	            '//lib/util:db_util',
                '//lib/util:util',
                '@usr_mysql//:mysql',
				'@com_github_grpc_grpc//:grpc++',
		],
)

cc_binary(
		name = "app_client",
		srcs = ["app_client.cc"],
		deps = [
				 '//lib/facade:terminal_proto',
				 '//lib/util:common_util',
				 '@com_github_grpc_grpc//:grpc++',
		],
)
