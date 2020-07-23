//
// Created by caizh on 2020/7/22.
//
#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "terminal.grpc.pb.h"
#endif

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using terminal::UserRequest;
using terminal::Result;
using terminal::TerminalService;
using std::cout;
using std::endl;
using std::cin;
using std::string;
#define EXIT "0"
#define LOGIN "1"
#define REGISTER "2"

/**
 * 客户端
 */
class AppClient {
public:
    AppClient(std::shared_ptr<Channel> channel): stub_(TerminalService::NewStub(channel)) {}

    /**
     * 登录操作
     * @param username
     * @param password
     */
    void login(const std::string& username,const std::string& password) {
        UserRequest request;
        request.set_username(username);
        request.set_password(password);
        ClientContext context;
        std::unique_ptr<::grpc::ClientReader<::terminal::Result>> stream_result = stub_->login(&context, request);
        Result result;
        stream_result->Read(&result);
        if(result.message().empty()){
            cout<<"服务器异常，稍后再试"<<endl;
            return;
        }
        printResult(result);

        // 通过stream机制保持与服务器的长连接
        while(stream_result->Read(&result)){
            printResult(result);
            processOp(result.code(),result.message());
        }
        cout<<"已断开与服务器的连接"<<endl;
    }

    /**
     * 注册操作
     * @param username
     * @param password
     */
    void registerUser(const std::string& username, const std::string& password) {
        UserRequest request;
        request.set_username(username);
        request.set_password(password);
        ClientContext context;
        Result result;
        Status status = stub_->registerUser(&context, request,&result);
        if(status.ok()){
            printResult(result);
        }else{
            cout<<"服务器异常，稍后再试"<<endl;
        }
    }

    /**
   * 输入操作
   * @return
   */
    string getOp(){
        string op;
        cout<<"请选择操作,1=登录,2=注册,0=退出:";
        getline(cin,op);
        return op;
    }

    /**
   * 输入用户名
   * @return
   */
    string getUsername(){
        cout<<"请输入用户名：";
        string username;
        getline(cin,username);
        return username;
    }

    /**
     * 输入密码
     * @return
     */
    string getPassword(){
        cout<<"请输入密码：";
        string password;
        getline(cin,password);
        return password;
    }

    /**
     * 打印服务器返回的日志
     * @param result
     */
    void printResult(Result &result){
        cout<<"接收到服务器消息:"<<result.message()<<endl;
    }

    /**
     * 处理服务器发出的指令
     */
    void processOp(int code,string message){

    }
private:
    std::unique_ptr<TerminalService::Stub> stub_;
};

int main(int argc, char** argv) {
    AppClient appClient(grpc::CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials()));
    cout<<"客户端已就绪"<<endl;
    while(1){
        string op = appClient.getOp();
        if(op.compare(EXIT) == 0){
            cout<<"感谢您的使用"<<endl;
            return 0;
        }else if(op.compare(LOGIN) != 0 && op.compare(REGISTER) != 0){
            cout<<"不支持该操作"<<endl;
            continue;;
        }
        string username = appClient.getUsername();
        string password = appClient.getPassword();
        if(op.compare(LOGIN) == 0){
            appClient.login(username,password);
        }else{
            appClient.registerUser(username, password);
        }
    }
}


