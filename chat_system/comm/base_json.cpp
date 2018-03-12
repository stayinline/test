#include "base_json.h"

int serialize(Json::Value &val, std::string &outString)
{
#ifdef _STYLE_
	Json::StyledWriter w;
#else
	Json::FastWriter w;
#endif

	outString = w.write(val);
	return 0;
}

int unserialize(std::string &inString, Json::Value &val)
{
	Json::Reader r;
	return r.parse(inString, val, false);
}












