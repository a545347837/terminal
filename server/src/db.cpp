#include"db.h"

Db::Db(const char* host,const unsigned int port,const char* username,const char* password,const char* db_name){
   // 1、初始化句柄
   mysql_init(&mysql);
   
   // 2、创建数据库连接
   if(!mysql_real_connect(&mysql ,host ,username, password, db_name,port,NULL,0)){
      cout<<"Error:"<<mysql_error(&mysql)<<endl;
   }else{
      cout<<"connect db success" << endl;
   }
}

Db::~Db(){
   mysql_close(&mysql);
   cout<<"close db success"<<endl;
}

vector<map<string,string>> Db::query(string sql){
   vector<map<string,string>> result;
   cout<<"query sql:"<<sql<<endl;
   // 设置编码格式
   mysql_query(&mysql, "set names gbk"); 
   
   // 1、查询
   if(mysql_query(&mysql,sql.c_str()) != 0){
     cout<<"query'"<<sql<<"',failed,reason:"<<mysql_error(&mysql)<<endl;
     return result;
   }
   
   // 2、将查询的结果存到对象中
   res = mysql_store_result(&mysql);
   if(!res){
     cout<<"get result failed,reason:"<<mysql_error(&mysql)<<endl;
     return result;
   }
   
   // 3、获取当前查到的字段个数
   int col_num = mysql_num_fields(res);
   vector<string> field; 

   // 4、将查询结果中的字段名存下来
   int i = 0;
   for( ; i < col_num ; i ++){
       field.push_back(mysql_fetch_field(res)->name);
   }
   
   // 5、循环将每一行数据以key-value的方式存到map中
   while (column = mysql_fetch_row(res)){
      map<string,string> values;
      for(i = 0 ;i < col_num;i++){
		values.insert(pair<string, string>(field[i], column[i]));
      }
      result.push_back(values);
    }
	
	// 6、执行结束后需要清空缓存
    mysql_free_result(res);
    return result;
}