#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include "udp_client.h"
#include "data.h"
#include "window.h"

udp_client *cp = NULL;  //一个全局变量
std::string nick_name;
std::string school;
std::vector<std::string> friends_list;
volatile int stop = 0;

static void usage(const char *proc)
{
	std::cout << "Usage:\n\t" << proc << "Usage: " << "[server_ip] " << "[server_port]\n" << std::endl;
}


void *runHeader(void *arg)
{
	window *wp = (window*)arg;
    std::string msg = "Welcome To Chat System!";
	int i = 1;
    int x, y;
	while(1){
		wp->drawHeader();
    	getmaxyx(wp->header, y, x); 			//返回win窗口中最大的行数并存储在y变量中，
												//返回最大的列数并存储在x变量中。 
												//函数没有返回值，也没有出错的信息
    	wp->putStringToWin(wp->header, y/2, i++, msg);
    	wrefresh(wp->header);
		wp->clearWinLines(wp->header, y/2, 1);
		if( i >= x - msg.size() ){
			i = 1;
		}
		usleep(600000);
	}
}

void addUser(std::string &name)
{
	std::vector<std::string>::iterator iter = friends_list.begin();
	for(; iter != friends_list.end(); iter++){
		if(*iter == name){
			return;
		}
	}
	friends_list.push_back(name);
}

void delUser(std::string &name)
{
	std::vector<std::string>::iterator iter = friends_list.begin();
	for(; iter != friends_list.end();){
		if(*iter == name){
			iter = friends_list.erase(iter);
			break;
		}else{
			iter++;
		}
	}
}

void *runOutputFlist(void *arg)
{
	window *wp = (window*)arg;

	wp->drawFlist();
	wrefresh(wp->flist);
	wp->drawOutput();
	wrefresh(wp->output);

	std::string recvString;
	int i = 1;
	int x, y;
	while(1){
		cp->recvData(recvString);
		data d;
        d.dataUnserialize(recvString);
		//name-school# nihao?
		std::string showString = d.nick_name;
		showString += "-";
		showString += d.school;
		std::string fname = showString;
		showString += "# ";
		showString += d.msg;

		if(d.cmd != "QUIT"){
		    getmaxyx(wp->output, y, x);
	        wp->putStringToWin(wp->output, i++, 1, showString);
		    wrefresh(wp->output);
		    if( i > y - 1){
		    	wp->clearWinLines(wp->output, 1, y);
		    	wp->drawOutput();
		    	wrefresh(wp->output);
		    	i = 1;
		    }
			addUser(fname);
		}else{
			delUser(fname);
		}

		getmaxyx(wp->flist, y, x);
		wp->clearWinLines(wp->flist, 1, y);
		wp->drawFlist();
		int i = 0;
		int num = friends_list.size();
		for(; i < num; i++){
			wp->putStringToWin(wp->flist, i+1, 1, friends_list[i]);
		}
		wrefresh(wp->flist);
	}
}

void *runInput(void *arg)
{
	window *wp = (window*)arg;
	std::string msg = "Please Enter# ";
	int x, y;
	std::string outString;

	while(1){
	    wp->drawInput();
	    getmaxyx(wp->input, y, x);
	    wp->putStringToWin(wp->input, 1, 1, msg);
	    wp->getStringFromWin(wp->input, outString);
	    wp->clearWinLines(wp->input, 1, 1);
	    wrefresh(wp->input);
		
		data d;
		d.nick_name = nick_name;
		d.school = school;
		d.cmd = "";
		d.msg = outString;
		std::string sendString;
        d.dataSerialize(sendString);

		cp->sendData(sendString);
	}
}

//设置2号信号的信号处理函数，
void handler(int sig)
{
	data d;
	std::string sendString;
	d.nick_name = nick_name;
	d.school = school;
	d.msg = "";
	d.cmd = "QUIT";
	d.dataSerialize(sendString);
	cp->sendData(sendString);
	stop = 1;
}

int main(int argc, char *argv[])
{
	if(argc != 3){
		usage(argv[0]);
		return 1;
	}


	std::cout << "Please Enter Your Nick Name# ";
	std::cin >> nick_name;
	std::cout << "Please Enter Your School# ";
	std::cin >> school;

	udp_client client(argv[1], atoi(argv[2]));		//client的IP和端口创建了一个client对象
	client.initClient();
	cp = &client;		//cp是一个全局的udp_client的类型指针
	window win;

	signal(2, handler);
	pthread_t header, output_flist, input;
	pthread_create(&header, NULL, runHeader, &win);
	pthread_detach(header);
	pthread_create(&output_flist, NULL, runOutputFlist, &win);
	pthread_detach(output_flist);
	pthread_create(&input, NULL, runInput, &win);
	pthread_detach(input);

	while(!stop){
		sleep(1);
	}
	return 0;

//	char buf[1024];
//	while(1){
//		std::cout << "Please Enter# ";
//		fflush(stdout);
//		ssize_t s = read(0, buf, sizeof(buf)-1);
//		if(s > 0){
//			buf[s-1] = 0;
//			std::string str = buf;
//			client.sendData(str);
//
//			client.recvData(str);
//			std::cout << "server echo# " << str << std::endl;
//		}
//	}
	return 0;
}

















