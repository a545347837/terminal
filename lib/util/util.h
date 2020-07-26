//
// Created by caizh on 2020/7/23.
//

#ifndef GRPC_CMAKE_EXAMPLE_UTIL_H
#define GRPC_CMAKE_EXAMPLE_UTIL_H

#include <map>
#include <string>
#include "lib/facade/terminal.grpc.pb.h"
#include "lib/thirt_party/bcrypt/BCrypt.hpp"
using terminal::UserRequest;
using terminal::Result;
using terminal::TerminalService;
using terminal::UserRequest;
using terminal::Result;
#define USERNAME_REGEX "^[A-Za-z0-9]{1,20}$"
#define EMPTY ""
/**
 *
 */
class TerminalUtil{
public:

    /**
     * 校验请求参数
     * @param userRequest
     * @return
     */
   static bool checkParam(const UserRequest *userRequest);

    /**
        * 校验账户密码
        * @param requestPassword 请求的密码
        * @param queryPassword 数据库存放的密码
        * @return
        */
    static bool validPassword(std::string requestPassword,std::string queryPassword);

    /**
     * 使用brcypt算法对密码进行哈希
     * @param password
     * @return
     */
    static std::string hashPassword(std::string password);
    /**
     * 检查字符串是否符合正则表达式
     */
    static bool isMatch(std::string str,std::string regex);

};



#endif //GRPC_CMAKE_EXAMPLE_UTIL_H
