#include "sql_api"
using namespace std;
void insertToDb(const char* string_arg)
{
	char buf[1024];
	//name="dfa"&sex="man"&age="20"
	char* myargv[6];
	strcpy(buf,string_srg);
	char* strat = buf;
	int i = 0;
	while(*start){
		if(*start == '='){
			start++;
			myargv[i++] = start;
			continue;
		}
		if(*start == '&'){
			*start = 0;
		}
		start++;
	}
	myargv[i] = NULL；
	
	sqlApi mydb("127.0.0.1",3306);
	mydb.connect();
	mydb.insert(myargv[0],myargv[1],myargv[2],myargv[3],myargv[4]);
	
}

int main()
{
	char* method=getenv("METHOD");//根据环境变量名取得环境变量内容
	char* query_string=NULL;
	char buf[1024];
	if(method){
		if(strcasecmp(method,"GET")==0){
			//query_string = getenv("QUERY_STRING");
			strcmp(buf,getenv("QUERY_STRING"));
			query_string=buf;
		}else{
			char* len=getenv("CONTENT_LEN");
			int cl=atoi(len);
			int i=0;
			for(;i<cl;i++){
				read(0,buf+i,1);
			}
			buf[i]=0;
			query_string=buf;
		}
	}
	insertToDb(query_string);
}