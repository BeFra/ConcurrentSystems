
.PHONY: all clean

all: nosharing

nosharing: nosharing.c
	make -C ../../lwt
	gcc -Wall -Wextra -I ../../lwt/include -O3 -std=c99 -o nosharing nosharing.c ../../lwt/build/lwt.a -pthread

clean:
	make -C ../../lwt clean
	rm -f nosharing
