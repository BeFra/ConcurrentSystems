#ifndef   LWT_BASIC_HEADER
#define   LWT_BASIC_HEADER

/// @file lwt-basic.h
/// @brief Header for basic initialisation, destruction and creation functions

#ifdef __cplusplus
extern "C" {
#endif

typedef void(*lwt_func)(void*, void*);

/// @brief LWT initialisation function
///
/// Starts a new thread-pool using Pthreads.
/// It initialises global data-structures and starts the function
/// @p start_func with @p argc, @p argv @p main_ret as arguments in the new
/// thread-pool. If successful this function will not return until
/// lwt_destroy() was called.
/// 
/// @param start_func Function pointer to be executed.
/// @param argc Argument passed to function @p start_func.
/// @param argv Argument passed to function @p start_func.
/// @param main_ret Argument passed to function @p start_func. 
/// 
/// @return Returns @c 0 on success.
int lwt_init(void(*start_func)(int, char**, int*), int argc, char *argv[], int *main_ret);

/// @brief Automatic LWT start function
///
/// This function is not implemented in the library, just a declaration is
/// provided here. If user code implements this function and does not have a
/// main() function, LWT will provide a main() function and automatically call
/// lwt_init() and pass lwt_main() as its argument. It will also pass a
/// zero-initialised @p ret_val argument which will be returned from main() if
/// lwt_main() returns. Note that no error checking is performed on the return
/// value of lwt_init().
/// 
/// @param argc Argument passed from main().
/// @param argv Argument passed from main().
/// @param main_ret Argument will be returned in the enclosing main() function. 
void lwt_main(int argc, char *argv[], int *ret_val);

/// @brief LWT termination function
///
/// Terminates all system threads and releases all internally aquired
/// resources. The initial thread that called lwt_init() will return from its
/// call. Assumes all user-threads created with lwt_run() already finished their
/// execution.
void lwt_destroy();

/// @brief Creates a new user thread in the thread pool of system threads.
///
/// @param func Funtion pointer to be executed.
/// @param arg1 Argument 1 passed to function @p func.
/// @param arg2 Argument 2 passed to function @p func.
///
/// @return Returns @c 0 on success.
int lwt_run(lwt_func func, void *arg1, void *arg2);


#ifdef __cplusplus
} // extern "C"
#endif

#endif /* LWT_BASIC_HEADER */
