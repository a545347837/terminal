//
// Created by caizh on 2020/7/24.
//

#ifndef TERMINAL_COMMON_UTIL_H
#define TERMINAL_COMMON_UTIL_H

#include <string>
#include <fstream>
class CommonUtil{
public:
    /**
     * 从文件中获取内容
     * @param filePath
     * @return
     */
    static std::string getContentFromFile(std::string filePath);
};



#endif //TERMINAL_COMMON_UTIL_H
