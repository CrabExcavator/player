//
// Created by weihan on 2020/10/31.
//

#include "Sync.h"
#include "misc/util.h"

namespace common {

    Sync::Sync() : _size(0), _cnt(0) {

    }

    void Sync::wait() {
        std::unique_lock<std::mutex> lock(this->_mutex);
        this->_cnt++;
        this->_cond.wait(lock, [&](){
            return this->_cnt >= this->_size;
        });
        lock.unlock();
        this->_cond.notify_one();
    }

    void Sync::init(int size) {
        this->_size = size;
    }

}