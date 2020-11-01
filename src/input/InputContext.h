//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_INPUTCONTEXT_H
#define PLAYER_INPUTCONTEXT_H

#include <bitset>
#include <memory>
#include <glog/logging.h>

#include "event.h"

namespace input {

    using event_set = std::bitset<static_cast<size_t>(event::numOfEvent)>;
    class InputContext {
    public:
        InputContext();
        InputContext(const InputContext& rhs) = delete;
        InputContext(InputContext&& rhs) noexcept ;
        InputContext& operator = (const InputContext& rhs) = delete;
        InputContext& operator = (InputContext&& rhs) noexcept ;
        ~InputContext() = default;
        void receive(event ev);
        event_set pollEvent();
        bool hasEvent(event ev);
        void clear();
    private:
        event_set _slots;
    };
    
    using input_ctx_sptr = std::shared_ptr<InputContext>;
}

#endif //PLAYER_INPUTCONTEXT_H
