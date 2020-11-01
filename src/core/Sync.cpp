//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "Sync.h"
#include "misc/util.h"

namespace core {

    Sync::Sync() : _size(0), _cnt(0), _close(true) {

    }

    void Sync::wait() {
        if (this->_close) return;
        std::unique_lock<std::mutex> lock(this->_mutex);
        this->_cnt++;
        this->_cond.wait(lock, [&](){
            return (this->_cnt >= this->_size) || (this->_close);
        });
        lock.unlock();
        this->_cond.notify_one();
    }

    void Sync::init(int size) {
        this->_mutex.lock();
        DEFER([&](){this->_mutex.unlock();});
        this->_cnt = 0;
        this->_size = size;
        this->_close = false;
    }

    void Sync::close() {
        this->_close = true;
        this->_cond.notify_all();
    }

}