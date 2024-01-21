#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "String.h"
using namespace Utils;

#include "ItemIF.h"
#include "DecorateItem.h"
#include "Normal.h"
#include "Bold.h"
#include "StrikeThrough.h"
#include "ForegroundColor.h"
#include "BackgroundColor.h"
#include "SwapColor.h"
#include "Underline.h"
#include "Blink.h"
using namespace Color;

std::map<std::string, std::shared_ptr<ItemIF>> Params = {};

void help()
{
	printf("Usage:\n");
	printf("  must:\n");
	printf("    -n [String] convert string\n");
	printf("    -s [String] convert string with strikethrough\n");
	printf("    -u [String] convert string with underline\n");
	printf("\n");
	printf("  optional:\n");
	printf("    -f [color]  foreground color\n");
	printf("    -b [color]  background color\n");
	printf("    -r          swap background and foreground\n");
	printf("    -B          bold\n");
	printf("    -x          blink\n");
	printf("    -h          show this message\n");
	printf("\n");
	exit(1);
}

void generate(int argc, char **argv)
{
	int opt;
	std::shared_ptr<ItemIF> obj2 = nullptr;
	std::string target = "";

	while ((opt = getopt(argc, argv, "n:s:u:f:b:rBxh")) != -1) {
		switch (opt) {
			case 'n': { std::shared_ptr<ItemIF> obj(new Normal());        obj2 = obj; target = optarg; break; }
			case 's': { std::shared_ptr<ItemIF> obj(new StrikeThrough()); obj2 = obj; target = optarg; break; }
			case 'u': { std::shared_ptr<ItemIF> obj(new Underline());     obj2 = obj; target = optarg; break; }

			case 'f': { std::shared_ptr<ItemIF> obj(new ForegroundColor(optarg, obj2)); obj2 = obj; break; }
			case 'b': { std::shared_ptr<ItemIF> obj(new BackgroundColor(optarg, obj2)); obj2 = obj; break; }
			case 'r': { std::shared_ptr<ItemIF> obj(new SwapColor(obj2)); obj2 = obj; break; }
			case 'B': { std::shared_ptr<ItemIF> obj(new Bold(obj2));      obj2 = obj; break; }
			case 'x': { std::shared_ptr<ItemIF> obj(new Blink(obj2));     obj2 = obj; break; }

			case 'h':
			default: help(); break;
		}
		if(target != "")
			Params[target] = obj2;
	}
}

int main(int argc, char **argv)
{
	try {
		generate(argc, argv);
	}
	catch (const std::exception & e) {
		return -1;
	}

	for(auto p : Params)
		p.second->Prepare(p.first);

	std::string line;
	while(std::getline(std::cin, line)) {
		for(auto p : Params)
			p.second->Filter(line);
		std::cout << line << std::endl;
	}
	return 0;
}
