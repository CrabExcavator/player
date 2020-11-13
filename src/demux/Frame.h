//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_FRAME_H
#define PLAYER_FRAME_H

#include <memory>
#include <chrono>
extern "C" {
#include <libavformat/avformat.h>
};

#include "audio/SampleFormat.h"
#include "video/ImageFormat.h"
#include "misc/typeptr.h"

namespace demux {

/**
 * @brief frame is basic unit of meaningful for data struct for player
 */
class Frame {
 public:
  /**
   * @brief allocate raw frame
   */
  Frame();

  /**
   * @brief default
   */
  ~Frame();

  /**
   * @brief delete
   * @param rhs
   */
  Frame(const Frame &rhs) = delete;

  /**
   * @brief delete
   * @param rhs
   */
  Frame(Frame &&rhs) = delete;

  /**
   * @brief delete
   * @param rhs
   * @return
   */
  Frame &operator=(const Frame &rhs) = delete;

  /**
   * @brief delete
   * @param rhs
   * @return
   */
  Frame &operator=(Frame &&rhs) = delete;

  /**
   * @brief get raw pointer
   * @return raw pointer
   * @todo hide raw pointer from player
   */
  AVFrame *raw() { return this->_frame; }

 public:
  /**
   * @brief is it the first frame of a stream
   */
  bool first = false;

  /**
   * @brief is it the last frame of a stream
   */
  bool last = false;

  /**
   * @brief sample format in audio frame
   */
  audio::SampleFormat sample_fmt = audio::SampleFormat::UNKNOWN;

  /**
   * @brief sample size in audio frame
   */
  int sample_size = 0;

  /**
   * @brief number of channel
   */
  int num_of_channel = 0;

  /**
   * @brief number of sample
   */
  int num_of_sample = 0;

  /**
   * @brief sample rate
   */
  int sample_rate = 0;

  /**
   * @brief image format in image frame
   */
  video::ImageFormat img_fmt = video::ImageFormat::unknown;

  /**
   * @brief transformed data
   */
  uint8_t *pixels = nullptr;

  /**
   * @brief pitch could be considered as the width of pic
   */
  int pitch = 0;

  /**
   * @brief height of pic
   */
  int height = 0;

  /**
   * @brief time base of frame
   */
  std::chrono::steady_clock::duration time_base{};

  /**
   * @brief pts of frame
   */
  int64_t pts = 0;

 private:
  /**
   * @brief raw frame in ffmpeg
   */
  AVFrame *_frame;
};

}

#endif //PLAYER_FRAME_H
