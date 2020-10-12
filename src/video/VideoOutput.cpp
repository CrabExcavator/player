//
// Created by weihan on 2020/10/7.
//

#include "VideoOutput.h"
#include "video/driver/DriverFactory.h"
#include "common/Config.h"
#include "core/PlayerContext.h"

namespace video {

    VideoOutput::VideoOutput(): _thread("vo") {

    }

    void VideoOutput::init(const core::player_ctx_sptr& player_ctx) {
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
            this->_driver->drawImage(shared_from_this());
        }
        return this->running;
    }

}
