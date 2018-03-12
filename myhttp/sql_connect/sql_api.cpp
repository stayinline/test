#include "sql_api.h"

int main()
{
	sqlApi mydb("127.0.0.1",3306);
	mydb.connect("zhangsan","man","20","cording","sust");
	return 0;
}






