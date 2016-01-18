#include "include/lwt-mutex.h"
#include "include/lwt-cond.h"
#include "mem/alloc.h"
#include "adt/simplequeue.h"
#include "spinlocks/readlock.h"




/// @file lwt-sem.cc
/// @brief Implementation for semaphore functions


struct lwt_sem {
	struct lwt_mutex* mutex;
	struct lwt_cond* cond;
    lwt::SimpleQueue<lwt::Thread> queue;

	int counter;
	
};

/// @brief Creates a new lwt_sem
///
/// @param num Starting value of internal counter.
///
/// @return New semaphore, @c 0 on failure
struct lwt_sem* lwt_sem_init(unsigned int num) {
	struct lwt_sem* sem = (struct lwt_sem*) lwt::Alloc::alloc(sizeof(struct lwt_sem));
	
	sem->counter = num;
	sem->queue.init();
	sem->mutex = lwt_mutex_init(0);
	sem->cond = lwt_cond_init();
	return sem;
}

/// @brief Destroy a semaphore.
///
/// @param sem Semaphore to be destroyed.
void lwt_sem_destroy(struct lwt_sem *sem) {
    lwt::Alloc::free(sem);
}

/// @brief Signal a semaphore
///
/// Signal a semaphore, if threads are waiting on this semaphore (and the
/// internal counter is 0) a single thread is woken up. If no thread waits on
/// this semaphore, then the internal counter is incremented.
///
/// @param sem Semaphore to be signaled.
void lwt_sem_signal(struct lwt_sem *sem) {
	lwt_mutex_lock(sem->mutex);
	if(sem->counter == 0) {
		lwt_cond_signal(sem->cond);
	} else {
		sem->counter++;
	}
	lwt_mutex_unlock(sem->mutex);
}
	

/// @brief Wait on a semaphore
///
/// if the internal counter is > 0 it is decremented and the calling thread
/// returns, else it waits on the semaphore until it is signaled.
///
/// @param sem Semaphore to wait on.
void lwt_sem_wait(struct lwt_sem *sem) {
	lwt_mutex_lock(sem->mutex);
	if (sem->counter == 0) {
		lwt_cond_wait(sem->cond,sem->mutex);
	} else {
		sem->counter--;
	}
	lwt_mutex_unlock(sem->mutex);
}


