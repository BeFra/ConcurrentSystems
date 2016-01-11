

/// @file lwt-sem.cc
/// @brief Implementation for semaphore functions


struct lwt_sem;

/// @brief Creates a new lwt_sem
///
/// @param num Starting value of internal counter.
///
/// @return New semaphore, @c 0 on failure
struct lwt_sem* lwt_sem_init(unsigned int num) {
	
}

/// @brief Destroy a semaphore.
///
/// @param sem Semaphore to be destroyed.
void lwt_sem_destroy(struct lwt_sem *sem) {
	
}

/// @brief Signal a semaphore
///
/// Signal a semaphore, if threads are waiting on this semaphore (and the
/// internal counter is 0) a single thread is woken up. If no thread waits on
/// this semaphore, then the internal counter is incremented.
///
/// @param sem Semaphore to be signaled.
void lwt_sem_signal(struct lwt_sem *sem) {
	
}
	

/// @brief Wait on a semaphore
///
/// if the internal counter is > 0 it is decremented and the calling thread
/// returns, else it waits on the semaphore until it is signaled.
///
/// @param sem Semaphore to wait on.
void lwt_sem_wait(struct lwt_sem *sem) {
	
}


