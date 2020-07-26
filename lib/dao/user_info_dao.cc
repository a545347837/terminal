//
// Created by caizh on 2020/7/26.
//
#include "user_info_dao.h"
/**
     * 初始化用户表操作工具
     * @param dbStr
     */
UserInfoDao::UserInfoDao(Db dbStr){
    this->db = dbStr;
}
UserInfoDao::~UserInfoDao(){
    delete db;
}

/**
* 根据用户名寻找用户
* @param dbStr
*/
UserInfoDO UserInfoDao::findByUsername(string username){
    char sql[256];
    sprintf(sql,UserInfoDao::QUERY_USER_SQL,username.c_str());
    vector<map<string,string>> result = db->query(string(sql));
    if(result.empty()){
        return  UserInfoDO::EMPTY_OBJ;
    }
    UserInfoDO userInfoDo();
    return result.front()["password"];
}

/**
* 保存用户到数据库表中
* @param dbStr
*/
void UserInfoDao::save(UserInfoDO userInfoDo){

}
