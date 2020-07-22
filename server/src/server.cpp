//
// Created by caizh on 2020/7/22.
//
#include "server.h"
// init

map<string,int>* connect_container = new map<string,int>();
Db *db = nullptr;

Server::Server(const char* host,const unsigned int port,const char* username,const char* password,const char* db_name){
    db = new Db(host,port,username,password,db_name);
}

Server::~Server(){
    delete db;
    delete connect_container;
    close(sockfd);
}

void Server::start_accept(const unsigned int listen_port){
    // 屏蔽信号量，当服务端对已断开的连接进行send，会产生一个信号导致程序退出，需要屏蔽
    signal(SIGPIPE,SIG_IGN);

    // 创建心跳线程
    create_heartbeat_thread();

    // 初始化服务监听
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in ser,cli;
    memset(&ser,0,sizeof(ser));

    ser.sin_family=AF_INET;
    ser.sin_port=htons(10000);
    ser.sin_addr.s_addr=htonl(INADDR_ANY);

    // 绑定端口
    int res=bind(sockfd,(struct sockaddr*)&ser,sizeof(ser));
    if(res == -1){
        printf("bind socket failed!\n");
        return ;
    }

    // 开启监听
    res = listen(sockfd,5);
    if(res == -1){
        printf("listen socket failed!\n");
        return ;
    }

    while(1){
        socklen_t len = sizeof(cli);
        printf("%s\n","waitting client connection...");

        // 当没有客户端连接时此处会阻塞
        int c = accept(sockfd,(struct sockaddr*)&cli,&len);
        if(c<0){
            perror("error\n");
            continue;
        }

        // 创建新线程处理客户端的连接
        pthread_t connect_handle_thread;
        pthread_create(&connect_handle_thread, NULL, handle, &c);
    }
}

// 心跳函数，用于对客户端发心跳命令
void* Server::heartbeat(void* args){
    printf("开始对客户端进行心跳包检测\n");
    map<string,int>::iterator iter;
    char content[] = "ping";
    while(1){
        for(iter = connect_container->begin();iter != connect_container->end();iter++){
            int res = send(iter->second,content,sizeof(content),0);
            if(res == -1){
                printf("%s已断开连接\n",iter->first.c_str());
                connect_container->erase(iter);
                break;
            }
        }
        sleep(1);
    }
}

// 创建心跳检测线程
void Server::create_heartbeat_thread(){
    pthread_t heartbeat_thread;
    pthread_create(&heartbeat_thread, NULL, heartbeat, NULL);
}

// 当服务端接收到一个客户端连接，会异步调用该方法进行处理
void* Server::handle(void* args){
    printf("%s\n","one client connected,waitting recv...");
    int client = *(int*)args;
    char recvbuf[SEND_BUFF_SIZE] = {0};
    int n = recv(client,recvbuf,sizeof(recvbuf),0);
    if(n<=0){
        printf("客户端已关闭连接\n");
    }else{
        printf("接收到登陆请求:%s\n",recvbuf);
        string *op = nullptr;
        string *username = nullptr;
        string *password = nullptr;

        split_params(recvbuf,&op,&username,&password);

        const char* valid_result = verification(*op,*username,*password);
        if(valid_result != SUCCESS_MSG){
            send(client,valid_result,SEND_BUFF_SIZE,0);
            return nullptr;
        }

        map<string,int>::iterator iter = connect_container->find(*username);
        if (iter != connect_container->end()){
            int old_client = iter->second;
            connect_container->erase(iter);
            close(old_client);
        }
        connect_container->insert(pair<string, int>(*username,client));
        send(client,valid_result,SEND_BUFF_SIZE,0);
    }
    return nullptr;
}

// 账号密码校验
const char* Server::verification(string &op,string &username,string &password){
    string encrypt_pwd = md5(password);
    string query_sql(QUERY_USER_SQL);
    query_sql.insert(query_sql.size() - 1,string(username));

    vector<map<string,string>> result = db->query(query_sql);
    if(op.compare(LOGIN) == 0){
        if(result.empty()){
            return INVALID_USERNAME_MSG;
        }
        map<string,string> columns = result.front();
        string pwd = columns["password"];
        if(pwd.compare(encrypt_pwd) != 0){
            return INVALID_PASSWORD_MSG;
        }
        return SUCCESS_MSG;
    }else if(op.compare(REGISTER) == 0){
        if(!result.empty()){
            return EXIST_USERNAME_MSG;
        }
        string insert_sql(INSERT_USER_SQL);
        insert_sql.insert(insert_sql.size() - 5,string(username));
        insert_sql.insert(insert_sql.size() - 2,string(encrypt_pwd));
        db->query(insert_sql);
        return SUCCESS_MSG;
    }
    return INVALID_PARAM_MSG;
}

// 参数分割
void Server::split_params(char *params,string **op,string **username,string **password){
    const char *sep = "&";

    char *p = strtok(params,sep);
    if(p == nullptr){
        return;
    }
    *op = new string(p);

    p = strtok(nullptr,sep);
    if(p == nullptr){
        return;
    }
    *username = new string(p);

    p = strtok(nullptr,sep);
    if(p == nullptr){
        return;
    }
    *password = new string(p);
}


