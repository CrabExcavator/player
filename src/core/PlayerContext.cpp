//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include <memory>

#include "PlayerContext.h"
#include "common/Config.h"
#include "input/InputContext.h"
#include "audio/AudioOutput.h"
#include "video/VideoOutput.h"
#include "demux/DemuxContext.h"
#include "Sync.h"

namespace core {

    static std::string audio_sample = "sample.mp3";
    static std::string video_sample = "small_bunny_1080p_60fps.mp4";

    void PlayerContext::init() {
        auto queue_size = GET_CONFIG(default_queue_size);
        this->vo_queue = std::make_shared<folly::MPMCQueue<demux::frame_sptr>>(queue_size);
        this->ao_queue = std::make_shared<folly::MPMCQueue<demux::frame_sptr>>(queue_size);

        this->play_list = std::make_shared<PlayList>();
        this->play_list->addLast(std::make_shared<core::PlayEntry>
                                         (core::entry_type::file, video_sample, 0));
        this->sync_ = std::make_shared<Sync>();
        this->input_ctx = std::make_shared<input::InputContext>();
        this->_demux_ctx = std::make_shared<demux::DemuxContext>();
        this->_demux_ctx->init(shared_from_this());
        this->_ao = std::make_shared<audio::AudioOutput>();
        this->_ao->init(shared_from_this());
        this->_vo = std::make_shared<video::VideoOutput>();
        this->_vo->init(shared_from_this());
    }

    void PlayerContext::run() {
        do {} while(this->loop());
    }

    bool PlayerContext::loop() {
        // handle events
        if (this->input_ctx->hasEvent(input::event::exit)) {
            // stop producer before consumer
            this->_demux_ctx->stopRunning();
            this->_ao->stopRunning();
            this->_vo->stopRunning();
            return false;
        } else if (this->input_ctx->hasEvent(input::event::window_resize)) {
            // todo handle window_resize event
        }
        this->input_ctx->clear();

        // some vo function must be called in main loop
        this->_vo->loopInMainThread();

        return true;
    }

}
