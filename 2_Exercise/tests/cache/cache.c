#include <stdio.h>
#include <stdint.h>
#include "lwt.h"

static struct lwt_barrier *barrier;

volatile int a = 0;
volatile int b = 0;

static void thread1()
{
	for(;;) {
		a = 1;
		int otherB = b;

		if(otherB == 0) {
			printf("saw 0\n");
		}
		lwt_barrier_wait(barrier);
		a = 0;
		printf("\n");
		lwt_barrier_wait(barrier);
	}
}

static void thread2()
{
	for(;;) {
		b = 1;
		int otherA = a;

		if(otherA == 0) {
			printf("saw 0\n");
		}
		lwt_barrier_wait(barrier);
		b = 0;
		lwt_barrier_wait(barrier);
	}
}

static struct lwt_signal *sig;
static void t1(void* a, void* b)
{
    (void) a;
    (void) b;
	thread1();
	lwt_sig_signal(sig);
}

static void t2(void* a, void* b)
{
    (void) a;
    (void) b;
	thread2();
	lwt_sig_signal(sig);
}

void lwt_main(int argc, char *argv[], int *ret_val)
{
	(void)argc;
	(void)argv;

	barrier = lwt_barrier_init(2);
	sig = lwt_sig_init(2);
	if(barrier == 0 || sig == 0) {
		*ret_val = 1;
		return;
	}

	lwt_run(t1, 0, 0);
	lwt_run(t2, 0, 0);

	lwt_sig_wait(sig);
	lwt_sig_destroy(sig);
}
