//
// Created by caizh on 2020/7/23.
//
#include <gtest/gtest.h>

/**
 * 运行所有测试用例
 * @param argc
 * @param argv
 * @return
 */
GTEST_API_ int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}