#include "client.h"
int main(){
    Client *client = new Client();
    char host[] = "127.0.0.1";
    const unsigned int listen_port = 10000;
    client->start_connect(host,listen_port);
    delete client;
}