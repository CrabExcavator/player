//
// Created by CrabExcavator on 2020/11/9.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_FFSTREAM_H
#define PLAYER_FFSTREAM_H

#include <folly/MPMCQueue.h>

#include "IStream.h"
#include "misc/avheader.h"

namespace demux::stream {

/**
 * @brief av stream
 */
class FFStream : public IStream {
 public:
  /**
   * @brief Init avS
   * @param stream
   * @return error code
   */
  common::Error Init(const AVStream *stream);

  /**
   * @brief read a frame
   * @param [out] frame
   * @return error code
   */
  common::Error Read(frame::frame_sptr &frame) override;

  /**
   * @brief Close stream
   * @return error code
   */
  common::Error Close() override;

  /**
   * @brief get time base of stream
   * @return time base
   */
  std::chrono::nanoseconds GetTimeBase() override;

  /**
   * @brief what type of output should this stream bind to
   * @return output port
   */
  output::OutputPort GetOutputPort() override;

  /**
   * @brief feed packet
   * @param [in] packet
   * @return error code
   */
  common::Error Feed(const av_packet_sptr &packet);

 private:
  bool first_ = true;

  AVCodec *codec_ = nullptr;

  av_codec_ctx_uptr codec_ctx_ = nullptr;

  std::shared_ptr<folly::MPMCQueue<frame::ffframe_sptr>> queue_ = nullptr;

  std::chrono::nanoseconds time_base_{};

  output::OutputPort op_ = output::OutputPort::EMPTY;
};

}

#endif //PLAYER_FFSTREAM_H
