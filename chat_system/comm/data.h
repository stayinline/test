#ifndef _DATA_H_
#define _DATA_H_

#include <iostream>
#include <string>
#include "base_json.h"

class data{
	public:
		data();
        int dataSerialize(std::string &outString);//序列化
        void dataUnserialize(std::string &inString);//反序列化
		~data();

	public:
		std::string nick_name;
		std::string school;
		std::string msg;
		std::string cmd;
};

#endif

