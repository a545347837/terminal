#include "server.h"
int main(){
    const char host[] = "db";
    const char username[] = "root";
    const char password[] = "545347837";
    const char db_name[]  = "test";
    const unsigned int db_port = 3306;
    const unsigned int listen_port = 10000;

    Server *server = new Server(host,db_port,username,password,db_name);
    server->start_accept(listen_port);
    delete server;
}