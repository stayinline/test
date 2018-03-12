#ifndef _BASE_JSON_H_
#define _BASE_JSON_H_

#include <iostream>
#include <string>
#include <json/json.h>


int serialize(Json::Value &val, std::string &outString);
int unserialize(std::string &inString, Json::Value &val);

#endif