#include <stdio.h>
#include <stdint.h>
#include "lwt.h"

static struct lwt_signal *sig;

static void count(void *d, void *unused)
{
	(void)unused;

	volatile uint64_t *data = (volatile uint64_t*)d;

	for(int i = 0; i < 100000; ++i) {
		for(int k = 0; k < 10000; ++k) {
			*data += 1;
		}
	}

	lwt_sig_signal(sig);
}

void lwt_main(int argc, char *argv[], int *ret_val)
{
	(void)argc;
	(void)argv;

	uint64_t data[4] = {0};

	sig = lwt_sig_init(4);
	if(sig == 0) {
		*ret_val = 1;
		return;
	}

	lwt_run(count, &data[0], 0);
	lwt_run(count, &data[1], 0);
	lwt_run(count, &data[2], 0);
	lwt_run(count, &data[3], 0);

	lwt_sig_wait(sig);
	lwt_sig_destroy(sig);

	printf("data: %ld %ld %ld %ld\n", data[0], data[1], data[2], data[3]);
}
