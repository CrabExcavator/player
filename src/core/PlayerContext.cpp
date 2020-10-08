//
// Created by weihan on 2020/10/7.
//

#include "PlayerContext.h"
#include "PlayerContextImpl.h"

namespace core {

    PlayerContext::PlayerContext(): _impl(std::make_unique<PlayerContextImpl>()) {

    }

    PlayerContext::PlayerContext(PlayerContext &&rhs) noexcept: _impl(std::move(rhs._impl)) {
        rhs._impl = nullptr;
    }

    PlayerContext &PlayerContext::operator=(PlayerContext &&rhs) noexcept {
        this->_impl.swap(rhs._impl);
        rhs._impl = nullptr;
        return *this;
    }

    PlayerContext::~PlayerContext() {
        this->_impl = nullptr;
    }

    void PlayerContext::run() {
        this->_impl->run();
    }

}