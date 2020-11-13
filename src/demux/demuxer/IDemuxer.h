//
// Created by CrabExcavator on 2020/11/8.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_IDEMUXER_H
#define PLAYER_IDEMUXER_H

#include "common/Error.h"
#include "misc/typeptr.h"

namespace demux::demuxer {

/**
 * @brief the interface of Demuxer
 */
class IDemuxer {
 public:
  /**
   * @brief default
   */
  virtual ~IDemuxer() = default;

  /**
   * @brief open an entry
   * @param [in] entry
   * @param [out] streams
   * @return error code
   */
  virtual common::Error open(const core::play_entry_sptr &entry, misc::vector_sptr<stream::stream_sptr> &streams) = 0;

  /**
   * @brief parse next packet && push it to stream
   * @param [out] packet
   * @return error code
   */
  virtual common::Error epoch() = 0;

  /**
   * @brief close demuxer
   * @return error code
   */
  virtual common::Error close() = 0;
};

}

#endif //PLAYER_IDEMUXER_H
