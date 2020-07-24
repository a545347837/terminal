//
// Created by caizh on 2020/7/24.
//
#include "common/common_util.h"

std::string CommonUtil::getContentFromFile(std::string filePath){
    std::ifstream finstream(filePath.c_str());
    std::string contents;
    contents.assign((std::istreambuf_iterator<char>(finstream)),
                    std::istreambuf_iterator<char>());
    finstream.close();
    return contents;
}