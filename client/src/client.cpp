#include "client.h"
Client::Client(){
}

Client::~Client(){
	close(sockfd);
	delete &ser;
	delete &cli;
}

void Client::start_connect(char* host,unsigned int listen_port){
	memset(&ser,0,sizeof(ser));
    ser.sin_family=AF_INET;
    ser.sin_port=htons(listen_port);
    ser.sin_addr.s_addr=inet_addr(host);
	
	while(1){
		int sockfd = socket(AF_INET,SOCK_STREAM,0);
		if(sockfd == -1){
			printf("连接创建失败\n");
			return;
		}
		int res=connect(sockfd,(struct sockaddr*)&ser,sizeof(ser));
		if(res==-1){
			printf("连接服务器失败，请检查服务器是否在运行\n");
			return;
		}

		printf("服务器连接成功\n");

        string op;
		while(1){
            printf("请选择操作，1=登录，2=注册：");
            getline(cin,op);
            if(op.compare("1") != 0 && op.compare("2") != 0){
                printf("输入操作错误\n");
                continue;
            }
            break;
		}

        // 获取用户的账户和密码
		printf("请输入用户名:");
		string username;
		getline(cin,username);
		printf("请输入密码:");
		string password;
		getline(cin,password);

		string params(op);
        const char *paramsChar = params.append("&").append(username).append("&").append(password).c_str();
		printf("即将发送的参数:%s\n",paramsChar);
		send(sockfd,paramsChar,strlen(paramsChar),0);
			
		recv(sockfd,recvbuff,sizeof(recvbuff),0);
		if(strcmp(recvbuff,"success") == 0){
			printf("登录成功！\n");
			while(1){
				memset(recvbuff,0,sizeof(recvbuff));
				int n = recv(sockfd,recvbuff,sizeof(recvbuff),0);
				if(n <= 0){
					printf("该用户已在其它设备登录或服务器已关闭，即将退出登录\n");
					close(sockfd);
					break;
				}
				printf("接收到服务器的消息:%s\n",recvbuff);
			}
		}else{
			printf("请求失败,原因：%s\n",recvbuff);
			close(sockfd);
		}
	}
}



