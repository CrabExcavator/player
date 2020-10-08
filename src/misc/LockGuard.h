//
// Created by weihan on 2020/10/8.
//

#ifndef PLAYER_LOCKGUARD_H
#define PLAYER_LOCKGUARD_H

#include "Lock.h"

namespace misc {

    class LockGuard {
    public:
        LockGuard() = delete;
        LockGuard(const LockGuard& rhs) = delete;
        LockGuard(LockGuard&& rhs) = delete;
        LockGuard& operator = (const LockGuard& rhs) = delete;
        LockGuard& operator = (LockGuard&& rhs) = delete;
        LockGuard(Lock& rhs): _lock(rhs) {
            this->_lock.lock();
        }
        ~LockGuard() {
            this->_lock.unlock();
        }

    private:
        Lock& _lock;
    };

}

#endif //PLAYER_LOCKGUARD_H
