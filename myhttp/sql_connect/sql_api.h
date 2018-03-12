#ifndef _SQL_
#define _SQL_

#include <iostream>
#include <mysql.h>
#include <string>
#include <stdlib.h>
#include <string.h>

using namespace std;
class sqlApi{
public:
	sqlApi(string& _h,int port,string& _u,string& _p,string& _db);
	~sqlApi();
	int insert(const string& _name,const string& _sex,const string& age,const string& _hobby,const string& school );
	int select();
private:
	MYSQL* conn;
	MYSQL* res;
	string host;
	string user;
	string passward;
	string db;
	int port;
};

sqlApi::sqlApi(const string& _h ,\
				int _port ,\
				const string& _u = "root",\
				const string& _p = "",\
				const string& _db = "forhttp")
{
	host = _h;
	port = _port;
	user = _u;
	password = _p;
	db = _db;
	conn = mysql_init(NULL);
	res = NULL;
}

sqlApi::int connect()
{
	if(mysql_real_connect(conn,host.c_str(),user.c_str(),"",db.c_str(),port,NULL,0))
	{
		cout<<"connect success!"<<endl;
		
	}else{
		cout<<"connect error"<<endl;
	}
}


//这里的传参依赖于具体业务，具体根据数据库中的表项，具体列具体传入
sqlApi::int insert(const string& _name,const string& _sex,const string& age,const string& _hobby,const string& school )
{
	// 插入的接口调用
	string sqlstr = "INSERT INTO Linux3 (name,sex,age,hobby,school) VALUES ('";
	
	sqlstr += _name ,sqlstr +="','";
	sqlstr += _sex ,sqlstr +="','";
	sqlstr += _age ,sqlstr +="','";
	sqlstr += _hobby ,sqlstr +="','";
	sqlstr += _school ,sqlstr +="','";   sqlstr +="')";
	
	int ret = mysql_query(conn,sqlstr.c_str())
	if (ret != 0){
		cout<<"insert error!"<<endl;
	}
}


sqlApi::int select()
{
	string sql = "SELECT * FROM Linux3";
	if(mysql_query(conn,sql.c_str()) == 0)
	{
		res = mysql_store_result(conn);
		if(res){
			int nums = mysql_num_rows(res);
			int cols = mysql_num_fields(res);
			cout<<"nums:"<<nums<<endl;
			cout<<"cols:"<<cols<<endl;
			
			MYSQL_FIELD *fd;
			for(;fd = mysql_fetch_failed(res);)
			{
				cout<<fd->name<<"  ";
			}
			cout<<endl;
			
			int i = 0;
			int j = 0;
			for(;i < nums ;){
				MTSQL_ROW row_res = mysql_fetch_row(res);
				j = 0;
				for(;i < cols;j++){
					cout<<row_res[j]<<endl;
				}
				cout<<endl;
			}
			cout<<endl;
		}
		
		
		
	}
}
sqlApi::~sqlApi()
{
	mysql_close(conn);
}

#endif