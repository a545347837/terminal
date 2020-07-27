//
// Created by root on 2020/7/27.
//
#include "gtest/gtest.h"
#include "lib/util/util.h"

TEST(TerminalUtil,test_checkParam){
    UserRequest userRequest;
    ASSERT_FALSE(TerminalUtil::checkParam(&userRequest));
    userRequest.set_username("123");
    ASSERT_FALSE(TerminalUtil::checkParam(&userRequest));
    userRequest.set_password("123");
    ASSERT_TRUE(TerminalUtil::checkParam(&userRequest));
}

TEST(TerminalUtil,test_validPassword){
    std::string hashPws = TerminalUtil::hashPassword("123");
    ASSERT_TRUE(TerminalUtil::validPassword("123",hashPws));
    ASSERT_FALSE(TerminalUtil::validPassword("456",hashPws));
}

TEST(TerminalUtil,test_hashPassword){
    std::string hashPws1 = TerminalUtil::hashPassword("123");
    std::string hashPws2 = TerminalUtil::hashPassword("123");
    ASSERT_STRNE(hashPws1.c_str(),hashPws2.c_str());
}
