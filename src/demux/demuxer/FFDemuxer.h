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
  common::Error open(const core::play_entry_sptr &entry,
                     misc::vector_sptr<stream::stream_sptr> &streams) override;

  /**
   * @brief parse one packet
   * @param [out] packet
   * @return error code
   */
  common::Error epoch() override;

  /**
   * @brief close
   * @return error code
   */
  common::Error close() override;

 private:
  /**
   * @brief av format context from libavformat
   */
  std::shared_ptr<AVFormatContext> _av_format_ctx;

  /**
   * @brief ref to streams opened
   */
  misc::vector_sptr<stream::ffstream_sptr> _streams;

  /**
   * @brief packet to fill
   */
  av_packet_sptr _av_packet;
};

}

#endif //PLAYER_FFDEMUXER_H
