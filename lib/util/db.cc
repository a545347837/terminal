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
   delete res;
   cout<<"close db success"<<endl;
}

vector<map<string,string>> Db::query(string sql,...){
   cout<<"query db request:"<<sql<<endl;
   vector<map<string,string>> result;
   // 设置编码格式
   mysql_query(&mysql, "set names gbk");

   vercto<string> sqlParams;
   va_list ap;
   va_start(ap, cnt);
   for (int i = 0; i < cnt; ++i) {
       sqlParams.push_back(va_arg(ap, string));
   }
   va_end(ap);
   MYSQL_STMT *stmt = mysql_stmt_init(conn); //创建MYSQL_STMT句柄
   if(mysql_stmt_prepare(stmt, sql, strlen(sql))){
       cout<<"mysql_stmt_prepare::"<<mysql_error(&mysql)<<endl;
        return -1;

    }
    MYSQL_BIND params[sqlParams.size()];
    memset(params, 0, sizeof(params));
    for(int i =0 ;i<sqlParams.size();i++){
        params[i].buffer_type = MYSQL_TYPE_STRING;
        params[i].buffer = sqlParams[i];
        params[i].buffer_length = strlen(name);
    }
    mysql_stmt_bind_param(stmt, params);
    mysql_stmt_execute(stmt);           //执行与语句句柄相关的预处理
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
    mysql_stmt_close(stmt);
    return result;
}