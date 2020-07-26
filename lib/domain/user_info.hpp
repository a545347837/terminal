//
// Created by caizh on 2020/7/26.
//

#ifndef TERMINAL_USER_INFO_HPP
#define TERMINAL_USER_INFO_HPP

/**
 * 数据库映射对象
 */
class UserInfoDO{
public:
    int getId() const {
        return id;
    }

    void setId(int id) {
        UserInfoDO::id = id;
    }

    const string &getUsername() const {
        return username;
    }

    void setUsername(const string &username) {
        UserInfoDO::username = username;
    }

    const string &getPassword() const {
        return password;
    }

    void setPassword(const string &password) {
        UserInfoDO::password = password;
    }

private:
    /**
     * 用户id
     */
    int id;
    /**
     * 用户名
     */
    string username;
    /**
     * 密码
     */
    string password;


};


#endif //TERMINAL_USER_INFO_HPP
