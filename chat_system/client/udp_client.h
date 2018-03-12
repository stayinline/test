#ifndef _UDP_CLIENT_H_
#define _UDP_CLIENT_H_

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>

class udp_client{
	public:
		udp_client(const std::string &_ip, const int &_port);
		int initClient();
		int recvData(std::string &outString);
		int sendData(const std::string &inString);
		~udp_client();
	private:
		udp_client(const udp_client &);

		struct sockaddr_in server;// 这里将一个server对象作为成员变量，直接将数据发给这个变量
		int sock;

};

#endif






