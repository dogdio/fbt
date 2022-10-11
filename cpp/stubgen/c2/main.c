#include <stdio.h>
#include <pthread.h>

void *Thread(void *arg)
{
	printf("hogehoge\n");
	return NULL;
}

//
// gcc pthread.c -lpthread
//
int main(void)
{
	pthread_t t;

	pthread_create(&t, NULL, Thread, NULL);
	pthread_join(t, NULL);

	return 0;
}
