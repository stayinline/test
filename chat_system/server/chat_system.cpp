#include "udp_server.h"
#include <unistd.h>
#include <pthread.h>

static void usage(const char *proc)
{
	std::cout << "Usage:\n\t" << proc << " [local_port]" << " [local_ip]\n" << std::endl;
}

void *run_consume(void *arg)
{
	udp_server *sp = (udp_server*)arg;
	while(1){
		sp->broadcast();
	}
}

void *run_product(void *arg)
{
	udp_server *sp = (udp_server*)arg;
	std::string outString;
	while(1){
		sp->recvData(outString);
		std::cout << "client# " << outString << std::endl;
	}
}

int main(int argc, char *argv[])
{
	if(argc != 3){
		usage(argv[0]);
		return 1;
	}
	udp_server server(argv[1], atoi(argv[2]));
	server.initServer();

	daemon(1, 0);
	pthread_t c, p;
	pthread_create(&p, NULL, run_consume, &server);
	pthread_create(&c, NULL, run_product, &server);

	pthread_join(p, NULL);
	pthread_join(c, NULL);
	return 0;
}















