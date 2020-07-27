//
// Created by caizh on 2020/7/23.
//
#include "util.h"

/**
 * 校验请求参数
 * @param userRequest
 * @return
 */
bool TerminalUtil::checkParam(const UserRequest *userRequest){
    return userRequest != nullptr && userRequest->password().compare(EMPTY) != 0 &&
           userRequest->password().compare(EMPTY) != 0;
}

/**
     * 使用brcypt算法对密码进行哈希
     * @param password
     * @return
     */
std::string TerminalUtil::hashPassword(std::string password){
    return BCrypt::generateHash(password, 10);
}
/**
       * 校验账户密码
       * @param requestPassword 请求的密码
       * @param queryPassword 数据库存放的密码
       * @return
       */
bool TerminalUtil::validPassword(std::string requestPassword,std::string queryPassword){
    return  BCrypt::validatePassword(requestPassword,queryPassword);
}
