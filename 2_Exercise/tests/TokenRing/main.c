#include <stdlib.h>
#include <stdio.h>
#include "lwt.h"

typedef struct
{
	struct lwt_signal *prev;
	struct lwt_signal *next;
	int id;
} ThreadData;

static const int numThreads = 100;
static volatile int token = 0;

static void ring(void *td, void *unused)
{
	
	(void)unused;

	ThreadData *data = (ThreadData*)td;
	const int myid = data->id;
	printf("enter thread %d\n",myid);
	
	if(myid != (numThreads - 1)) {
		lwt_sig_wait(data->prev);
	}
	token += 1;
	lwt_sig_signal(data->next);
}

ThreadData tdata[100];
struct lwt_signal* signal[100];

void lwt_main(int argc, char *argv[], int *ret_val)
{
	(void)argc;
	(void)argv;
	
	
	//sem_t sems[numThreads];
	
	
	for(int i = 0; i < numThreads; ++i) {
		signal[i] = lwt_sig_init(1);
	}


	for(int i = 0; i < numThreads; ++i) {
		if(i == 0) {
			tdata[i].prev = signal[numThreads-1];
		}
		else {
			tdata[i].prev = signal[i-1];
		}

		tdata[i].next = signal[i];
		tdata[i].id = i;
	}
	
    printf("finsih\n");
	for(int i = 0; i < numThreads; ++i) {
		lwt_run(ring, (void*)(&tdata[i]), 0);
	}

	lwt_destroy();

	printf("token == %d\n", token);
	*ret_val = 0;
	return;
}
