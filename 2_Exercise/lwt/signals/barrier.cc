#include "../spinlocks/readlock.h"
#include "../include/lwt-barrier.h"
#include "../sched/thread.h"
#include "../mem/alloc.h"
struct lwt_barrier {
    unsigned int number;
    lwt::ReadSpinlock s_lock;
    lwt::Thread *currentThread;
};

/// @brief Creates a new lwt_barrier
///
/// @param num Initialise the barrier with the number @p num. Which will be the
/// number of user-threads that call lwt_barrier_wait()
///
/// @return New lwt_barrier object, @c 0 on failure
struct lwt_barrier* lwt_barrier_init(unsigned int num) {
    struct lwt_barrier *barrier = (struct lwt_barrier*) lwt::Alloc::alloc(sizeof(struct lwt_barrier));
    

}

/// @brief Destroy a barrier.
///
/// @param barrier Barrier to be destroyed.
void lwt_barrier_destroy(struct lwt_barrier *barrier) {

}

/// @brief Wait on the barrier.
///
/// Wait for the predefined number of threads to arrive at the barrier.
///
/// @param barrier The barrier to wait on.
void lwt_barrier_wait(struct lwt_barrier *barrier) {

}

