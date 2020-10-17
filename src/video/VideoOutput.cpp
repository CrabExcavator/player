//
// Created by weihan on 2020/10/7.
//

#include "VideoOutput.h"
#include "video/driver/DriverFactory.h"
#include "common/Config.h"
#include "core/PlayerContext.h"
#include "demux/Frame.h"

namespace video {

    VideoOutput::VideoOutput(): _thread("vo") {

    }

    void VideoOutput::init(const core::player_ctx_sptr& player_ctx) {
        this->queue = player_ctx->queue;

        this->_input_ctx = player_ctx->input_ctx;
        this->window_width = GET_CONFIG(window_width);
        this->window_height = GET_CONFIG(window_height);
        this->_driver = driver::DriverFactory::create(GET_CONFIG(vo_driver));
        this->_driver->init(shared_from_this());
        this->running = true;
        this->_thread.run([&](){
            do{} while(this->loop());
        });
    }

    input::input_ctx_sptr VideoOutput::getInputCtx() {
        return this->_input_ctx;
    }

    bool VideoOutput::loop() {
        if (this->running) {
            if (this->needReConfig) {
                this->_driver->reConfig(shared_from_this());
                this->needReConfig = false;
            }
            if (this->queue->read(this->_frame)) {
                if (this->_frame->imgfmt != this->imgfmt) {
                    this->imgfmt = this->_frame->imgfmt;
                    this->_driver->reConfig(shared_from_this());
                }
                if (this->_frame->height != this->img_height || this->_frame->pitch != this->img_pitch) {
                    this->img_height = this->_frame->height;
                    this->img_pitch = this->_frame->pitch;
                    this->_driver->reConfig(shared_from_this());
                }
                this->frame_rendering = this->_frame;
                this->_frame = nullptr;
            }
            this->_driver->drawImage(shared_from_this());
            this->frame_rendering = nullptr;
        }
        return this->running;
    }

    void VideoOutput::loopInMainThread() {
        this->_driver->waitEvents(shared_from_this());
    }

}
