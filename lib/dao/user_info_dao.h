//
// Created by caizh on 2020/7/26.
//

#ifndef TERMINAL_USER_INFO_DAO_H
#define TERMINAL_USER_INFO_DAO_H

#include "lib/util/db.h"
#include "lib/domain/user_info.hpp"
class UserInfoDao{
public:
    /**
     * 初始化用户表操作工具
     * @param dbStr
     */
    UserInfoDao(Db *dbStr);
    ~UserInfoDao();

    /**
    * 根据用户名寻找用户
    * @param dbStr
    */
    UserInfoDO findByUsername(string username);

    /**
    * 保存用户到数据库表中
    * @param dbStr
    */
    void save(UserInfoDO userInfoDo);

private:
    Db *db;
    // 根据用户名查找用户
    const char *const QUERY_USER_SQL ="select * from user_info where username='%s'";

    //插入用户信息
    const char *const INSERT_USER_SQL = "insert into user_info(username,password) values('%s','%s')";
};
#endif //TERMINAL_USER_INFO_DAO_H
