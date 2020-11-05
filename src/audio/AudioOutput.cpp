//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "AudioOutput.h"
#include "driver/DriverFactory.h"
#include "common/Config.h"
#include "core/PlayerContext.h"
#include "demux/Frame.h"
#include "demux/stream/Stream.h"

namespace audio {

    AudioOutput::AudioOutput(): _thread("audio output") {

    }

    common::error AudioOutput::init(const core::player_ctx_sptr &player_ctx) {
        this->_driver = driver::DriverFactory::create(GET_CONFIG(ao_driver));
        this->_version = player_ctx->sync_ctx->version;
        this->_sync_ctx = player_ctx->sync_ctx;
        this->_running = true;
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
                /**
                 * we can not just dive into playback code with sync reason
                 * sync with other output
                 * @todo use audio clock for sync reason
                 */
                //_sync->wait();

                /// start playback
                this->frame_playing = this->_frame;
                this->_driver->play(shared_from_this());
                this->frame_playing = nullptr;
                /// end playback

                this->_frame = nullptr;
            }

            /// update output version
            if (this->_version != this->_sync_ctx->version) {
                this->_sync_ctx->getAudioStream(this->_stream);
                this->_version = this->_sync_ctx->version;
            }

            if (this->_stream != nullptr &&
            this->_stream->read(this->_frame) == common::error::success) {
                /**
                 * we should do something for first frame
                 * @attention the first frame always carry data
                 */
                if (this->_frame->first) {
                    this->sampleFormat = this->_frame->sample_fmt;
                    this->num_of_channel = this->_frame->num_of_channel;
                    this->size_of_sample = this->_frame->sample_size;
                    this->sample_rate = this->_frame->sample_rate;
                    this->_driver->init(shared_from_this());
                }

                /**
                 * we should do something for last frame
                 * @attentionq the last frame never carry data
                 */
                if (this->_frame->last) {
                    /// @todo do something
                    this->_stream = nullptr;
                }
            }
        }
        return this->_running;
    }

}
