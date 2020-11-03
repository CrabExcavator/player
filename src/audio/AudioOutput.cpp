//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "AudioOutput.h"
#include "driver/DriverFactory.h"
#include "common/Config.h"
#include "core/PlayerContext.h"
#include "core/Sync.h"
#include "demux/Frame.h"

namespace audio {

    static std::shared_ptr<core::Sync> _sync = nullptr;

    AudioOutput::AudioOutput(): _thread("ao") {

    }

    common::error AudioOutput::init(const core::player_ctx_sptr &player_ctx) {
        this->queue = player_ctx->ao_queue;
        this->_driver = driver::DriverFactory::create(GET_CONFIG(ao_driver));
        this->_running = true;
        _sync = player_ctx->sync_;
        this->_thread.run([&](){
           do{} while(this->loop());
        });
        return common::error::success;
    }

    common::error AudioOutput::stopRunning() {
        this->_running = false;
        this->_thread.join();
        return common::error::success;
    }

    bool AudioOutput::loop() {
        if (this->_running) {
            /// force reConfig
            if (this->need_reConfig) {
                this->_driver->reConfig(shared_from_this());
                this->need_reConfig = false;
            }

            if (this->_frame != nullptr) {
                /// @todo these variables are useless for audio we need driver to produce audio clock
                this->_last_tick = std::chrono::steady_clock::now();
                this->_last_pts = this->_frame->pts;

                /**
                 * we can not just dive into playback code with sync reason
                 * sync with other output
                 * @todo use audio clock for sync reason
                 */
                _sync->wait();

                /// start playback
                this->frame_playing = this->_frame;
                this->_driver->play(shared_from_this());
                this->frame_playing = nullptr;
                /// end playback

                this->_frame = nullptr;
            } else if (this->queue->read(this->_frame)) {
                /**
                 * we should do something for first frame
                 * @attention the first frame always carry data
                 */
                if (this->_frame->first) {
                    this->_last_tick = std::chrono::steady_clock::now() + std::chrono::seconds(1);
                    this->_last_pts = 0;
                    this->sampleFormat = this->_frame->sample_fmt;
                    this->num_of_channel = this->_frame->num_of_channel;
                    this->size_of_sample = this->_frame->sample_size;
                    this->sample_rate = this->_frame->sample_rate;
                    this->_driver->init(shared_from_this());
                    this->_time_base = this->_frame->time_base;
                }

                /**
                 * we should do something for last frame
                 * @attentionq the last frame never carry data
                 */
                if (this->_frame->last) {
                    /// @todo do something
                }
            }
        }
        return this->_running;
    }

}
