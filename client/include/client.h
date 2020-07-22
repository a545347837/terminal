#ifndef CLIENT_H
#define CLIENT_H
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<iostream>
#define SEND_BUFF_SIZE 128
using namespace std;
class Client{
	public:
	  Client();
	  ~Client();
	  
	  // 连接服务器
	  void start_connect(char* host,const unsigned int listen_port);
	private:
	  // 接收服务器消息
	  struct sockaddr_in ser,cli;
	  int sockfd;
	  char recvbuff[SEND_BUFF_SIZE] = {0};	
	  char param[SEND_BUFF_SIZE] = {0};

};
#endif