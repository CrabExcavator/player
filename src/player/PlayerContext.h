//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_PLAYERCONTEXT_H
#define PLAYER_PLAYERCONTEXT_H

#include <memory>
#include <folly/MPMCQueue.h>

#include "misc/typeptr.h"
#include "misc/Runnable.h"
#include "common/Error.h"
#include "misc/Future.h"
#include "misc/Ticker.h"
#include "misc/Looper.h"

namespace player {

/**
 * @brief player context
 */
class PlayerContext : public misc::Runnable, public misc::Looper<60>, public std::enable_shared_from_this<PlayerContext> {
 public:
  /**
   * @brief default
   */
  PlayerContext() = default;

  /**
   * @brief init player context && start threads
   * @return error code
   */
  common::Error Init();

  /**
   * @brief run main loop
   */
  common::Error Run() override;

  /**
   * @brief stop main thread
   * @return error code
   */
  common::Error Stop();

 protected:
  /**
   * @brief loop for one tick
   * @return is running
   */
  bool LoopImpl() override;

 private:
  /**
   * @brief sync
   */
  common::sync_ctx_sptr sync_ctx_;

  /**
   * @brief input context
   */
  input::input_ctx_sptr input_ctx_;

  /**
   * @brief event_handler;
   */
  input::handler::event_handler_chain_sptr event_handler_;

  /**
   * @brief video output
   */
  video::vo_sptr vo_;

  /**
   * @brief audio output
   */
  output::audio::ao_sptr ao_;

  /**
   * @brief demux context
   */
  demux::demux_ctx_sptr demux_ctx_;

  /**
   * @brief all runners
   */
  std::vector<misc::future_node_sptr> runners;
};

}

#endif //PLAYER_PLAYERCONTEXT_H
