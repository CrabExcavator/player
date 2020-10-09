//
// Created by weihan on 2020/10/8.
//

#ifndef PLAYER_LOCK_H
#define PLAYER_LOCK_H

#include <string>
#include <mutex>
#include <glog/logging.h>

namespace misc {

    class Lock {
#define MISC_LOCK_TT "lock " << _lock_name << " "
    public:
        Lock(): _lock_name("air") {};
        Lock(const Lock& rhs) = delete;
        Lock(Lock&& rhs) = delete;
        Lock& operator = (const Lock& rhs) = delete;
        Lock& operator = (Lock&& rhs) = delete;
        explicit Lock(std::string lock_name): _lock_name(std::move(lock_name)) {

        }
        ~Lock() = default;

        inline void lock() {
            LOG(INFO) << MISC_LOCK_TT << "lock";
            this->_mutex.lock();
        }
        inline void unlock() {
            LOG(INFO) << MISC_LOCK_TT << "unlock";
            this->_mutex.unlock();
        }
        inline bool try_lock() {
            LOG(INFO) << MISC_LOCK_TT << "try_lock";
            return this->_mutex.try_lock();
        }

    private:
        std::string _lock_name;
        std::mutex _mutex;
    };

}

#endif //PLAYER_LOCK_H
