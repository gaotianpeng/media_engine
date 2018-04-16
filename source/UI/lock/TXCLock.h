//
// Created by alderzhang on 2017/6/23.
//

#ifndef TXCLOCK_H
#define TXCLOCK_H

#define ASSERT(X)
#define ASSERT2(X, Y, ...)

#include "TXCMutex.h"
#include "TXCSpinLock.h"

typedef std::unique_lock<TXCMutex> TXCScopedLock;
typedef std::unique_lock<TXCSpinLock> TXCScopedSpinLock;

#endif //TXCLOCK_H
