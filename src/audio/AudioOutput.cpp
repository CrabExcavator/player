//
// Created by weihan on 2020/10/20.
//

#include "AudioOutput.h"
#include "driver/DriverFactory.h"
#include "common/Config.h"
#include "core/PlayerContext.h"
#include "demux/Frame.h"

namespace audio {

    AudioOutput::AudioOutput(): _thread("ao") {

    }

    void AudioOutput::init(const core::player_ctx_sptr &player_ctx) {
        this->queue = player_ctx->ao_queue;
        this->_driver = driver::DriverFactory::create(GET_CONFIG(ao_driver));
        this->_driver->init(shared_from_this());
        this->_running = true;
        this->_thread.run([&](){
           do{} while(this->loop());
        });
    }

    void AudioOutput::stopRunning() {
        this->_running = false;
        this->_thread.join();
    }

    bool AudioOutput::loop() {
        if (this->_running) {
            if (this->need_reConfig) {
                this->_driver->reConfig(shared_from_this());
                this->need_reConfig = false;
            }
            if (this->_frame != nullptr) {
                this->_last_tick = std::chrono::steady_clock::now();
                this->_last_pts = this->_frame->pts;

                // playing
                this->frame_playing = this->_frame;
                this->_driver->play(shared_from_this());
                this->frame_playing = nullptr;

                this->_frame = nullptr;
            } else if (this->queue->read(this->_frame)) {
                // todo check sound fmt
                // todo check sound cfg
                if (this->_frame->first) {
                    this->_last_tick = std::chrono::steady_clock::now() + std::chrono::seconds(1);
                    this->_last_pts = 0;
                }
                this->_time_base = this->_frame->time_base;
            }
        }
        return this->_running;
    }

}
