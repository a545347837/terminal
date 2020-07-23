//
// Created by caizh on 2020/7/22.
//
#include "util.h"

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
        }else if(!terminalUtil -> checkLogin(request->password(),queryPwd)){
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
        string query_sql(QUERY_USER_SQL);
        query_sql.insert(query_sql.size() - 1,string(username));
        vector<map<string,string>> result = db->query(query_sql);
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
        string encryptPwd = md5(password);
        string insert_sql(INSERT_USER_SQL);
        insert_sql.insert(insert_sql.size() - 5,username);
        insert_sql.insert(insert_sql.size() - 2,encryptPwd);
        db->query(insert_sql);
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
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
    return 0;
}
