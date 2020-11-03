//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "DemuxContext.h"
#include "core/PlayerContext.h"
#include "core/Sync.h"

common::error demux::DemuxContext::init(const core::player_ctx_sptr& player_ctx) {
    this->vo_queue = player_ctx->vo_queue;
    this->ao_queue = player_ctx->ao_queue;
    this->sync_ = player_ctx->sync_;

    this->_play_list = player_ctx->play_list;
    this->_running = true;
    this->_thread.run([&](){
        do{} while(this->loop());
    });
    return common::error::success;
}

bool demux::DemuxContext::loop() {
    if (this->_demuxer == nullptr) {
        auto entry = this->_play_list->current();
        if (entry == nullptr) return false;
        this->_demuxer = std::make_shared<Demuxer>();
        this->_demuxer->init(entry, shared_from_this());
        if (this->sync_ != nullptr) {
            this->sync_->close();
            // todo current only have ao + vo
            this->sync_->init(std::min(1, this->_demuxer->nbStreams()));
        }
    }
    auto ret = this->_demuxer->epoch();
    if (ret == common::error::eof) {
        this->_demuxer->flush();
        this->_demuxer->close();
        this->_demuxer = nullptr;
        // todo wait playback end
        this->_play_list->next();
    }
    return _running;
}

common::error demux::DemuxContext::stopRunning() {
    this->_running = false;
    this->_thread.join();
    return common::error::success;
}
