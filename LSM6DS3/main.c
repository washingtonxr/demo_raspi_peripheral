#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include "main.h"
#include "LSM6DS3.h"

struct timespec ts;

void sigalrm_fun(int sig)
{
	alarm(1);
	clock_gettime(CLOCK_MONOTONIC, &ts);
	printf("Info:STS: %d.%d\n", ts.tv_sec, ts.tv_nsec);
#if 0
	Lsm6ds3_GetSingleRawData();
#else
	Lsm6ds3_GetRawDataFromFIFO();
#endif
	return;
}

int main(int argc, unsigned char **argv)
{
	printf("Info: This is a demostration for LSM6DS3TR-C DS.\n");
	
	Lsm6ds3_Init();
	
	signal(SIGALRM, sigalrm_fun);
	alarm(1);

	while(1){
#if 0
		Lsm6ds3_GetSingleRawData();
		usleep(10000);
#endif
		sleep(1);
	}
	return 0;
}
/* End of this file. */
