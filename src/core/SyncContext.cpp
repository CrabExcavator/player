//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "SyncContext.h"
#include "misc/util.h"
#include "demux/stream/Stream.h"
#include "core/PlayerContext.h"

namespace core {

    SyncContext::SyncContext() : _size(0), _cnt(0), _close(true) {

    }

    common::error SyncContext::init(const core::player_ctx_sptr& player_ctx) {
        this->_video_streams = std::make_shared<std::vector<demux::stream::stream_sptr>>();
        this->_audio_streams = std::make_shared<std::vector<demux::stream::stream_sptr>>();
        this->version = 0;
        this->_input_ctx = player_ctx->input_ctx;
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

    common::error SyncContext::addVideoStream(const demux::stream::stream_sptr &stream) {
        this->_video_streams->emplace_back(stream);
        return common::error::success;
    }

    common::error SyncContext::addAudioStream(const demux::stream::stream_sptr &stream) {
        this->_audio_streams->emplace_back(stream);
        return common::error::success;
    }

    common::error SyncContext::addStream(const demux::stream::stream_sptr &stream) {
        if (stream->op == output_port::audio) {
            return this->addAudioStream(stream);
        } else if (stream->op == output_port::video) {
            return this->addVideoStream(stream);
        }
        return common::error::unknownError;
    }

    common::error SyncContext::getVideoStream(demux::stream::stream_sptr &stream) {
        assert(this->_video_streams->size() <= 1);
        if (this->_video_streams->empty()) {
            return common::error::noStream;
        }
        stream = this->_video_streams->at(0);
        this->_video_streams->clear();
        return common::error::success;
    }

    common::error SyncContext::getAudioStream(demux::stream::stream_sptr &stream) {
        assert(this->_audio_streams->size() <= 1);
        if (this->_audio_streams->empty()) {
            return common::error::noStream;
        }
        stream = this->_audio_streams->at(0);
        this->_audio_streams->clear();
        return common::error::success;
    }

}