#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "Json.h"

void json_parse(void)
{
	Json::JsonIF *json = Json::Create("hoge");

	json->ParseFile("hoge.json");
	json->Dump();

	Json::GetInstance("hoge")->Save("hoge.dat");

#if 0
	JSON_MAP &Map = json->GetRoot();
	std::cout << std::boolalpha;
	std::cout << Map["key4"].GetInt() << std::endl;
	std::cout << Map["key7"][1].GetString() << std::endl;
	std::cout << Map["key3"]["KEY1"].GetBool() << std::endl;
	std::cout << Map["key3"]["KEY3"][2]["bbb"].GetFloat() << std::endl;
	std::cout << Map["key3"]["KEY3"][2]["ccc"].GetString() << std::endl;

	Map["key3"]["KEY3"][2]["bbb"].SetFloat(-123.456);
	Map["key3"]["KEY3"][2]["ccc"].SetString("hogehoge");
	std::cout << Map["key3"]["KEY3"][2]["bbb"].GetFloat() << std::endl;
	std::cout << Map["key3"]["KEY3"][2]["ccc"].GetString() << std::endl;
#endif

	Json::Destroy("hoge");
}

int main(int argc, char **argv)
{
	json_parse();
	return 0;
}
