FLAGS = -Wall -Wextra -O3
LDFLAGS+=-lpthread -lm 



.PHONEY: all clean single pthread lwtThreads

all: single pthread lwtThreads

%.o: %.c
	gcc ${FLAGS} -std=c99 -c $<

%.o: %.cc
	g++ ${FLAGS} -std=c++11 -c $<

single: single.o mandel.o
	gcc ${FLAGS} -o $@ $^ -lm

pthread: pthread.o mandel.o
	gcc ${FLAGS} -o $@ $^ ${LDFLAGS}

lwtThreads: lwtThreads.c mandel.c
	make -C ../lwt
	gcc -Wall -Wextra -I ../lwt/include -O3 -std=c99 -o lwtThreads lwtThreads.c mandel.c ../lwt/build/lwt.a -pthread -lm
	#gcc ${FLAGS} -I ../lwt/include -o $@ $^ ../lwt/build/lwt.a -pthread

clean:
	make -C ../lwt clean
	rm -f single pthread lwtThreads *.o
