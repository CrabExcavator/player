//
// Created by CrabExcavator on 2020/11/3.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "Universal.h"
#include "input/InputContext.h"
#include "player/PlayerContext.h"

namespace input::handler {

common::Error Universal::Init(const player::player_ctx_sptr &player_ctx) {
  this->player_ctx_ = player_ctx;
  return common::Error::SUCCESS;
}

common::Error Universal::Filter(const misc::vector_sptr<input::input_ctx_sptr> &in,
                                misc::vector_sptr<input::input_ctx_sptr> &out) {
  assert(in->size() == 1);
  auto input_ctx = in->at(0);
  auto player_ctx = this->player_ctx_.lock();
  if (player_ctx == nullptr) {
    return common::Error::EXIT;
  }
  if (input_ctx->HasEvent(Event::exit)) {
    player_ctx->Stop();
    return common::Error::EXIT;
  }
  return common::Error::SUCCESS;
}

}