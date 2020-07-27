//
// Created by root on 2020/7/27.
//

#include "connection_pool.h"

ConnectionPool::ConnectionPool(){
    connectPool = new std::map<std::string,int>;
}

ConnectionPool::~ConnectionPool(){
    delete connectPool;
}

/**
 * 提交一个连接
 * @param username
 * @return 返回该连接的句柄
 */
int ConnectionPool::submitConnection(std::string username){
    std::map<std::string,int>::iterator iter = connectPool->find(username);
    int fd = 0;
    if (iter != connectPool->end()){
        fd = (iter -> second) + 1;
        iter -> second = fd;
    } else{
        connectPool->insert(std::pair<std::string, int>(username,fd));
    }
    return fd;
}

/**
 * 将该用户下的连接移除
 * @param username
 * @param fd 即将移除的连接句柄
 */
void ConnectionPool::removeConnection(std::string username,int fd){
    std::map<std::string,int>::iterator iter = connectPool->find(username);
    if (iter != connectPool->end() && iter -> second == fd){
        connectPool->erase(iter);
    }
}

/**
 * 判断该用户某个连接是否处于连接状态
 * @param username
 * @param fd
 * @return
 */
bool ConnectionPool::isAlived(std::string username,int fd){
    std::map<std::string,int>::iterator iter = connectPool->find(username);
    if (iter != connectPool->end()){
        return iter -> second == fd;
    }
    return false;
}
