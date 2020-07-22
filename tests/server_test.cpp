//
// Created by caizh on 2020/7/22.
//
#include <gtest/gtest.h>
#include "server.h"

TEST(TestCase, test_split_params_success){
    string *op = nullptr;
    string *username = nullptr;
    string *password = nullptr;
    char params[SEND_BUFF_SIZE] = "1&caizh&123456";
    EXPECT_EQ(0,Server::split_params(params, &op, &username, &password));
}

TEST(TestCase, test_split_params_fail){
    string *op = nullptr;
    string *username = nullptr;
    string *password = nullptr;
    char params[SEND_BUFF_SIZE] = "1&caizh";
    EXPECT_EQ(-1,Server::split_params(params, &op, &username, &password));
}

GTEST_API_ int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

