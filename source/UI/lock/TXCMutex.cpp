//
// Created by alderzhang on 2017/6/23.
//

#include "TXCMutex.h"

TXCMutex::TXCMutex(bool recursive) : _recursive(recursive), _recursive_mutex(nullptr), _mutex(nullptr) {
#if _GTHREAD_USE_MUTEX_TIMEDLOCK
	if (_recursive) {
		_recursive_mutex = new std::recursive_timed_mutex();
	} else {
		_mutex = new std::timed_mutex();
	}
#else
	if (_recursive) {
		_recursive_mutex = new std::recursive_mutex();
	} else {
		_mutex = new std::mutex();
	}
#endif
}

TXCMutex::~TXCMutex() {
	if (_recursive_mutex) {
		delete _recursive_mutex;
	}
	if (_mutex) {
		delete _mutex;
	}
}

void TXCMutex::lock() {
	if (_recursive) {
		_recursive_mutex->lock();
	} else {
		_mutex->lock();
	}
}

bool TXCMutex::try_lock() {
	if (_recursive) {
		return _recursive_mutex->try_lock();
	} else {
		return _mutex->try_lock();
	}
}

void TXCMutex::unlock() {
	if (_recursive) {
		_recursive_mutex->unlock();
	} else {
		_mutex->unlock();
	}
}

bool TXCMutex::islocked() {
	bool ret = try_lock();

	if (ret) unlock();

	return ret;
}

#if _GTHREAD_USE_MUTEX_TIMEDLOCK

bool TXCMutex::timedlock(long millisecond) {
	return try_lock_for(std::chrono::milliseconds(millisecond));
}

bool TXCMutex::try_lock_for(const std::chrono::duration<long long int, std::milli> &duration) {
	if (_recursive) {
		return _recursive_mutex->try_lock_for(duration);
	} else {
		return _mutex->try_lock_for(duration);
	}
}

bool TXCMutex::try_lock_until(const std::chrono::system_clock::time_point &timePoint) {
	if (_recursive) {
		return _recursive_mutex->try_lock_until(timePoint);
	} else {
		return _mutex->try_lock_until(timePoint);
	}
}

#endif
