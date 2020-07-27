
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

vector<map<string,string>> Db::query(string sql,int count,...){
   cout<<"query db request:"<<sql<<endl;
   vector<map<string,string>> result;
   // 设置编码格式
   mysql_query(&mysql, "set names gbk");

   vector<char*> sqlParams;
   va_list ap;
   va_start(ap, sql);
   for (int i = 0; i < count; ++i) {
       sqlParams.push_back(va_arg(ap, char*));
   }
   va_end(ap);
   MYSQL_STMT *stmt = mysql_stmt_init(&mysql); //创建MYSQL_STMT句柄

   if(mysql_stmt_prepare(stmt, sql.c_str(), sql.length())){
       cout<<"mysql_stmt_prepare::"<<mysql_error(&mysql)<<endl;
        return result;
    }
    MYSQL_BIND params[sqlParams.size()];
    memset(params, 0, sizeof(params));
    for(int i =0 ;i < sqlParams.size();i++){
        params[i].buffer_type = MYSQL_TYPE_STRING;
        params[i].buffer = sqlParams[i];
        params[i].buffer_length = strlen(sqlParams[i]);
    }

    res = mysql_stmt_result_metadata(stmt);
    if(!res){
        cout<<"get result failed,reason:"<<mysql_error(&mysql)<<endl;
        return result;
    }
    // 3、获取当前查到的字段个数
    int col_num = mysql_num_fields(res);
    // 4、将查询结果中的字段名存下来
    vector<char*> field;
    int i = 0;
    MYSQL_BIND resParams[col_num];
    memset(resParams, 0, sizeof(resParams));

    unsigned long length[col_num];
    char strData[col_num][128];
    unsigned long strLength[col_num];

    memset(strData, 0, sizeof(strData));
    memset(strLength, 0, sizeof(strLength));

    for( ; i < col_num ; i ++){
        field.push_back(mysql_fetch_field(res)->name);
        memset(strData[i], 0, sizeof(strData[i]));
        resParams[i].buffer_type = MYSQL_TYPE_STRING;
        resParams[i].buffer = (char *)strData[i];
        resParams[i].buffer_length = 128;
        resParams[i].length = &length[i];
    }
    mysql_stmt_bind_param(stmt, params);
    mysql_stmt_execute(stmt);           //执行与语句句柄相关的预处理
    mysql_stmt_store_result(stmt);
    mysql_stmt_bind_result(stmt,resParams);
   // 5、循环将每一行数据以key-value的方式存到map中
   while (!mysql_stmt_fetch(stmt)){
      map<string,string> values;
      for(i = 0 ;i < col_num;i++){
		values.insert(pair<string, string>(string(field[i]), strData[i]));
      }
      result.push_back(values);
    }
	
	// 6、执行结束后需要清空缓存
    mysql_free_result(res);
    mysql_stmt_close(stmt);
    return result;
}