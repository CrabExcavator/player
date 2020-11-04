//
// Created by CrabExcavator on 2020/11/3.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "Universal.h"
#include "input/InputContext.h"
#include "core/PlayerContext.h"

namespace input::handler {

    common::error Universal::filter(const misc::vector_sptr<input::input_ctx_sptr> &in,
                                                    misc::vector_sptr<input::input_ctx_sptr> &out) {
        assert(in->size() == 1);
        auto input_ctx = in->at(0);
        auto player_ctx = getPlayContext(input_ctx);
        if (player_ctx == nullptr) {
            return common::error::exit;
        }
        if (input_ctx->hasEvent(input::event::exit)) {
            player_ctx->stopRunning();
            return common::error::exit;
        }
        input_ctx->clearAllEvent();
        return common::error::success;
    }

}