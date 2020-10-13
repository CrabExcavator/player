//
// Created by weihan on 2020/10/11.
//

#include "DemuxContext.h"
#include "core/PlayerContext.h"

void demux::DemuxContext::init(const core::player_ctx_sptr& player_ctx) {
    this->queue = player_ctx->queue;

    this->_play_list = player_ctx->play_list;
    this->running = true;
    this->_thread.run([&](){
        do{} while(this->loop());
    });
}

bool demux::DemuxContext::loop() {
    if (this->_demuxer == nullptr) {
        auto entry = this->_play_list->current();
        if (entry == nullptr) return false;
        this->_demuxer = std::make_shared<Demuxer>(entry);
        this->_demuxer->init(shared_from_this());
    }
    int ret = this->_demuxer->epoch();
    if (ret < 0) {
        this->_demuxer = nullptr;
        this->_play_list->next();
    }
    return running;
}
