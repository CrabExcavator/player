//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_PLAYERCONTEXT_H
#define PLAYER_PLAYERCONTEXT_H

#include <memory>
#include <folly/MPMCQueue.h>

#include "misc/typeptr.h"
#include "common/Error.h"

namespace core {

/**
 * @brief player context
 */
class PlayerContext : public std::enable_shared_from_this<PlayerContext> {
 public:
  /**
   * @brief default
   */
  PlayerContext() = default;

  /**
   * @brief init player context && start threads
   * @return error code
   */
  common::Error init();

  /**
   * @brief run main loop
   */
  void run();

  /**
   * @brief stop main thread
   * @return error code
   */
  common::Error stopRunning();

 public:
  /**
   * @brief sync
   */
  sync_ctx_sptr sync_ctx;

  /**
   * @brief play list
   */
  play_list_sptr play_list;

  /**
   * @brief input context
   */
  input::input_ctx_sptr input_ctx;

 private:
  /**
   * @brief loop for one tick
   * @return is running
   */
  bool loop();

 private:
  /**
   * @brief video output
   */
  video::vo_sptr _vo;

  /**
   * @brief audio output
   */
  audio::ao_sptr _ao;

  /**
   * @brief demux context
   */
  demux::demux_ctx_sptr _demux_ctx;
};

}

#endif //PLAYER_PLAYERCONTEXT_H
