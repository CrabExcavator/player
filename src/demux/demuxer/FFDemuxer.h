//
// Created by CrabExcavator on 2020/11/8.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_FFDEMUXER_H
#define PLAYER_FFDEMUXER_H

#include "IDemuxer.h"
#include "misc/typeptr.h"
#include "misc/avheader.h"
#include "demux/stream/FFStream.h"

namespace demux::demuxer {

/**
 * @brief impl of demuxer using libavformat from project ffmpeg
 */
class FFDemuxer : public IDemuxer {
 public:
  /**
   * @brief open entry
   * @param [in] entry
   * @param [out] streams
   * @return error code
   */
  common::Error Open(const player::play_entry_sptr &entry,
                     misc::vector_sptr<stream::stream_sptr> &streams) override;

  /**
   * @brief parse one packet
   * @param [out] packet
   * @return error code
   */
  common::Error Epoch() override;

  /**
   * @brief Close
   * @return error code
   */
  common::Error Close() override;

 private:
  /**
   * @brief av format context from libavformat
   */
  std::shared_ptr<AVFormatContext> av_format_ctx_;

  /**
   * @brief ref to streams opened
   */
  misc::vector_sptr<stream::ffstream_sptr> streams_;

  /**
   * @brief packet to fill
   */
  av_packet_sptr av_packet_;
};

}

#endif //PLAYER_FFDEMUXER_H
