
.PHONY: all clean

all: main

main: main.c
	make -C ../../lwt
	gcc -Wall -Wextra -I ../../lwt/include -O3 -std=c99 -o main main.c ../../lwt/build/lwt.a -pthread

clean:
	make -C ../../lwt clean
	rm -f main
