#include "mem/alloc.h"
#include "spinlocks/readlock.h"
#include "include/lwt.h"
#include "sched/scheduler.h"
#include "lwt.h"
#include <stdio.h>

struct lwt_signal {
    unsigned int number;
    lwt::ReadSpinlock s_lock;
    lwt::Thread *currentThread;
};

/// @brief Creates a new lwt_signal
///
/// @param num Initialise the signal with the number @p num.
///
/// @return New signal object, @c 0 on failure
struct lwt_signal* lwt_sig_init(unsigned int num) {
    struct lwt_signal *signal = (struct lwt_signal*) lwt::Alloc::alloc(sizeof(struct lwt_signal));
    signal->number = num;
    signal->s_lock.init();
    return signal;   
}
    

/// @brief Reset the signal counter.
///
/// Reset the signal counter. The behavior is undefined if the signal object
/// is concurrently used by a waiting user-thread or by a signaling
/// user-thread.
///
/// @param sig Signal object to reset.
/// @param num Value to assign to the signal object.
void lwt_sig_reset(struct lwt_signal *sig, unsigned int num) {
    sig->number = num;
}

/// @brief Destroy a signal.
///
/// @param sig Signal to be destroyed.
void lwt_sig_destroy(struct lwt_signal *sig) {
    lwt::Alloc::free(sig);
}

/// @brief Signal a signal object.
///
/// Decrement the signal counter by one. Multiple user-threads can use this
/// function in parallel. The behavior is undefined if the signal-object is
/// signaled more often than it was initialised to.
///
/// @param sig Signal object to signal.
void lwt_sig_signal(struct lwt_signal *sig) {
    sig->s_lock.lock();
    sig->number -=1;
    lwt::Scheduler::readyThread(sig->currentThread);
    printf("signaleddddddddddddddddd\n");
    sig->s_lock.unlock();
}

void lwt_sig_unlock(void *sig) {
    struct lwt_signal *signal = (struct lwt_signal*) sig;
    signal->currentThread = lwt::Scheduler::currentThread;
    signal->s_lock.unlock();
}

/// @brief Wait for the signal to become zero.
///
/// Only one user-thread is allowed to wait for a given signal. The behavior is
/// undefined if multiple threads try to wait on the same signal.
///
/// @param sig Signal to wait for. 
void lwt_sig_wait(struct lwt_signal *sig) {
    bool block = true;
    do {
        sig->s_lock.lock();
        if(sig->number != 0) {
            lwt::Scheduler::block(lwt_sig_unlock, sig);
        } else {
            block = false;
	    sig->s_lock.unlock();
        }
    } while(block);
}




