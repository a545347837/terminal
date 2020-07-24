//
// Created by caizh on 2020/7/23.
//

#ifndef GRPC_CMAKE_EXAMPLE_UTIL_H
#define GRPC_CMAKE_EXAMPLE_UTIL_H


#include <map>
#include <string>
#include "md5.h"
#include "terminal.grpc.pb.h"
#include <unistd.h>

#include <iostream>
#include <memory>
#include <grpcpp/grpcpp.h>
#include "terminal.grpc.pb.h"
#include "db.h"
#include <regex>
#define EMPTY ""
#define INVALID_USERNAME_MSG "用户不存在"
#define EXIST_USERNAME_MSG "用户名已存在"
#define INVALID_PASSWORD_MSG "密码错误"
#define INVALID_PARAM_MSG "用户或密码格式有误"
#define QUERY_USER_SQL "select * from user_info where username='%s'"
#define INSERT_USER_SQL "insert into user_info(username,password) values('%s','%s')"
#define USERNAME_REGEX "^[A-Za-z0-9]{1,20}$"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;
using terminal::UserRequest;
using terminal::Result;
using terminal::TerminalService;
using terminal::UserRequest;
using terminal::Result;

class TerminalUtil{
public:
    TerminalUtil();
    ~TerminalUtil();

    // 连接池相关操作

    /**
     * 提交一个连接
     * @param username
     * @return 返回该连接的句柄
     */
    int submitConnection(string username);

    /**
     * 将该用户下的连接移除
     * @param username
     * @param fd 即将移除的连接句柄
     */
    void removeConnection(string username,int fd);

    /**
     * 判断该用户某个连接是否处于连接状态
     * @param username
     * @param fd
     * @return
     */
    bool isConnected(string username,int fd);

    /**
     * 校验请求参数
     * @param userRequest
     * @return
     */
    bool checkParam(const UserRequest *userRequest);

    /**
        * 校验账户密码
        * @param requestPassword 请求的密码
        * @param queryPassword 数据库存放的密码
        * @return
        */
    bool checkLogin(string requestPassword,string queryPassword);

    /**
     * 检查字符串是否符合正则表达式
     */
    bool isMatch(string str,string regex);
private:
    /**
        * 抽象客户端连接池
        */
   map<string,int> *connectPool = nullptr;
};



#endif //GRPC_CMAKE_EXAMPLE_UTIL_H
