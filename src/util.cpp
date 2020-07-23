//
// Created by caizh on 2020/7/23.
//
#include "util.h"

TerminalUtil::TerminalUtil(){
    connectPool = new map<string,int>();
}
TerminalUtil::~TerminalUtil(){
    delete connectPool;
}

// 连接池相关操作

/**
 * 提交一个连接
 * @param username
 * @return 返回该连接的句柄
 */
int TerminalUtil::submitConnection(string username){
    map<string,int>::iterator iter = connectPool->find(username);
    int fd = 0;
    if (iter != connectPool->end()){
        fd = (iter -> second) + 1;
        iter -> second = fd;
    } else{
        connectPool->insert(pair<string, int>(username,fd));
    }
    return fd;
}

/**
 * 将该用户下的连接移除
 * @param username
 * @param fd 即将移除的连接句柄
 */
void TerminalUtil::removeConnection(string username,int fd){
    map<string,int>::iterator iter = connectPool->find(username);
    if (iter != connectPool->end()){
        connectPool->erase(iter);
    }
}

/**
 * 判断该用户某个连接是否处于连接状态
 * @param username
 * @param fd
 * @return
 */
bool TerminalUtil::isConnected(string username,int fd){
    map<string,int>::iterator iter = connectPool->find(username);
    if (iter != connectPool->end()){
        return iter -> second == fd;
    }
    return false;
}

/**
 * 校验请求参数
 * @param userRequest
 * @return
 */
bool TerminalUtil::checkParam(const UserRequest *userRequest){
    return userRequest != nullptr &&
           (userRequest->username().compare(EMPTY) != 0) &&
           (userRequest->password().compare(EMPTY) != 0);
}