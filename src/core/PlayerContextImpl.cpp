//
// Created by weihan on 2020/10/7.
//

#include "PlayerContextImpl.h"
#include "video/VideoOutput.h"

namespace core {

    PlayerContextImpl::PlayerContextImpl() {
        this->_input_ctx = std::make_shared<input::InputContext>();
        this->_vo = std::make_shared<video::VideoOutput>(this->_input_ctx);
        this->_vo->init();
    }

    void PlayerContextImpl::run() {
        do{} while(this->loop());
    }

    bool PlayerContextImpl::loop() {
        this->_vo->loop();
        if (this->_input_ctx->hasEvent(input::event::exit)) {
            return false;
        }
        this->_input_ctx->clear();
        return true;
    }

}
