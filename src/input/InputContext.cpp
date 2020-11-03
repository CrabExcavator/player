//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "InputContext.h"
#include "core/PlayerContext.h"
#include "misc/Chain.h"
#include "handler/Universal.h"

namespace input {

    InputContext::InputContext() {
        this->_slots.reset();
    }

    InputContext::InputContext(InputContext &&rhs) noexcept: _slots(rhs._slots) {

    }

    InputContext &InputContext::operator=(InputContext &&rhs) noexcept {
        this->_slots = rhs._slots;
        return *this;
    }

    common::error InputContext::init(const core::player_ctx_sptr& player) {
        this->_player_ctx = player;
        this->_handler_chain = std::make_shared<misc::Chain<input_ctx_sptr>>();
        this->_handler_chain->addLast(std::make_shared<handler::Universal>())
        ;
        return common::error::success;
    }

    void InputContext::receive(event ev) {
        this->_slots[static_cast<int>(ev)] = true;
    }

    event_set InputContext::pollEvent() {
        auto ret = this->_slots;
        this->clear();
        return ret;
    }

    bool InputContext::hasEvent(event ev) {
        return this->_slots[static_cast<int>(ev)];
    }

    void InputContext::clearEvent(event ev) {
        this->_slots[static_cast<int>(ev)] = false;
    }

    void InputContext::clear() {
        this->_slots.reset();
    }

    common::error InputContext::handleEvent() {
        auto in = std::make_shared<std::vector<input_ctx_sptr>>();
        in->emplace_back(shared_from_this());
        misc::vector_sptr<input_ctx_sptr> out = nullptr;
        return this->_handler_chain->filter(in, out);
    }

}
