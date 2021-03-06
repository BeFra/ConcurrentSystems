#include "include/lwt-mutex.h"

/// @file lwt-mutex.cc
/// @brief Implementation of mutex functions

#include <stdint.h>
#include <stdio.h>
#include "mem/alloc.h"
#include "adt/simplequeue.h"
#include "spinlocks/readlock.h"
#include "sched/thread.h"
#include "sched/scheduler.h"

struct lwt_mutex {
    uintptr_t mutex_id;
    lwt::SimpleQueue<lwt::Thread> queue;
    lwt::ReadSpinlock s_lock;
    lwt::Thread* mutex_owner;
    bool lock;
    int config;

};

/// @brief Creates a new lwt_mutex
///
/// @param config Initialise the mutex with zero or more bitwise-or'ed flags
/// defined by @c LWT_MUTEX_...
///
/// @return New mutex object, @c 0 on failure
struct lwt_mutex* lwt_mutex_init(int config) {

    struct lwt_mutex* mutex = (struct lwt_mutex*) lwt::Alloc::alloc(sizeof(struct lwt_mutex));
    mutex->queue.init();
    mutex->s_lock.init();
    mutex->mutex_owner = lwt::Scheduler::currentThread;
    mutex->lock = 0;
    mutex->config = config;
    //mutex->id = (int) mutex;
    return mutex;
}

/// @brief Destroy a mutex.
///
/// @param mutex Mutex to be destroyed.
void lwt_mutex_destroy(struct lwt_mutex *mutex) {
    mutex->s_lock.lock();
    lwt::Alloc::free(mutex);
}

void lwt_mutex_block_function(void *mut) {
    struct lwt_mutex* mutex = (struct lwt_mutex*) mut;
    mutex->s_lock.unlock();
}

/// @brief Lock a mutex
///
/// @param mutex Mutex to be locked.
///
/// @return @c 1 on succes, @c 0 if the configuration
/// option LWT_MUTEX_ORDERED was provided at creation time
/// and order checking failed or LWT_MUTEX_DETECT was defined and a deadlock
/// checking found that a deadlock would happen.
int  lwt_mutex_lock(struct lwt_mutex *mutex) {
	int ret_value = 0;
    mutex->s_lock.lock();
    if(((mutex->config & LWT_MUTEX_RECURSIVE) == LWT_MUTEX_RECURSIVE)
		  &&  (lwt::Scheduler::currentThread == mutex->mutex_owner)) {
		// wenn mutex_ower ++ currentThread dann wurde der mutex zuvor von currentThread
		// gelockt und er darf bei rekursiver nutzung weiter machen.
		mutex->s_lock.unlock();
		return 1;
	} else {
		while(mutex->lock == 1) {
			mutex->queue.enqueue(lwt::Scheduler::currentThread);
			lwt::Scheduler::block(lwt_mutex_block_function, mutex);
			//ret_value = 0;
			mutex->s_lock.lock();
		}
	}
    mutex->mutex_owner = lwt::Scheduler::currentThread;
    mutex->lock = 1;
    ret_value = 1;
    mutex->s_lock.unlock();
    return ret_value;
}

/// @brief Unlock a mutex
///
/// Depending on the configuration this function checks if the mutex was
/// released by its owner, or if it was released in the right order if
///	LWT_MUTEX_ORDERED was specified at creation time.
///
/// @param mutex Mutex to be unlocked.
///
/// @return @c 1 on succes, @c 0 if any check failed
int  lwt_mutex_unlock(struct lwt_mutex *mutex) {
	int ret_value = 0;
	mutex->s_lock.lock();
    if((mutex->mutex_owner != lwt::Scheduler::currentThread)) { //mutex->config == LWT_MUTEX_CHECK_OWNER
        ret_value = 0;
    } else {
		while(!mutex->queue.isEmpty()) {
            lwt::Scheduler::readyThread(mutex->queue.dequeue());
		}
		ret_value = 1;
		mutex->mutex_owner = 0;
		mutex->lock = 0;
	}
	mutex->s_lock.unlock();
    return ret_value;
}

/// @brief Try to lock a mutex
///
/// This function tries to lock the mutex but does not wait to acquire the
/// lock if an other thread holds it.
///
/// @param mutex Mutex to be locked.
///
/// @return @c 1 on succes, @c 0 if the configuration
/// option LWT_MUTEX_ORDERED was provided at creation time
/// and order checking failed
int  lwt_mutex_trylock(struct lwt_mutex *mutex) {
	int ret_value = 0;
	mutex->s_lock.lock();
	if(mutex->mutex_owner == 0) {
		mutex->mutex_owner = lwt::Scheduler::currentThread; 
        ret_value = 1;
	} else {
		ret_value = 0;
	}
	mutex->s_lock.unlock();
    return ret_value;
}

/// @brief Return mutex id
///
/// Returns a unique number for this mutex that is different to any other
/// number returned for other mutexes that are currently allocated.
///
/// @param mutex Mutex object of which the id should be returned.
///
/// @return Mutex id, undefined if @p mutex was already freed.
uintptr_t lwt_mutex_id(struct lwt_mutex *mutex) {
    return mutex->mutex_id;
}


