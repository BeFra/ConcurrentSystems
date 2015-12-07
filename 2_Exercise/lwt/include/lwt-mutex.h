#ifndef   LWT_MUTEX_HEADER
#define   LWT_MUTEX_HEADER

/// @file lwt-mutex.h
/// @brief Header for mutex functions

#include <stdint.h>

/// @brief Most simple mutex with no checking whatsoever
#define LWT_MUTEX_SIMPLE      (0)

/// @brief Mutex that checks if its unlocked by its owner
#define LWT_MUTEX_CHECK_OWNER (1)

/// @brief Mutex that can be used recursively without deadlock
#define LWT_MUTEX_RECURSIVE   (2)

/// @brief Mutex that can only be locked if the Thread does not hold a mutex with a higher ID
#define LWT_MUTEX_ORDERED     (4)

/// @brief Deadlock detecting Mutex
#define LWT_MUTEX_DETECT      (8)

#ifdef __cplusplus
extern "C" {
#endif

struct lwt_mutex;

/// @brief Creates a new lwt_mutex
///
/// @param config Initialise the mutex with zero or more bitwise-or'ed flags
/// defined by @c LWT_MUTEX_...
///
/// @return New mutex object, @c 0 on failure
struct lwt_mutex* lwt_mutex_init(int config);

/// @brief Destroy a mutex.
///
/// @param mutex Mutex to be destroyed.
void lwt_mutex_destroy(struct lwt_mutex *mutex);

/// @brief Lock a mutex
///
/// @param mutex Mutex to be locked.
///
/// @return @c 1 on succes, @c 0 if the configuration
/// option LWT_MUTEX_ORDERED was provided at creation time
/// and order checking failed or LWT_MUTEX_DETECT was defined and a deadlock
/// checking found that a deadlock would happen.
int  lwt_mutex_lock(struct lwt_mutex *mutex);

/// @brief Unlock a mutex
///
/// Depending on the configuration this function checks if the mutex was
/// released by its owner, or if it was released in the right order if
///	LWT_MUTEX_ORDERED was specified at creation time.
///
/// @param mutex Mutex to be unlocked.
///
/// @return @c 1 on succes, @c 0 if any check failed
int  lwt_mutex_unlock(struct lwt_mutex *mutex);

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
int  lwt_mutex_trylock(struct lwt_mutex *mutex);

/// @brief Return mutex id
///
/// Returns a unique number for this mutex that is different to any other
/// number returned for other mutexes that are currently allocated.
///
/// @param mutex Mutex object of which the id should be returned.
///
/// @return Mutex id, undefined if @p mutex was already freed.
uintptr_t lwt_mutex_id(struct lwt_mutex *mutex);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* LWT_MUTEX_HEADER */
