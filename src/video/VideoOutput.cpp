//
// Created by weihan on 2020/10/7.
//

#include "VideoOutput.h"
#include "video/driver/DriverFactory.h"
#include "common/Config.h"
#include "core/PlayerContext.h"
#include "demux/Frame.h"

namespace video {

    static std::shared_ptr<common::Sync> _sync = nullptr;

    VideoOutput::VideoOutput(): _thread("vo") {

    }

    void VideoOutput::init(const core::player_ctx_sptr& player_ctx) {
        this->queue = player_ctx->vo_queue;
        this->_input_ctx = player_ctx->input_ctx;
        this->window_width = GET_CONFIG(window_width);
        this->window_height = GET_CONFIG(window_height);
        this->_driver = driver::DriverFactory::create(GET_CONFIG(vo_driver));
        this->_driver->init(shared_from_this());
        this->_running = true;
        _sync = player_ctx->sync;
        this->_thread.run([&](){
            do{} while(this->loop());
        });
    }

    input::input_ctx_sptr VideoOutput::getInputCtx() {
        return this->_input_ctx;
    }

    bool VideoOutput::loop() {
        if (this->_running) {
            if (this->need_reConfig) {
                this->_driver->reConfig(shared_from_this());
                this->need_reConfig = false;
            }
            if (this->_frame != nullptr) {
                auto rendering_time = (this->_frame->pts - this->_last_pts) * this->_time_base + this->_last_tick;
                std::this_thread::sleep_until(rendering_time);
                this->_last_tick = std::chrono::steady_clock::now();
                this->_last_pts = this->_frame->pts;

                // rendering
                _sync->wait();
                this->frame_rendering = this->_frame;
                this->_driver->drawImage(shared_from_this());
                this->frame_rendering = nullptr;

                this->_frame = nullptr;
            } else if (this->queue->read(this->_frame)) {
                if (this->_frame->img_fmt != this->imgfmt) {
                    this->imgfmt = this->_frame->img_fmt;
                    this->_driver->reConfig(shared_from_this());
                }
                if (this->_frame->height != this->img_height || this->_frame->pitch != this->img_pitch) {
                    this->img_height = this->_frame->height;
                    this->img_pitch = this->_frame->pitch;
                    this->_driver->reConfig(shared_from_this());
                }
                if (this->_frame->first) {
                    this->_last_tick = std::chrono::steady_clock::now() + std::chrono::seconds(1);
                    this->_last_pts = 0;
                }
                this->_time_base = this->_frame->time_base;
            }
        }
        return this->_running;
    }

    void VideoOutput::loopInMainThread() {
        this->_driver->waitEvents(shared_from_this());
    }

    void VideoOutput::stopRunning() {
        this->_running = false;
        this->_thread.join();
    }

}
