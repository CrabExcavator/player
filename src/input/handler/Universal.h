//
// Created by CrabExcavator on 2020/11/3.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_UNIVERSAL_H
#define PLAYER_UNIVERSAL_H

#include "EventHandlerBase.h"

namespace input::handler {

/**
 * @brief universal event handler
 */
class Universal : public EventHandlerBase {
 public:
  common::Error Init(const player::player_ctx_sptr& player_ctx);

  /**
   * handle event
   * @param in
   * @param out
   * @return
   */
  common::Error Filter(const misc::vector_sptr<input_ctx_sptr> &in, misc::vector_sptr<input_ctx_sptr> &out) override;

 private:
  player::player_ctx_wptr player_ctx_;
};

}

#endif //PLAYER_UNIVERSAL_H
