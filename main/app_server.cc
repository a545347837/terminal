//
// Created by caizh on 2020/7/22.
//
#include "lib/util/common_util.h"
#include "lib/util/db.h"
#include "lib/dao/user_info_dao.h"
#include "lib/service/terminal_service_impl.hpp"
#include <grpcpp/grpcpp.h>
#include <grpc++/security/credentials.h>
/**
 * 终端RPC服务端
 */

int main(int argc, char** argv) {

    //
    Db *db = new Db("db",3306,"root","545347837","test");
    UserInfoDao *userInfoDao = new UserInfoDao(db);
    TerminalServiceImpl terminalServiceImpl(userInfoDao);

    std::string server_address("0.0.0.0:50051");
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

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address,creds);
    builder.RegisterService(&terminalServiceImpl);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
    return 0;
}
