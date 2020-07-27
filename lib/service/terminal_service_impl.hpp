//
// Created by root on 2020/7/27.
//
#include <string>
#include <unistd.h>
#include <iostream>
#include <grpcpp/grpcpp.h>
#include <grpc++/security/credentials.h>
#include "lib/dao/user_info_dao.h"
#include "lib/domain/user_info.hpp"
#include "lib/facade/terminal.grpc.pb.h"
#include "connection_pool.h"
#include "lib/util/util.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;
using terminal::UserRequest;
using terminal::Result;
using terminal::TerminalService;
using terminal::UserRequest;
using terminal::Result;

#define INVALID_USERNAME_MSG "用户不存在"
#define EXIST_USERNAME_MSG "用户名已存在"
#define INVALID_PASSWORD_MSG "密码错误"
#define INVALID_PARAM_MSG "用户或密码格式有误"

/**
 * 终端RPC服务端
 */
class TerminalServiceImpl final : public TerminalService::Service {

        public:
        TerminalServiceImpl(UserInfoDao *userInfoDao){
            this->userInfoDao = userInfoDao;
            this->connectionPool = new ConnectionPool();
        }

        ~TerminalServiceImpl(){
            delete userInfoDao;
            delete connectionPool;
        }
        protected:
        /**
           登录
        */
        Status login(ServerContext* context, const UserRequest* request,
        ServerWriter<terminal::Result>* writer) override {
            Result result;
            if(!TerminalUtil::checkParam(request)){
                result.set_message(INVALID_PARAM_MSG);
                writer->Write(result);
                return Status::OK;
            }
            result.set_message(EMPTY);

            string username = request->username();
            UserInfoDO userInfoDo = userInfoDao->findByUsername(username);
            if(userInfoDo.getId() == -1){
                result.set_message(INVALID_USERNAME_MSG);
            }else if(!TerminalUtil::validPassword(request->password(),userInfoDo.getPassword())){
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
            if(!TerminalUtil::checkParam(request)){
                result -> set_message(INVALID_PARAM_MSG);
                return Status::OK;
            }

            UserInfoDO userInfoDo = userInfoDao->findByUsername(request->username());
            if(userInfoDo.getId() != -1){
                result -> set_message(EXIST_USERNAME_MSG);
                return Status::OK;
            }

            UserInfoDO saveObj;
            saveObj.setUsername(request->username());
            saveObj.setPassword(TerminalUtil::hashPassword(request->password()));
            userInfoDao->save(saveObj);
            result->set_message("注册成功");
            return Status::OK;
        }

        /**
         * 通过grpc的stream与客户端保持连接
         * @param writer
         */
        void keepAlive( ServerWriter<terminal::Result>* writer,string username,Result &result){
            // 获取连接句柄
            int fd = connectionPool -> submitConnection(username);
            try{
                // 每次循环时需要通过句柄判断连接是否有效
                while(connectionPool->isAlived(username,fd)){
                    result.set_message("心跳中...");
                    if(!writer->Write(result)){
                        cout << "用户(" << username << ")已主动断开连接" << endl;
                        connectionPool->removeConnection(username,fd);
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

        UserInfoDao *userInfoDao;

        ConnectionPool *connectionPool;
};
