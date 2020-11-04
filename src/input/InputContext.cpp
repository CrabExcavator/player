//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "InputContext.h"
#include "core/PlayerContext.h"
#include "misc/Chain.h"
#include "handler/Universal.h"
#include "core/PlayList.h"

namespace input {

    InputContext::InputContext(InputContext &&rhs) noexcept: _events(std::move(rhs._events)) {

    }

    InputContext &InputContext::operator=(InputContext &&rhs) noexcept {
        this->_events = rhs._events;
        return *this;
    }

    common::error InputContext::init(const core::player_ctx_sptr& player) {
        this->_player_ctx = player;
        this->_player_list = player->play_list;
        this->_handler_chain = std::make_shared<misc::Chain<input_ctx_sptr>>();
        this->_handler_chain->addLast(std::make_shared<handler::Universal>())
        ;
        return common::error::success;
    }

    void InputContext::receiveEvent(event ev) {
        this->_events.insert(ev);
    }

    bool InputContext::hasEvent(event ev) {
        return this->_events.contains(ev);
    }

    void InputContext::clearEvent(event ev) {
       this->_events.erase(ev);
    }

    void InputContext::clearAllEvent() {
        this->_events.clear();
    }

    common::error InputContext::handleEvent() {
        auto in = std::make_shared<std::vector<input_ctx_sptr>>();
        in->emplace_back(shared_from_this());
        misc::vector_sptr<input_ctx_sptr> out = nullptr;
        return this->_handler_chain->filter(in, out);
    }

    common::error InputContext::nextEntry() {
        this->_play_entry = this->_player_list->current();
        this->_player_list->next();
        return common::error::success;
    }

    common::error InputContext::getCurrent(core::play_entry_sptr &entry) {
        entry = this->_play_entry;
        this->_play_entry = nullptr;
        return common::error::success;
    }

}
