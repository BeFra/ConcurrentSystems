#ifndef   LWT_COND_VAR_HEADER
#define   LWT_COND_VAR_HEADER

/// @file lwt-cond.h
/// @brief Header for condition variable functions

#include "lwt-mutex.h"

#ifdef __cplusplus
extern "C" {
#endif

struct lwt_cond;

/// @brief Creates a new lwt_cond
///
/// @return New condition variable, @c 0 on failure
struct lwt_cond* lwt_cond_init();

/// @brief Destroy a condition variable.
///
/// @param cond Cond to be destroyed.
void lwt_cond_destroy(struct lwt_cond *cond);

/// @brief Signal a condition
///
/// Signal a condition, waking up a single thread waiting on it.
/// Before calling this function the same mutex must be locked that was used
/// in the lwt_cond_wait() call.
///
/// @param cond Condition variable to be signaled.
///
/// @return @c 1 if a thread could be woken up, @c 0 if there was no waiting
/// thread.
int lwt_cond_signal(struct lwt_cond *cond);


/// @brief Wait for a condition
///
/// Wait for a condition to be signaled. The @p mutex object must be locked
/// before calling this function. The current thread will sleep on the condition
/// variable and atomically release the mutex. If the thread was signaled, it
/// must acquire the lock again.
///
/// @param cond Condition variable to wait on.
/// @param mutex To be released while waiting, will be acquired again after
/// wakeup.
///
/// @return @c 1 if successful, @c 0 if any checks failed, the mutex may not
/// be locked in this case.
/// thread.
int lwt_cond_wait(struct lwt_cond *cond, struct lwt_mutex *mutex);


#ifdef __cplusplus
} // extern "C"
#endif

#endif /* LWT_COND_VAR_HEADER */
