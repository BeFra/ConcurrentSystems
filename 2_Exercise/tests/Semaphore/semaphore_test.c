#include <stdio.h>
#include <stdint.h>
#include "lwt.h"

static struct lwt_signal *sig;
static struct lwt_sem *semaphore;
int da[2] = {0};

static void count0(void *d, void *unused)
{
	(void)unused;
	printf("thread ZERO\n");
	volatile int *data = (volatile int*)d;
	while(1) {
		lwt_sem_wait(semaphore);
		//printf("thread ZERO in\n");
		da[0] = *data;
		for(volatile int i = 0; i < 100000000; i++);
		if(da[0] == *data) {
			printf("%d value %d\n", *data, da[0]);
		}
		lwt_sem_signal(semaphore);
		//printf("thread ZERO out\n");
		for(volatile int i = 0; i < 1000000; i++);
			
	}
	lwt_sig_signal(sig);
}
static void count1(void *d, void *unused)
{
	(void)unused;
	printf("thread ONE\n");
	volatile int *data = (volatile int*)d;
	while(1) {
		lwt_sem_wait(semaphore);
		//printf("thread ONE in\n");
		da[0]=*data;
		for(volatile int i = 0; i < 56000000; i++);
		if(da[0] == *data) {
			printf("%d value %d\n", *data, da[0]);
		}
		lwt_sem_signal(semaphore);
		//printf("thread ONE out\n");
		for(volatile int i = 0; i < 1000000; i++);

	}
	lwt_sig_signal(sig);
}

static void count2(void *d, void *unused)
{
	(void)unused;
	printf("thread TWO\n");
	volatile int *data = (volatile int*)d;
	while(1) {
		lwt_sem_wait(semaphore);
		//printf("thread TWO in\n");
		da[0]=*data;
		for(volatile int i = 0; i < 1000; i++);
		if(da[0] == *data) {
			printf("%d value %d\n", *data, da[0]);
		}
		lwt_sem_signal(semaphore);
		//printf("thread TWO out\n");
		for(volatile int i = 0; i < 1000000; i++);

	}
	lwt_sig_signal(sig);
}

static void count3(void *d, void *unused)
{
	(void)unused;
	printf("thread THREE\n");
	volatile int *data = (volatile int*)d;
	while(1) {
		lwt_sem_wait(semaphore);
		//printf("thread THREE in\n");
		da[0]=*data;
		for(volatile int i = 0; i < 1000000; i++);
		if(da[0] == *data) {
			printf("%d value %d\n", *data, da[0]);
		}
		lwt_sem_signal(semaphore);
		//printf("thread THREE out\n");
		for(volatile int i = 0; i < 1000000; i++);

	}
	lwt_sig_signal(sig);
}


void lwt_main(int argc, char *argv[], int *ret_val)
{
	int data[4] = {0,1,2,3};
	(void)argc;
	(void)argv;

	

	sig = lwt_sig_init(2);
	if(sig == 0) {
		*ret_val = 1;
		return;
	}
	semaphore = lwt_sem_init(1);
	
	lwt_run(count0, &data[0], 0);
	lwt_run(count1, &data[1], 0);
	lwt_run(count2, &data[2], 0);
	lwt_run(count3, &data[3], 0);

	lwt_sig_wait(sig);
	lwt_sig_destroy(sig);
	
	printf("data: %d %d %d %d\n", data[0], data[1], data[2], data[3]);
}
