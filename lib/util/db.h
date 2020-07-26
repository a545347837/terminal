#ifndef DB_H
#define DB_H
#include<iostream>
#include<vector>
#include<map>
#include<string>
#include<vector>

#include"mysql/mysql.h"
using namespace std;

/**
* 数据库操作对象类
*/
class Db{
 
  public:
     // 构造函数，传入数据库的配置信息后即可使用
	 Db(const char* host,const unsigned int port,const char* username,const char* password,const char* db_name);
	 
	 // 析构函数，关闭数据库连接等
	 ~Db();
	 
	 // 通用执行sql方法
	 vector<map<string,string>> query(string sql);
	 
  private:
	 // 数据库实例
	 MYSQL mysql;
	 // 结果集
	 MYSQL_RES *res;
	 // 行内容
	 MYSQL_ROW column;
};
#endif