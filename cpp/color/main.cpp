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

std::map<std::string, ItemIF *> Params = {};

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
	ItemIF *obj = nullptr;
	std::string target;

	while ((opt = getopt(argc, argv, "n:s:u:f:b:rBxh")) != -1) {
		switch (opt) {
			case 'n': obj = new Normal();        target = optarg; break;
			case 's': obj = new StrikeThrough(); target = optarg; break;
			case 'u': obj = new Underline();     target = optarg; break;

			case 'f': if(obj != nullptr) obj = new ForegroundColor(optarg, obj); break;
			case 'b': if(obj != nullptr) obj = new BackgroundColor(optarg, obj); break;
			case 'r': if(obj != nullptr) obj = new SwapColor(obj); break;
			case 'B': if(obj != nullptr) obj = new Bold(obj); break;
			case 'x': if(obj != nullptr) obj = new Blink(obj); break;

			case 'h':
			default: help(); break;
		}
		Params[target] = obj;
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
