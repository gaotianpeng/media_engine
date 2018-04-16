//
// Created by alderzhang on 2017/6/23.
//

#ifndef LITEAV_TXCMUTEX_H
#define LITEAV_TXCMUTEX_H

#include <mutex>

class TXCMutex {
public:
	TXCMutex(bool recursive = false);

	~TXCMutex();

	void lock();

    bool try_lock();

	void unlock();

    bool islocked();

#if _GTHREAD_USE_MUTEX_TIMEDLOCK

    bool timedlock(long millisecond);

	bool try_lock_for(const std::chrono::duration<long long int, std::milli>  &duration);

	bool try_lock_until(const std::chrono::system_clock::time_point &timePoint);

#endif

private:
	TXCMutex(const TXCMutex&);
	TXCMutex& operator = (const TXCMutex&);

private:
	bool _recursive;

#if _GTHREAD_USE_MUTEX_TIMEDLOCK
	std::recursive_timed_mutex *_recursive_mutex;
	std::timed_mutex *_mutex;
#else
    std::recursive_mutex *_recursive_mutex;
	std::mutex *_mutex;
#endif
};


#endif //LITEAV_TXCMUTEX_H
