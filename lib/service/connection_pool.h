//
// Created by root on 2020/7/27.
//

#ifndef TERMINAL_CONNECTION_POOL_H
#define TERMINAL_CONNECTION_POOL_H
#include <map>
#include <string>

class ConnectionPool{
public:

    // 连接池相关操作

    ConnectionPool();
    ~ConnectionPool();
    /**
     * 提交一个连接
     * @param username
     * @return 返回该连接的句柄
     */
    int submitConnection(std::string username);

    /**
     * 将该用户下的连接移除
     * @param username
     * @param fd 即将移除的连接句柄
     */
    void removeConnection(std::string username,int fd);

    /**
     * 判断该用户某个连接是否处于连接状态
     * @param username
     * @param fd
     * @return
     */
    bool isAlived(std::string username,int fd);

private:
    /**
        * 抽象客户端连接池
        */
    std::map<std::string,int> *connectPool = nullptr;

};
#endif //TERMINAL_CONNECTION_POOL_H
