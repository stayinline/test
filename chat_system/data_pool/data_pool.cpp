#include "data_pool.h"

data_pool::data_pool(int size):
	pool(size), 
	cap(size)
{
	c_step = 0;
	p_step = 0;
	sem_init(&data_sem, NULL, 0);
	sem_init(&blank_sem, NULL, cap);
}

int data_pool::put_data(const std::string &inString)
{
	sem_wait(&blank_sem);
	pool[p_step++] = inString;
	p_step %= cap;
	sem_post(&data_sem);
}

int data_pool::get_data(std::string &outString)
{
	sem_wait(&data_sem);
	outString = pool[c_step++];
	c_step %= cap;
	sem_wait(&blank_sem);
}

data_pool::~data_pool()
{
	sem_destroy(&data_sem);
	sem_destroy(&blank_sem);
}












