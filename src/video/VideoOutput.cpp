//
// Created by weihan on 2020/10/7.
//

#include "VideoOutput.h"
#include "video/driver/DriverFactory.h"
#include "common/Config.h"

namespace video {

    VideoOutput::VideoOutput(input::input_ctx_sptr input_ctx): _input_ctx(std::move(input_ctx)) {
        this->window_width = GET_CONFIG(window_width);
        this->window_height = GET_CONFIG(window_height);
    }

    VideoOutput::VideoOutput(VideoOutput &&rhs) noexcept: _input_ctx(std::move(rhs._input_ctx)) {

    }

    VideoOutput &VideoOutput::operator=(VideoOutput &&rhs) noexcept {
        this->_input_ctx = std::move(rhs._input_ctx);
        return *this;
    }

    void VideoOutput::init() {
        this->_driver = driver::DriverFactory::create("air");
        this->_driver->init(shared_from_this());
    }

    input::input_ctx_sptr VideoOutput::getInputCtx() {
        return this->_input_ctx;
    }

    void VideoOutput::loop() {
        this->_driver->drawImage(shared_from_this());
        this->_driver->waitEvents(shared_from_this());
    }

}
