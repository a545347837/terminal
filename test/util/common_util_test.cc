//
// Created by root on 2020/7/27.
//
#include "gtest/gtest.h"
#include "lib/util/common_util.h"

TEST(CommonUtil,test_getContentFromFile){
    int tick = (int)(std::clock());
    char filePath[128];
    char content[] = "123";
    sprintf(filePath,"/tmp/%d",tick);
    std::ofstream fonstream(filePath);
    fonstream.write(content,strlen(content));
    fonstream.close();
    std::string contentStr = CommonUtil::getContentFromFile(filePath);
    remove(filePath);
    EXPECT_STREQ(contentStr.c_str(),content);

}
