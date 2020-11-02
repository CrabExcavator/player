//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "VideoOutput.h"
#include "video/driver/DriverFactory.h"
#include "common/Config.h"
#include "core/Sync.h"
#include "core/PlayerContext.h"
#include "demux/Frame.h"

namespace video {

    static core::sync_sptr _sync = nullptr;

    VideoOutput::VideoOutput(): _thread("vo") {

    }

    common::error VideoOutput::init(const core::player_ctx_sptr& player_ctx) {
        this->queue = player_ctx->vo_queue;
        this->_input_ctx = player_ctx->input_ctx;
        this->window_width = GET_CONFIG(window_width);
        this->window_height = GET_CONFIG(window_height);
        this->_driver = driver::DriverFactory::create(GET_CONFIG(vo_driver));
        this->_driver->init(shared_from_this());
        _sync = player_ctx->sync_;
        this->_running = true;
        this->_thread.run([&](){
            do{} while(this->loop());
        });
        return common::error::success;
    }

    input::input_ctx_sptr VideoOutput::getInputCtx() {
        return this->_input_ctx;
    }

    bool VideoOutput::loop() {
        if (this->_running) {
            /* force reConfig */
            if (this->need_reConfig) {
                this->_driver->reConfig(shared_from_this());
                this->need_reConfig = false;
            }

            if (this->_frame != nullptr) {
                /* we can not just dive into playback code with sync reason */
                /* sync with system clock */
                auto rendering_time = (this->_frame->pts - this->_last_pts) * this->_time_base + this->_last_tick;
                std::this_thread::sleep_until(rendering_time);
                this->_last_tick = std::chrono::steady_clock::now();
                this->_last_pts = this->_frame->pts;

                /* we can not just dive into playback code with sync reason */
                /* sync with other output */
                _sync->wait();

                /* we can measure this is the true playback code for simplicity */
                /******************** playback ********************/
                this->frame_rendering = this->_frame;
                this->_driver->drawImage(shared_from_this());
                this->frame_rendering = nullptr;
                /******************** playback ********************/

                this->_frame = nullptr;
            } else if (this->queue->read(this->_frame)) {
                /* init driver if some args not match, idk is it ok putting in first frame */
                if (this->_frame->img_fmt != this->imgfmt) {
                    this->imgfmt = this->_frame->img_fmt;
                    this->_driver->reConfig(shared_from_this());
                }
                if (this->_frame->height != this->img_height || this->_frame->pitch != this->img_pitch) {
                    this->img_height = this->_frame->height;
                    this->img_pitch = this->_frame->pitch;
                    this->_driver->reConfig(shared_from_this());
                }

                /* we should do something for first frame */
                /* ATT: the first frame always carry data */
                if (this->_frame->first) {
                    this->_last_tick = std::chrono::steady_clock::now() + std::chrono::seconds(1);
                    this->_last_pts = 0;
                    this->_time_base = this->_frame->time_base;
                }

                /* we should do something for last frame */
                /* ATT: the last frame never carry data */
                if (this->_frame->last) {
                    // do something
                }
            }
        }
        return this->_running;
    }

    common::error VideoOutput::loopInMainThread() {
        this->_driver->waitEvents(shared_from_this());
        return common::error::success;
    }

    common::error VideoOutput::stopRunning() {
        this->_running = false;
        this->_thread.join();
        return common::error::success;
    }

}
