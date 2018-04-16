//
// Created by alderzhang on 2017/6/23.
//

#ifndef TXCSPINLOCK_H
#define TXCSPINLOCK_H

#include <atomic>

class TXCSpinLock {
	std::atomic_flag _flag;
public:
	TXCSpinLock(){
		_flag.clear();
	}

	TXCSpinLock(const TXCSpinLock &) = delete;

	TXCSpinLock &operator=(const TXCSpinLock &) = delete;

	void lock() {
		while (_flag.test_and_set(std::memory_order_acquire));
	}

	void unlock() {
		_flag.clear(std::memory_order_release);
	}
};

#endif  //TXCSPINLOCK_H
