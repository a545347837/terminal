//
// Created by caizh on 2020/7/22.
//
#include "lib/util/util.h"
#include "lib/util/common_util.h"

/**
 * 终端RPC服务端
 */
class TerminalServiceImpl final : public TerminalService::Service {

public:
    TerminalServiceImpl(const char* host,const unsigned int port,const char* username,const char* password,const char* db_name){
        db = new Db(host,port,username,password,db_name);
        terminalUtil = new TerminalUtil();
    }

    ~TerminalServiceImpl(){
        delete db;
        delete terminalUtil;
    }
protected:
    /**
       登录
    */
    Status login(ServerContext* context, const UserRequest* request,
                 ServerWriter<terminal::Result>* writer) override {
        Result result;
        if(!terminalUtil -> checkParam(request)){
            result.set_message(INVALID_PARAM_MSG);
            writer->Write(result);
            return Status::OK;
        }
        result.set_message(EMPTY);
        string username = request->username();
        string queryPwd = queryPassword(request->username());
        if(queryPwd.compare(EMPTY) == 0){
            result.set_message(INVALID_USERNAME_MSG);
        }else if(!terminalUtil -> validPassword(request->password(),queryPwd)){
            result.set_message(INVALID_PASSWORD_MSG);
        }
        if(result.message().compare(EMPTY) != 0){
            writer->Write(result);
            return Status::OK;
        }

        result.set_message("登录成功");
        writer->Write(result);
        cout << "用户(" << username << ")登录成功" << endl;
        // 保持住长连接
        keepAlive(writer,username,result);
        return Status::OK;
    }

    /**
        注册
     */
    Status registerUser(ServerContext* context, const UserRequest* request,
                        Result* result) override {
        if(!terminalUtil -> checkParam(request)){
            result -> set_message(INVALID_PARAM_MSG);
            return Status::OK;
        }

        string queryPwd = queryPassword(request -> username());
        if(queryPwd.compare(EMPTY) != 0){
            result -> set_message(EXIST_USERNAME_MSG);
            return Status::OK;
        }

        registerUserInfo(request->username(),request->password());
        result->set_message("注册成功");
        return Status::OK;
    }

    /**
     * 根据用户名查找密码，若查不到则返回空
     * @param username
     * @return
     */
    string queryPassword(string username){
        char sql[256];
        sprintf(sql,QUERY_USER_SQL,username.c_str());
        vector<map<string,string>> result = db->query(string(sql));
        if(result.empty()){
            return EMPTY;
        }
        return result.front()["password"];
    }

    /**
     * 注册用户
     * @param username
     * @param password
     */
    void registerUserInfo(string username,string password){
        string encryptPwd = terminalUtil->hashPassword(password);
        char sql[256];
        sprintf(sql,INSERT_USER_SQL,username.c_str(),encryptPwd.c_str());
        db->query(sql);
    }

    /**
     * 通过grpc的stream与客户端保持连接
     * @param writer
     */
    void keepAlive( ServerWriter<terminal::Result>* writer,string username,Result &result){
        // 获取连接句柄
        int fd = terminalUtil -> submitConnection(username);
        try{
            // 每次循环时需要通过句柄判断连接是否有效
            while(terminalUtil->isConnected(username,fd)){
                result.set_message("心跳中...");
                if(!writer->Write(result)){
                    cout << "用户(" << username << ")已主动断开连接" << endl;
                    terminalUtil->removeConnection(username,fd);
                    return;
                }
                sleep(1);
            }
            result.set_message("您的账号已在其它地方登录");
            writer->Write(result);
        } catch (...) {
            cout<<"出现未知异常"<<endl;
        }
    }

private:
    /**
     * Db指针
     */
    Db *db = nullptr;

    /**
     * 终端工具
     */
    TerminalUtil *terminalUtil = nullptr;
};

int main(int argc, char** argv) {
    std::string server_address("0.0.0.0:50051");
    TerminalServiceImpl service("db",3306,"root","545347837","test");
    auto clientcert = CommonUtil::getContentFromFile(string("/home/config/server/client_self_signed_crt.pem")); // for verifying clients
    auto servercert = CommonUtil::getContentFromFile(string("/home/config/server/server_self_signed_crt.pem"));
    auto serverkey  = CommonUtil::getContentFromFile(string("/home/config/server/server_privatekey.pem"));
    grpc::SslServerCredentialsOptions::PemKeyCertPair pkcp = {
            serverkey.c_str(), servercert.c_str()
    };

    grpc::SslServerCredentialsOptions ssl_opts(GRPC_SSL_REQUEST_AND_REQUIRE_CLIENT_CERTIFICATE_AND_VERIFY);
    ssl_opts.pem_root_certs = clientcert;
    ssl_opts.pem_key_cert_pairs.push_back(pkcp);

    std::shared_ptr<grpc::ServerCredentials> creds;
    creds = grpc::SslServerCredentials(ssl_opts);

    ServerBuilder builder;
    builder.AddListeningPort(server_address,creds);
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
    return 0;
}
