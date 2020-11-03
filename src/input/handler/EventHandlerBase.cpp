//
// Created by CrabExcavator on 2020/11/3.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "EventHandlerBase.h"
#include "input/InputContext.h"

namespace input::handler {


    common::error
    EventHandlerBase::flush(const misc::vector_sptr<input_ctx_sptr> &in, misc::vector_sptr<input_ctx_sptr> &out) {
        return common::error::unknownError;
    }

    common::error EventHandlerBase::close() {
        return common::error::unknownError;
    }

    core::player_ctx_sptr EventHandlerBase::getPlayContext(const input_ctx_sptr &input_ctx) {
        return input_ctx->_player_ctx.lock();
    }
}
