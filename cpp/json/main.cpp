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
	JSON_MAP &Root = json->GetRoot();

	Root["key3"]["KEY1"].Set(true);
	for(auto &m : Root) {
		std::cout << m.first << " => " << m.second.ToString() << std::endl;
	}
	for(auto &m : Root["key3"].Map()) {
		std::cout << m.first << " => " << m.second.ToString() << std::endl;
	}

	std::cout << Root["key3"]["KEY3"][2]["bbb"].GetDouble() << std::endl;
	std::cout << Root["key3"]["KEY3"][2]["ccc"].GetString() << std::endl;

	Root["key3"]["KEY3"][2]["bbb"].Set(-123.456);
	Root["key3"]["KEY3"][2]["ccc"].Set("hogehoge");
	Root["key7"][2].Set("GHI");
	std::cout << std::endl;

	for(auto &a : Root["key3"]["KEY3"].Array()) {
		for(auto &m : a.Map()) {
			std::cout << m.first << " => " << m.second.ToString() << std::endl;
		}
	}
	for(auto &a : Root["key7"].Array()) {
		std::cout << a.GetString() << std::endl;
	}

#endif

	Json::Destroy("hoge");
}

int main(int argc, char **argv)
{
	json_parse();
	return 0;
}
