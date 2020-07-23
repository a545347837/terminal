#include <gtest/gtest.h>
#include "util.h"

TerminalUtil terminalUtil;

/**
 * 场景：用户在一个设备进行登录
 */
TEST(TestCase, test_submitConnection_1){
    EXPECT_EQ(0,terminalUtil.submitConnection("123"));
}

/**
* 场景：用户在另一个设备进行登录
*/
TEST(TestCase, test_submitConnection_2){
    EXPECT_EQ(1,terminalUtil.submitConnection("123"));
}

/**
* 场景：用户下线
*/
TEST(TestCase, test_removeConnection){
    terminalUtil.removeConnection("123",1);
}

/**
* 场景：用户处在连接中
*/
TEST(TestCase, test_isConnected){
    int fd = terminalUtil.submitConnection("123");
    EXPECT_EQ(true,terminalUtil.isConnected("123",fd));
}

/**
* 场景：用户断开连接
*/
TEST(TestCase, test_isConnected_remove){
    terminalUtil.removeConnection("123",1);
    EXPECT_EQ(false,terminalUtil.isConnected("123",1));
}

/**
* 场景：正常的参数请求
*/
TEST(TestCase, test_checkParam_success){
    UserRequest userRequest;
    userRequest.set_username("123");
    userRequest.set_password("123");
    EXPECT_EQ(true,terminalUtil.checkParam(&userRequest));
}

/**
* 场景：异常的参数请求
*/
TEST(TestCase, test_checkParam_fail){
    UserRequest userRequest;
    EXPECT_EQ(false,terminalUtil.checkParam(&userRequest));
}
/**
* 场景：用户密码正确
*/
TEST(TestCase, test_checkLogin_success){
    EXPECT_EQ(true,terminalUtil.checkLogin("123","202cb962ac59075b964b07152d234b70"));
}

/**
* 场景：用户密码错误
*/
TEST(TestCase, test_checkLogin_fail){
    EXPECT_EQ(false,terminalUtil.checkLogin("123","123"));
}

