//
// Created by weihan on 2020/10/7.
//

#include "PlayerContext.h"

namespace core {

    void PlayerContext::init() {
        this->play_list = std::make_shared<PlayList>();
        this->play_list->addLast(std::make_shared<core::PlayEntry>
                                         (core::entry_type::file, "small_bunny_1080p_60fps.mp4", 0));
        this->input_ctx = std::make_shared<input::InputContext>();
        this->_vo = std::make_shared<video::VideoOutput>();
        this->_vo->init(shared_from_this());
        this->_demux_ctx = std::make_shared<demux::DemuxContext>();
        this->_demux_ctx->init(shared_from_this());
    }

    void PlayerContext::run() {
        do {} while(this->loop());
    }

    bool PlayerContext::loop() {
        // handle events
        if (this->input_ctx->hasEvent(input::event::exit)) {
            this->_vo->running = false;
            this->_vo->_thread.join();
            this->_demux_ctx->running = false;
            this->_demux_ctx->_thread.join();
            return false;
        }
        this->input_ctx->clear();

        // some vo function must be called in main loop
        this->_vo->_driver->waitEvents(this->_vo);

        return true;
    }

}
