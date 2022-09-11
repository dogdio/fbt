#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "TestMain.h"
#include "TestConfigName.h"
using namespace test;

void help(void)
{
	printf("ExecTest\n");
	printf("    -l <file> .... set log file(default: ./test.log)\n");
	printf("    -d <dir>  .... set DLL search path(default: ./plugin)\n");
	printf("    -n <num>  .... test loop number(default: 1)\n");
	printf("\n");
	printf("    -s <str>  .... test pattern to run(default: \"\")\n");
	printf("    -u <str>  .... do test until specified pattern(default: \"\")\n");
	printf("    -f <str>  .... do test from specified pattern(default: \"\")\n");
	printf("\n");
	printf("    -h        .... show this message\n");
	printf("\n");
}

int main(int argc, char **argv)
{
	int opt = 0;
	int opt_suf = 0;
	int ret = -1;

	main::Init();

	while ((opt = getopt(argc, argv, "l:d:n:s:u:f:h")) != -1) {
		switch (opt) {
			case 'n':
				main::SetConfig(CONFIG_LOOPNUM, atoi(optarg));
				break;
			case 'l':
				main::SetConfig(CONFIG_LOGFILE, optarg);
				break;
			case 'd':
				main::SetConfig(CONFIG_DIRECTORY, optarg);
				break;
			case 's':
				main::SetConfig(CONFIG_TEST_PATTERN_RUN, optarg);
				opt_suf++;
				break;
			case 'u':
				main::SetConfig(CONFIG_TEST_PATTERN_UNTIL, optarg);
				opt_suf++;
				break;
			case 'f':
				main::SetConfig(CONFIG_TEST_PATTERN_FROM, optarg);
				opt_suf++;
				break;
			case 'h':
			default:
				help();
				goto end;
		}
	}
	if(opt_suf > 1) {
		printf("ERR: only one of -s,-u,-f can be selected\n");
		help();
		goto end;
	}

	if(main::Run()) {
		printf("\nresult: OK\n");
		ret = 0;
	}
	else
		printf("\nresult: NG\n");
end:
	return ret;
}
