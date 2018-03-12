#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
void test()
{
	FILE* f = fopen("../test.html","r");
	char c = 0;
	while(!feof(f)){
		c = fgetc(f);
		printf("%c",c);
	}
	fclose(f);
		
}
void mymath(char* data)
{
//	printf("%s\n",data);
	char *start=data;
	char *myargv[3];
	int i=0;
	while(*start){
		if(*start=='='){
			myargv[i++]=start+1;
			start+=1;
		}
		if(*start=='&'){
			*start='\0';
		}
		start++;
	}
	myargv[i]=0;
	int fdata=atoi(myargv[0]);
	int ldata=atoi(myargv[1]);
//	printf("data1= %d,data2= %d\n",fdata,ldata);
//	printf("<html><h1>math_cgi</h1></html>");
	printf("<html>");
	printf("<h1>%d + %d = %d\n</h1>",fdata,ldata,fdata+ldata);
	printf("<h1>%d - %d = %d\n</h1>",fdata,ldata,fdata-ldata);
	printf("<h1>%d * %d = %d\n</h1>",fdata,ldata,fdata*ldata);
	printf("<h1>%d / %d = %d\n</h1>",fdata,ldata,fdata/ldata);
	printf("<h1>%d %% %d = %d\n</h1>",fdata,ldata,fdata%ldata);
	char* s = "hello cgi"; 

	printf("<input type=text value=\"%s\"  id=\"NameText\"\n",s); 
	printf("</html>");
}
int main()
{
//	printf("this is math_cgi\n");
    char* method=getenv("METHOD");//根据环境变量名取得环境变量内容
 	char* query_string=NULL;
 	char buf[1024];
//	printf("math_cgi: method:%s\n",method);
 	if(method){
 		if(strcasecmp(method,"GET")==0){
 			query_string = getenv("QUERY_STRING");
 			strcpy(buf,getenv("QUERY_STRING"));
 			query_string=buf;
//			printf("math_cgi_query_string:%s\n",query_string);
 		}
 		else{
 			char* len=getenv("CONTENT_LENGTH");
			//printf("math_cgi_len:%s\n",len);
			int cl=atoi(len);
 			int i=0;
 			for(;i<cl;i++){
 				read(0,buf+i,1);
 			}
 			buf[i]=0;
 			query_string=buf;
 		}
 	}
//	printf("math_cgi_query_string:%s\n",query_string);
 	if(query_string){
		mymath(query_string);
	//	test();
 	}
	return 0;
}


