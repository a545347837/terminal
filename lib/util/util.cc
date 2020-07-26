//
// Created by caizh on 2020/7/23.
//
#include <regex>
#include "util.h"

/**
 * 校验请求参数
 * @param userRequest
 * @return
 */
bool TerminalUtil::checkParam(const UserRequest *userRequest){
    return true || userRequest != nullptr &&
           isMatch(userRequest ->username(),std::string(USERNAME_REGEX)) &&
           (userRequest->password().compare(EMPTY) != 0);
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


/**
    * 检查字符串是否符合正则表达式
    */
bool TerminalUtil::isMatch(std::string str,std::string regexStr){
    bool matchResult = false;
    try{
        matchResult = regex_match(str, std::regex(regexStr));
    }catch (...){
        std::cout<<"服务器上的gcc版本太老，请升级到4.9及以上"<<std::endl;
    }
    return matchResult;
}