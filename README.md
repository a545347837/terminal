# 快速开始
## 执行建库语句
```
DROP TABLE IF EXISTS `user_info`;
CREATE TABLE `user_info` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(255) NOT NULL,
  `password` varchar(255) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `idx_username` (`username`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=50001 DEFAULT CHARSET=latin1;
```
## 克隆源码到本地仓库
`git clone https://github.com/a545347837/terminal.git`
## 修改数据库地址
打开源码
`app_server.cpp`修改以下内容
`TerminalServiceImpl service("db",3306,"root","545347837","test");` 参数依次为数据库地址、数据库端口、登录用户、密码、实例名称
## 编译
回到项目目录，执行以下命令（cmake后面的点不能去掉）
`cmake . && make`
## 运行
./app_server 服务端
./app_client 客户端
./test 测试用例

# 项目说明
## 目录结构
```
├── app_client.cpp					客户端主程序
├── app_server.cpp					服务端主程序
├── cmake							
│   ├── FindGRPC.cmake				        查找grpc包脚本
│   └── FindProtobuf.cmake			        查找protobuf包脚本
├── CMakeLists.txt					cmake打包脚本
├── include							
│   ├── db.h						数据库操作类头文件
│   ├── md5.h						md5工具头文件
│   └── util.h						常用工具方法头文件
├── protos
│   └── terminal.proto				        grpc接口描述文件
├── README.md
├── src
│   ├── db.cpp						数据库操作类
│   ├── md5.cpp						md5类
│   └── util.cpp					工具类
└── tests
    ├── googletest					gtest源码
    ├── main.cpp					测试用例运行主类
    └── util_test.cpp				        工具测试类
```
备注：编译时会自动生成grpc接口对应的服务类和实体等
## 交互时序图
### 注册
![](https://i.loli.net/2020/07/23/cNJSVxCj1qLgMdZ.jpg)

### 登录
![](https://i.loli.net/2020/07/23/3qBzjnJwtFrscLi.jpg)
