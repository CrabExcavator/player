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
  Universal() = default;

  /**
   * handle event
   * @param in
   * @param out
   * @return
   */
  common::Error filter(const misc::vector_sptr<input_ctx_sptr> &in, misc::vector_sptr<input_ctx_sptr> &out) override;

};

}

#endif //PLAYER_UNIVERSAL_H
