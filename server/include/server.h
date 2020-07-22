//
// Created by caizh on 2020/7/22.
//
#ifndef TERMINAL_SERVER_H
#define TERMINAL_SERVER_H
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<pthread.h>
#include<map>
#include<csignal>
#include "md5.h"
#include "db.h"

#define SEND_BUFF_SIZE 128
#define LOGIN "1"
#define REGISTER "2"
#define SUCCESS_MSG "success"
#define INVALID_USERNAME_MSG "用户不存在"
#define EXIST_USERNAME_MSG "用户名已存在"
#define INVALID_PASSWORD_MSG "密码错误"
#define INVALID_PARAM_MSG "用户或密码格式有误"
#define QUERY_USER_SQL "select * from user_info where username=''"
#define INSERT_USER_SQL "insert into user_info(username,password) values('','')"
using namespace std;
class Server{
    public:
        Server(const char* host,const unsigned int port,const char* username,const char* password,const char* db_name);
        Server();
        ~Server();
        void start_accept(const unsigned int listen_port);

        // 参数分割
        static int split_params(char *params,string **op,string **username,string **password);
    protected:

        // 创建心跳检测线程
        int create_heartbeat_thread();

        // 心跳函数，用于对客户端发心跳命令
        static void* heartbeat(void* args);

        // 当服务端接收到一个客户端连接，会异步调用该方法进行处理
        static void* handle(void* args);

        // 账号密码校验
        static const char* verification(string &op,string &username,string &password);



        int sockfd;
};


#endif