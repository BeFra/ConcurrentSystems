#ifndef   LWT_SIGNAL_HEADER
#define   LWT_SIGNAL_HEADER

/// @file lwt-signal.h
/// @brief Header for single-waiter signal-functions.

#ifdef __cplusplus
extern "C" {
#endif

struct lwt_signal;

/// @brief Creates a new lwt_signal
///
/// @param num Initialise the signal with the number @p num.
///
/// @return New signal object, @c 0 on failure
struct lwt_signal* lwt_sig_init(unsigned int num);

/// @brief Reset the signal counter.
///
/// Reset the signal counter. The behavior is undefined if the signal object
/// is concurrently used by a waiting user-thread or by a signaling
/// user-thread.
///
/// @param sig Signal object to reset.
/// @param num Value to assign to the signal object.
void lwt_sig_reset(struct lwt_signal *sig, unsigned int num);

/// @brief Destroy a signal.
///
/// @param sig Signal to be destroyed.
void lwt_sig_destroy(struct lwt_signal *sig);

/// @brief Signal a signal object.
///
/// Decrement the signal counter by one. Multiple user-threads can use this
/// function in parallel. The behavior is undefined if the signal-object is
/// signaled more often than it was initialised to.
///
/// @param sig Signal object to signal.
void lwt_sig_signal(struct lwt_signal *sig);

/// @brief Wait for the signal to become zero.
///
/// Only one user-thread is allowed to wait for a given signal. The behavior is
/// undefined if multiple threads try to wait on the same signal.
///
/// @param sig Signal to wait for. 
void lwt_sig_wait(struct lwt_signal *sig);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* LWT_SIGNAL_HEADER */
