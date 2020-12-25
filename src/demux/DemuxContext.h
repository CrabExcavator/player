//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_DEMUXCONTEXT_H
#define PLAYER_DEMUXCONTEXT_H

#include <memory>

#include "player/PlayList.h"
#include "misc/Thread.h"
#include "misc/typeptr.h"
#include "common/Error.h"
#include "misc/Looper.h"
#include "misc/Runnable.h"

namespace demux {

/**
 * @brief runtime demux
 */
class DemuxContext : public misc::Runnable, public misc::Looper<60>, public std::enable_shared_from_this<DemuxContext> {
 public:
  /**
   * @brief default
   */
  DemuxContext() = default;

  /**
   * @brief setNumOfStream
   * @param [in] player_ctx
   * @return error code
   */
  common::Error Init(const input::input_ctx_sptr &input_ctx);

  /**
   * @brief run
   * @return error code
   */
  common::Error Run() override;

  /**
   * @brief stop demux thread
   * @return error code
   */
  common::Error Stop();

 protected:
  bool LoopImpl() override;

 private:
  /**
   * @brief demuxer for entry
   */
  demuxer::demuxer_sptr demuxer_;

  /**
   * @brief input context
   */
  input::input_ctx_sptr input_context_;

  /**
   * @brief flag to mark is demux thread running
   */
  std::atomic<bool> running_ = false;
};

}

#endif //PLAYER_DEMUXCONTEXT_H
