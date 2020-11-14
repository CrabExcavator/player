//
// Created by CrabExcavator on 2020/11/3.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "Universal.h"
#include "input/InputContext.h"
#include "player/PlayerContext.h"

namespace input::handler {

common::Error Universal::filter(const misc::vector_sptr<input::input_ctx_sptr> &in,
                                misc::vector_sptr<input::input_ctx_sptr> &out) {
  assert(in->size() == 1);
  auto input_ctx = in->at(0);
  auto player_ctx = getPlayContext(input_ctx);
  if (player_ctx == nullptr) {
    return common::Error::exit;
  }
  if (input_ctx->hasEvent(input::event::exit)) {
    player_ctx->stopRunning();
    return common::Error::exit;
  }
  if (input_ctx->hasEvent(input::event::nextEntry)) {
    input_ctx->nextEntry();
    input_ctx->clearEvent(input::event::nextEntry);
  }
  return common::Error::SUCCESS;
}

}