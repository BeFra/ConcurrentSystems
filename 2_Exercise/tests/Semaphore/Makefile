
.PHONY: all clean

all: semaphore_test

semaphore_test: semaphore_test.c
	make -C ../../lwt
	gcc -Wall -Wextra -I ../../lwt/include -O3 -g -std=c99 -o semaphore_test semaphore_test.c ../../lwt/build/lwt.a -pthread

clean:
	make -C ../../lwt clean
	rm -f semaphore_test
