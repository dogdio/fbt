#include <stdio.h>
#include <unistd.h>
#include <time.h>

int
main(int argc, char *argv[])
{
	struct timespec req;
	struct timespec now;
	struct timespec init;
	int num = 1;

	clock_gettime(CLOCK_REALTIME, &req);
	init = req;
	req.tv_sec  += 1;
	req.tv_nsec += 0;

	while (1) {
		clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &req, NULL);
		// reqをそのまま加算(sleep後の処理にかかる時間を排除)
		req.tv_sec  += 1;
		req.tv_nsec += 0;

		clock_gettime(CLOCK_REALTIME, &now); // 現在時刻
		printf("[%ld] init: %ld.%09ld, now: %ld.%09ld, [%f]msec\n", now.tv_sec - init.tv_sec,
			(long) init.tv_sec, init.tv_nsec, (long) now.tv_sec, now.tv_nsec,
			((double)(now.tv_nsec - init.tv_nsec)/ 1000000));

		{ // do anything .....
			if(num++ % 7 == 0) {
				usleep(1000*300);
				printf("#####\n");
			}
		}
	}
	return 0;
}
