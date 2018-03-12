#include"httpd.h"

static void usage(const char * proc)
{
	printf("Usage:\n\t%s./http [http_ip] [http_port]\n\n",proc);
}

int main(int argc,char *argv[])
{

	if(argc!=3){
		usage(argv[0]);
		return 1;
	}

	int listen_sock=startup(argv[1],atoi(argv[2]));
	printf("listen_sock:%d\n",listen_sock);
	//daemon(0,0);
	while(1){
		struct sockaddr_in client;
		socklen_t len=sizeof(client);
		client.sin_family=AF_INET;
		client.sin_port=htons(atoi(argv[2]));
		client.sin_addr.s_addr=inet_addr(argv[1]);
		int rw_sock=accept(listen_sock,(struct sockaddr*)&client,&len);
		if(rw_sock<0)
		{
			perror("accept");
			//print_log(strerror(errno));
			continue;
		}
		printf("rw_sock:%d\n",rw_sock);
		printf("get a client:[%s:%d]\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
	
		pthread_t id;
	
		int ret = pthread_create(&id,NULL,handler_request,(void *)rw_sock);
		if(ret != 0){
			//print_log(strerror(strrno));
			close(rw_sock);
		}else{
			pthread_detach(id);
		}
	}

	return 0;
}
