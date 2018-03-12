#include "udp_server.h"
#include "data.h"

udp_server::udp_server(const std::string &_ip, const int &_port)
	:ip(_ip)
	,port(_port)
	,sock(-1)
	, pool(128)
{}


int udp_server::initServer()
{
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0){
		std::cerr << "socket error!" << std::endl;
		return -1;
	}

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = inet_addr(ip.c_str());
	
	if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0){
		std::cerr << "bind error!" << std::endl;
		return -2;
	}
}


void udp_server::add_user(struct sockaddr_in &peer, const socklen_t &len)
{
	online_user.insert(std::pair<int, struct sockaddr_in>(peer.sin_addr.s_addr, peer));
}

void udp_server::del_user(struct sockaddr_in &peer, const socklen_t &len)
{
	std::map<int,struct sockaddr_in>::iterator iter = online_user.find(peer.sin_addr.s_addr);
	if(iter != online_user.end()){
		online_user.erase(iter);
	}
}

int udp_server::recvData(std::string &outString)
{
	char buf[1024];
	struct sockaddr_in peer;
	socklen_t len = sizeof(peer);
	ssize_t s = recvfrom(sock, buf, sizeof(buf)-1, 0, (struct sockaddr*)&peer, &len);
	if(s > 0)
	{
		buf[s] = 0;
		outString = buf;
		//put data pool
		pool.put_data(outString);

		data d;
		d.dataUnserialize(outString);
		if(d.cmd == "QUIT"){
			del_user(peer, len);
		}else{
			add_user(peer, len);
		}
		return -1;
	}
	return 0;
}

int udp_server::sendData(struct sockaddr_in &client, socklen_t len, std::string &msg)
{
	//get data pool
	if(sendto(sock, msg.c_str(), msg.size(), 0, (struct sockaddr*)&client, len) < 0){
		std::cerr << "send error" << std::endl;
		return -1;
	}
	return 0;
}

int udp_server::broadcast()
{
	std::string msg;
	pool.get_data(msg);
	std::map<int,struct sockaddr_in>::iterator iter = online_user.begin();
	for( ; iter != online_user.end(); iter++)
	{
		sendData(iter->second, sizeof(iter->second), msg);
	}
}

udp_server::~udp_server()
{
	if(sock != -1){
		close(sock);
	}
}













