#ifndef _HTTPD_H_
#define _HTTPD_H_
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/sendfile.h>
#include<fcntl.h>

#define BACKLOG 10
#define SIZE 1024
int startup(const char* ip,int port);
void * handler_request(void * arg);
int get_line(int sock,char *buff,int size);
int echo_www(int sock,const char *path,int st_size);
void handler_header(int sock);
void error_response(int sock,int err_code);
void exe_cgi(int sock,const char* method,const char* query_string,const char *path);
void print_log(const char* msg);
#endif
