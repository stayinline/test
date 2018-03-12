#include "data.h"

data::data()
{}

//序列化就是将string转化成Value类型，，然后用于网络中的传输
int data::dataSerialize(std::string &outString)
{
	Json::Value root;
	root["nick_name"] = nick_name;
	root["school"] = school;
	root["msg"] = msg;
	root["cmd"] = cmd;

	return serialize(root, outString);
}

void data::dataUnserialize(std::string &inString)
{
	Json::Value root;
	unserialize(inString, root);
	nick_name = root["nick_name"].asString();
	school = root["school"].asString();
	msg = root["msg"].asString();
	cmd = root["cmd"].asString();
}

data::~data()
{
}

//
//int main()
//{
//	data d;
//	d.nick_name = "flypig";
//	d.school = "SUST";
//	d.msg = "hello world!";
//	d.cmd = "";
//	std::string s;
//	d.dataSerialize(s);
//	std::cout << s << std::endl;
//	return 0;
//}
//







