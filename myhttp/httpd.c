#include"httpd.h"

const char* status_line="HTTP/1.0 200 OK\r\n";
const char *type_line="Content_Type:text/html;charset=utf-8\r\n";
const char* blank_line="\r\n";

int startup(const char *ip,int port)
{
	int sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock<0)
	{
		perror("socket");
		exit(2);
	}
	int opt=1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	struct sockaddr_in local;
	local.sin_family=AF_INET;
	local.sin_port=htons(port);
	local.sin_addr.s_addr=inet_addr(ip);
	if((bind(sock,(struct sockaddr*)&local,sizeof(local)))<0)
	{
		perror("bind");
		exit(3);
	}
	if(listen(sock,BACKLOG)<0){
		perror("listen");
		exit(4);
	}
	return sock;
}

// ret > 1 line!= '\0'   ret =1&&line=='\n'       ret < 0 && line == '\0'
int get_line(int sock,char* buff,int len)
{
	char c='\0';
	int i=0;
	while(i<len-1 && c!='\n'){
		ssize_t s=recv(sock,&c,1,0);
		if(s>0){
			if(c=='\r'){// \r \n \r\n
				recv(sock,&c,1,MSG_PEEK);
				if(c=='\n'){
					recv(sock,&c,1,0);
				}else{
					c='\n';
				}
			}
			buff[i++]=c;
		}
		else{
			break;
		}
	}
	buff[i]=0;
	return i;
}
void show_404(int sock)
{
	const char* error_line="HTTP/1.0 404 NOT Found\r\n";
//	send(sock,status_line,strlen(status_line),0);
	send(sock,error_line,strlen(error_line),0);
	send(sock,type_line,strlen(type_line),0);
	send(sock,blank_line,strlen(blank_line),0);
}
void error_response(int sock,int err_code)
{
	switch(err_code){
		case 404:
			show_404(sock);
			break;
		case 500:
			break;
		case 503:
			break;
		default:
			break;
	}
}
int echo_www(int sock,const char *path,int st_size)
{
	int fd=open(path,O_RDONLY);
	if(fd<0){
		return 404;
	}
	send(sock,status_line,strlen(status_line),0);
//	const char *status_line="HTTP/1.0 200 OK\r\n";
//	send(fd,status_line,strlen(status_line),0);
	send(fd,type_line,strlen(type_line),0);
//	const char* blank_line="\r\n";
//	send(fd,blank_line,strlen(blank_line),0);
	send(sock,blank_line,strlen(blank_line),0);
	sendfile(sock,fd,NULL,st_size);
	close(fd);
	return 200;
}
void handler_header(int sock)
{
	char buf[SIZE];
	int ret=0;
	do{
		ret=get_line(sock,buf,SIZE);
	}while(ret>0 &&strcmp(buf,"\n")!=0);
}
void exe_cgi(int sock,const char* method,const char* query_string,const char *path)
{
	char buf[SIZE];
	int content_len=-1;
	char method_env[SIZE];
	char query_string_env[SIZE];
	char content_len_env[SIZE];
	if(strcasecmp(method,"GET")==0){
		handler_header(sock);
	}else{//POST
		printf("POST WAY\n");
		int ret=-1;
		do{
			ret=get_line(sock,buf,SIZE);
			printf("buf:%s\n",buf);
			char* r = strstr(buf,"Content-Length:");
			printf("strstr return val:%s\n",r);
			if(ret >0 && r != NULL){
				content_len=atoi(&buf[16]);
			}
		}while(ret>0 && strcmp(buf,"\n")!=0);
		
		printf("exe_cgi:content_len=%d\n",content_len);

		if(content_len<0){
			return;
		}
	}
	//get post的参数交给path
	int input[2];
	int output[2];
	if(pipe(input)<0){
		return ;//优化
	}
	if(pipe(output)<0){
		return;
	}

	send(sock,status_line,strlen(status_line),0);
	send(sock,type_line,strlen(type_line),0);
	send(sock,blank_line,strlen(blank_line),0);
	pid_t id=fork();
	printf("fork return value:%d\n",id);
	if(id<0){
		return;
	}else if(id==0){//child
		close(input[1]);
		close(output[0]);
		dup2(input[0],0);
		dup2(output[1],1);
		sprintf(method_env,"METHOD=%s",method);
		putenv(method_env);//设置环境变量
		
	//printf("exe_cgi_method_env:%s\n",method);
	//	cgi =0 ;
		if(strcasecmp(method,"GET")==0){
			sprintf(query_string_env,"QUERY_STRING=%s",query_string);
			putenv(query_string_env);
	//		printf("exe_cgi_query_string:%s\n",query_string);
		}else{
			sprintf(content_len_env,"CONTENT_LENGTH=%d",content_len);
			putenv(content_len_env);
		}
//		printf("path:%s\n",path);
		execl(path,path,NULL);//执行cgi
		exit(1);
	}else{//father
		close(input[0]);
		close(output[1]);
		int i =0;
		char c;
		if(strcasecmp(method,"POST")==0){
			for(i;i<content_len;i++){
				recv(sock,&c,1,0);//从sock中读，往pipe中写
				write(input[1],&c,1);
			}
		}
		//read 从子进程中读，读出来的直接发到客户端
		c = '\0';
		
		while(read(output[0],&c,1)>0){
			send(sock,&c,1,0);
		}
		//send(sock,"f",1,0);
		//printf("father proc\n");
		waitpid(id,NULL,0);
		close(input[1]);
		close(output[0]);
	}
}

void *handler_request(void * arg)
{
//	printf("haha\n");
	int sock=(int)arg;
	char buff[SIZE];
 
	char method[SIZE/10];
	char url[SIZE];
	int err_code=200;
	int i,j;
	int cgi=0;
	char* query_string=NULL;//指向参数部分，？后面部分
	char path[SIZE];
#ifdef _STDOUT_
	char buf[4096];
	ssize_t s=read(sock,buf,sizeof(buf)-1);
	if(s<0){
		perror("read");
		return;
	}
	buf[s]=0;
	printf("%s",buf);
#else
	//printf("%s,%d,%s\n",method,cgi,query_string);
	if(get_line(sock,buff,sizeof(buff))<=0){
		err_code=404;
		goto end;
	}
	printf("head line:%s\n",buff);
	//GET /HTTP/1.0
	i=0;
	j=0;
	while(i<sizeof(method)-1 && j<sizeof(buff)&& !isspace(buff[j])){
		method[i]=buff[j];
		i++,j++;
	}
	method[i]='\0';
	//j是空格
	while(j<sizeof(buff)&& isspace(buff[j])){
		j++;
	}
	//j一定不是空格
	i=0;
	while(i<sizeof(url)-1 && j<sizeof(buff) && !isspace(buff[j])){
		url[i]=buff[j];
		i++,j++;
	}
	url[i]=0;
	if(strcasecmp(method,"GET") && strcasecmp(method,"POST")){
		err_code=404;
		goto end;
	}
	printf("after deal with the head line:%s,%s,  cgi:%d\n",method,url,cgi);
	if(strcasecmp(method,"POST")==0){
		cgi=1;
		printf("POST:cgi = %d\n",cgi);
	}else{//get
		query_string=url;
	//	printf("GETWAY query_string:%s,cgi:%d\n",query_string,cgi);
		while(*query_string){
			if(*query_string=='?'){
				cgi=1;
				*query_string='\0';
				query_string++;
				break;
			}
			query_string++;
		}
	}
	sprintf(path,"wwwroot%s",url);
	printf("path:%s\n",path);
	
	if(path[strlen(path)-1]=='/'){
		strcat(path,"index.html");
	}
	struct stat st;
	if(stat(path,&st)<0){
		err_code=404;
		goto end;
	}else{
		if(S_ISDIR(st.st_mode)){//目录文件,绝对不是以 /  结尾
			strcat(path,"/index.html");
			printf("path is dir ,after cat index.html,the path is:%s\n",path);
		}else if((st.st_mode & S_IXGRP) || (st.st_mode & S_IXUSR)|| (st.st_mode & S_IXOTH)){//可执行文件
	     	printf("path is exe ,path:%s,\n",path);
			cgi=1;
		}else{}
		printf("path:%s,cgi:%d\n",path,cgi);
		//知道method,get(query_string),是否cgi,path
		if(cgi){
			//CGI方式
			exe_cgi(sock,method,query_string,path);
		}else{
			//非CGI方式，get 方法 中的直接请求方式，
			printf("not CGI way : method：%s,cgi：%d，url:%s,path:%s,query_string:%s\n",method,cgi,url,path,query_string);
			handler_header(sock);
			err_code = echo_www(sock,path,st.st_size); //
		}
	}
#endif
end:

	error_response(sock,err_code);
	close(sock);
	printf("####################################33\n");
	return (void*)0;
}
