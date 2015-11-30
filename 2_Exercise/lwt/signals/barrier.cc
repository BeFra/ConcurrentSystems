#include "../spinlocks/readlock.h"
#include "../include/lwt-barrier.h"
#include "../sched/thread.h"
#include "../mem/alloc.h"
#include "../adt/simplequeue.h"
#include "sched/scheduler.h"
struct lwt_barrier {
    unsigned int number;
    unsigned int base;
    lwt::ReadSpinlock s_lock;
    lwt::SimpleQueue<lwt::Thread> queue;
};

/// @brief Creates a new lwt_barrier
///
/// @param num Initialise the barrier with the number @p num. Which will be the
/// number of user-threads that call lwt_barrier_wait()
///
/// @return New lwt_barrier object, @c 0 on failure
struct lwt_barrier* lwt_barrier_init(unsigned int num) {
    struct lwt_barrier *barrier = (struct lwt_barrier*) lwt::Alloc::alloc(sizeof(struct lwt_barrier));
    barrier->number = num;
    barrier->base = num;
    barrier->s_lock.init();
    barrier->queue.init();
    return barrier;
}

/// @brief Destroy a barrier.
///
/// @param barrier Barrier to be destroyed.
void lwt_barrier_destroy(struct lwt_barrier *barrier) {
    barrier->s_lock.lock();
    free(barrier);
}

void lwt_barrier_unlock(void *bar) {
    struct lwt_barrier* barrier = (struct lwt_barrier*) bar;
   
    barrier->s_lock.unlock();
}

/// @brief Wait on the barrier.
///
/// Wait for the predefined number of threads to arrive at the barrier.
///
/// @param barrier The barrier to wait on.
void lwt_barrier_wait(struct lwt_barrier *barrier) {
    barrier->s_lock.lock();
    barrier->number -= 1;
    if(barrier->number == 0) {
        //wackup all other sleeping threads
        while(!barrier->queue.isEmpty()) {
            lwt::Scheduler::readyThread(barrier->queue.dequeue());
        }
        barrier->number = barrier->base;
    } else {
        barrier->queue.enqueue(lwt::Scheduler::currentThread);
        lwt::Scheduler::block(lwt_barrier_unlock, barrier);
        return;
    }
    barrier->s_lock.unlock();
}

