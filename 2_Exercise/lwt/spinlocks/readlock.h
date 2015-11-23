#ifndef   LWT_READ_SPINLOCK_HEADER
#define   LWT_READ_SPINLOCK_HEADER

#include "arch/atomic.h"
#include "util/die.h"

namespace lwt {

class ReadSpinlock
{
	private:
	volatile int lockvar;

	public:
	void init()
	{
		lockvar = 0;
	}

	void lock()
	{
		while(lwt::Atomic::swap(&lockvar, 1) == 1) {
			while(lockvar == 1) { // ist billiger als staendig per atomic auf die variable zugreifen
				lwt::Atomic::busy();
			}
		}
	}

	void unlock()
	{
		lwt::Atomic::swap(&lockvar, 0);
	}

	bool trylock()
	{
		return lwt::Atomic::swap(&lockvar, 1) == 0;
	}
};

} // namespace lwt

#endif /* LWT_READ_SPINLOCK_HEADER */
