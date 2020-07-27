# 单终端管理系统
## 需求点
1. 具备注册登录功能;
2. 一个用户只能在一个设备上登录，切换终端登录时，其他已登录的终端会被踢出;
## 关键技术点
- 使用c++语言编写服务端及客户端代码
- 使用带SSL的grpc实现服务端与客户端的通信
- 使用protobuf实现grpc通信时的序列化及反序列化
- 使用grpc的stream特性实现客户端与服务端保持长连接和通信
- 使用mysql_stmt相关预处理函数保证sql安全，防止被注入
- 使用bcrypt对用户密码进行单向加密
- 使用bazel编译和管理项目
- 使用gtest进行单测编写

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
`Db *db = new Db("db",3306,"root","545347837","test");` 参数依次为数据库地址、数据库端口、登录用户、密码、实例名称
## 编译
回到WORKSPACE项目目录，执行以下命令（cmake后面的点不能去掉）
`bazel build main:all`
## 生成通信秘钥
由于该项目使用了SSL进行通信加密，所以需要生成公私钥，依次执行下述语句生成公私钥
```
mkdir -p /home/config/client
mkdir -p /home/config/server
cd /home/config
openssl genrsa -passout pass:123 -des3 -out ca.key 4096
openssl req -passin pass:123 -new -x509 -days 365 -key ca.key -out ca.crt -subj  "/C=CN/ST=CA/L=Cupertino/O=YourCompany/OU=YourApp/CN=MyRootCA"
openssl genrsa -passout pass:123 -des3 -out server.key 4096
openssl req -passin pass:123 -new -key server.key -out server.csr -subj  "/C=CN/ST=CA/L=Cupertino/O=YourCompany/OU=YourApp/CN=localhost"
openssl x509 -req -passin pass:123 -days 365 -in server.csr -CA ca.crt -CAkey ca.key -set_serial 01 -out server.crt
openssl rsa -passin pass:123 -in server.key -out server.key
openssl genrsa -passout pass:123 -des3 -out client.key 4096
openssl req -passin pass:123 -new -key client.key -out client.csr -subj  "/C=CN/ST=CA/L=Cupertino/O=YourCompany/OU=YourApp/CN=FengJun"
openssl x509 -passin pass:123 -req -days 365 -in client.csr -CA ca.crt -CAkey ca.key -set_serial 01 -out client.crt
openssl rsa -passin pass:123 -in client.key -out client.key
# 客户端读取的密钥
mv client_privatmv client_privatekey.pem client/
mv client_self_signed_crt.pem client/ 
mv server_self_signed_crt.pem client/
# 服务端读取的密钥
mv client_self_signed_crt.pem server/
mv server_privatekey.pem server/
mv server_self_signed_crt.pem server/

```
(服务端和客户端默认读取/home/config/下的文件)
## 运行
./bazel-bin/main/app_server 服务端
./bazel-bin/main/app_client 客户端


# 项目说明
## 目录结构
```
|-- README.md
|-- WORKSPACE                             工作区，声明项目用到的源码库、本地库等
|-- lib                                   调用库
|   |-- dao                               持久化层，对对象进行持久化均在此进行
|   |   |-- BUILD                         
|   |   |-- user_info_dao.cc              用户表持久化操作类
|   |   `-- user_info_dao.h     
|   |-- domain
|   |   |-- BUILD
|   |   `-- user_info.hpp                 用户表映射对象
|   |-- facade                            外部接口包
|   |   |-- BUILD
|   |   `-- terminal.proto                gprc的通信文件
|   |-- service
|   |   |-- BUILD
|   |   |-- connection_pool.cc            虚拟连接池，用来提供抽象管理客户端连接的能力（内部实现实际不涉及真实的连接）
|   |   |-- connection_pool.h       
|   |   `-- terminal_service_impl.hpp     grpc服务实现     
|   |-- thirt_party                       第三方包
|   |   |-- bcrypt                        bcrypt单向加密库
|   |   |-- mysql                         mysql相关的库
|   |       `-- BUILD
|   `-- util
|       |-- BUILD
|       |-- common_util.cc                通用工具类，客户端与服务端均需要引用
|       |-- common_util.h
|       |-- db.cc                         底层数据库操作类，用来提供底层的操作物理库能力
|       |-- db.h
|       |-- util.cc                       服务端使用的工具类
|       `-- util.h
|-- main
|   |-- BUILD
|   |-- app_client.cc                     客户端启动入口类
|   `-- app_server.cc                     服务端启动入口类
`-- test                                  单测库
    |-- service
    |   |-- BUILD.bazel
    |   `-- connection_pool_test.cc
    `-- util
        |-- BUILD.bazel
        |-- common_util_test.cc
        `-- util_test.cc

```
备注：编译时会自动生成grpc接口对应的服务类和实体等
## 交互时序图
### 注册
![](https://i.loli.net/2020/07/27/zSfJwQ2Tv3dVOyo.jpg)

### 登录
![](https://i.loli.net/2020/07/27/5WS7G8MecJQaUVd.jpg)
