//
// Created by caizh on 2020/7/26.
//
#include "user_info_dao.h"

static UserInfoDO nullUserInfoDo;
/**
     * 初始化用户表操作工具
     * @param dbStr
     */
UserInfoDao::UserInfoDao(Db *dbStr){
    nullUserInfoDo.setId(-1);
    this-> db = dbStr;
}
UserInfoDao::~UserInfoDao(){
    delete db;
}

/**
* 根据用户名寻找用户
* @param dbStr
*/
UserInfoDO UserInfoDao::findByUsername(string username){
    vector<map<string,string>> result = db->query(string(QUERY_USER_SQL),1,username.c_str());
    if(result.empty()){
        return nullUserInfoDo;
    }
    UserInfoDO userInfoDo;

    userInfoDo.setUsername(result.front()["username"]);
    userInfoDo.setPassword(result.front()["password"]);
    return userInfoDo;
}

/**
* 保存用户到数据库表中
* @param dbStr
*/
void UserInfoDao::save(UserInfoDO userInfoDo){
    db->query(string(INSERT_USER_SQL),2,userInfoDo.getUsername().c_str(),userInfoDo.getPassword().c_str());
}
