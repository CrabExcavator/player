//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "DemuxContext.h"
#include "core/PlayerContext.h"
#include "core/SyncContext.h"
#include "input/InputContext.h"

common::error demux::DemuxContext::init(const core::player_ctx_sptr& player_ctx) {
    this->sync_ctx = player_ctx->sync_ctx;
    this->_input_context = player_ctx->input_ctx;
    this->_running = true;
    this->_thread.run([&](){
        do{} while(this->loop());
    });
    return common::error::success;
}

bool demux::DemuxContext::loop() {
    if (this->_demuxer == nullptr) {
        core::play_entry_sptr entry = nullptr;
        this->_input_context->getCurrent(entry);
        if (entry == nullptr) return _running;
        this->_demuxer = std::make_shared<Demuxer>();
        this->_demuxer->init(entry, shared_from_this());
        this->sync_ctx->close();
        this->sync_ctx->version++;
    }
    auto err = this->_demuxer->epoch();
    if (err == common::error::eof) {
        this->_demuxer->flush();
        this->_demuxer->close();
        this->_demuxer = nullptr;
    }
    return _running;
}

common::error demux::DemuxContext::stopRunning() {
    this->_running = false;
    this->_thread.join();
    return common::error::success;
}
