#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "Library.h"
using namespace Utils;
#include "Base.h"
using namespace Instance;

void help(void)
{
	printf("InstanceManager\n");
	printf("\n");
	printf("    -d  .... specify load directory\n");
	printf("    -h  .... show this message\n");
	printf("\n");
}

int main(int argc, char **argv)
{
	int opt = 0;
	char buf[80];
	bool loop = true;
	const char *dir = "./plugin";

	if(!Base::InitializeBase())
		return -1;

	while ((opt = getopt(argc, argv, "d:h")) != -1) {
		switch (opt) {
			case 'd':
				dir = optarg;
				break;
			case 'h':
			default:
				help();
				goto end;
		}
	}

	if(!Library::Load(dir)) {
		printf("### load error: %s\n", dir);
		return -1;
	}

	while(loop) {
		fgets(buf, sizeof(buf)-1, stdin);
		switch(buf[0]) {
			case 'i': Base::Initialize(); Base::Start(); break;
			case 'f': Base::Stop(); Base::Finalize(); break;
			case 's': Base::StartWDT(); break;
			case 'e': Base::StopWDT(); break;
			case 'q': loop = false; break;
		}
	}

	Library::UnLoad();
end:
	Base::FinalizeBase();
	return 0;
}
