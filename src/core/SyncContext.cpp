//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "SyncContext.h"
#include "misc/util.h"
#include "demux/Stream.h"

namespace core {

    SyncContext::SyncContext() : _size(0), _cnt(0), _close(true) {

    }

    common::error SyncContext::init(core::player_ctx_sptr player_ctx) {
        this->_streams = std::make_shared<std::vector<demux::stream_sptr>>();
        this->version = 0;
        return common::error::success;
    }

    void SyncContext::wait() {
        if (this->_close) return;
        std::unique_lock<std::mutex> lock(this->_mutex);
        this->_cnt++;
        this->_cond.wait(lock, [&](){
            return (this->_cnt >= this->_size) || (this->_close);
        });
        lock.unlock();
        this->_cond.notify_one();
    }

    void SyncContext::setNumOfStream(int size) {
        this->_mutex.lock();
        DEFER([&](){this->_mutex.unlock();});
        this->_cnt = 0;
        this->_size = size;
        this->_close = false;
        this->version = 0;
    }

    void SyncContext::close() {
        this->_close = true;
        this->_cond.notify_all();
    }

    common::error SyncContext::addStream(const demux::stream_sptr& stream) {
        this->_streams->emplace_back(stream);
        return common::error::success;
    }

}