#ifndef _UDP_SERVER_H_
#define _UDP_SERVER_H_

#include <iostream>
#include <string>
#include <map>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <data_pool.h>

class udp_server{
	public:
		udp_server(const std::string &_ip, const int &_port);
		
		int initServer(); 
		//用构造函数传的ip和potr，执行socket()、bind()、 然后等待，在recvData函数中tecvfrom
		
		int recvData(std::string &outString);
		//用outString作为输入参数，将buf中的数据put到data_pool中.
		
		int sendData(struct sockaddr_in &client, socklen_t len, std::string &msg);
		//调用sendto()函数将msg发送到client
		
		int broadcast();
		// 首先将data_pool中的数据取出来(get_data函数)，然后遍历online_user(map保存的)，
		// 将取出来的数据通过sendto（）发送给每一个用户
		
		~udp_server();
		
	private:
		udp_server(const udp_server &);//禁止拷贝构造
		void add_user(struct sockaddr_in &peer, const socklen_t &len);
		void del_user(struct sockaddr_in &peer, const socklen_t &len);

		std::string ip;
		int port;
		int sock;

		std::map<int, struct sockaddr_in> online_user; //用编号和struct sockaddr_in 结构体来标识每一个用户
		data_pool pool;
};

#endif
