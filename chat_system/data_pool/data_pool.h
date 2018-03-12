
#ifndef _DATA_POOL_H_
#define _DATA_POOL_H_

#include <iostream>
#include <vector>
#include <string>
#include <semaphore.h>

class data_pool{
	public:
		data_pool(int size);
		int put_data(const std::string &inString);
		//放数据之前要加锁(blank_sem),放完要将p_step++；并且%=size ;最后释放data_sem
		
		int get_data(std::string &outString);
		~data_pool();
	private:
	
		std::vector<std::string> pool;//用vector当做环形队列
		int cap;
		sem_t data_sem;//用两个信号量来保证互斥与同步
		sem_t blank_sem;
		int c_step; //两个标识生产者和消费者的位置的索引
		int p_step;
};

#endif












