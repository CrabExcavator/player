//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_VIDEOOUTPUT_H
#define PLAYER_VIDEOOUTPUT_H

#include <memory>
#include <chrono>
#include <folly/MPMCQueue.h>

#include "ImageFormat.h"
#include "output/video/driver/VideoDriver.h"
#include "misc/Thread.h"
#include "misc/typeptr.h"
#include "common/Error.h"

namespace video {

/**
 * @brief video output
 */
class VideoOutput : public std::enable_shared_from_this<VideoOutput> {
 public:
  /**
   * @brief default
   */
  VideoOutput();

  /**
   * @brief delete
   * @param rhs
   */
  VideoOutput(const VideoOutput &rhs) = delete;

  /**
   * @brief default
   * @param rhs
   */
  VideoOutput(VideoOutput &&rhs) = default;

  /**
   * @brief delete
   * @param rhs
   * @return
   */
  VideoOutput &operator=(const VideoOutput &rhs) = delete;

  /**
   * @brief delete
   * @param rhs
   * @return
   */
  VideoOutput &operator=(VideoOutput &&rhs) = delete;

  /**
   * @brief default
   */
  ~VideoOutput() = default;

  /**
   * @brief setNumOfStream video output
   * @param [in] player_ctx
   * @return error code
   */
  common::Error Init(const player::player_ctx_sptr &player_ctx);

  /**
   * @brief get input context
   * @return input context
   */
  input::input_ctx_sptr GetInputCtx();

  /**
   * @brief some function in video output must loop in main thread
   * @return error code
   */
  common::Error LoopInMainThread();

  /**
   * @brief stop video output thread
   * @return error code
   */
  common::Error StopRunning();

 public:
  /**
   * @brief image format of video output
   */
  ImageFormat image_format_ = ImageFormat::unknown;

  /**
   * @brief window width
   */
  int window_width_ = 1920;

  /**
   * @brief window height
   */
  int window_height_ = 1080;

  /**
   * @brief image pitch
   */
  int img_pitch_ = 1920;

  /**
   * @brief image height
   */
  int img_height_ = 1080;

  /**
   * @brief rendering frame
   */
  demux::frame::frame_sptr frame_rendering_ = nullptr;

  /**
   * @brief flag to mark force reConfig
   */
  bool need_re_config_ = false;

 private:
  /**
   * @brief loop in tick
   * @return running
   */
  bool Loop();

 private:
  /**
   * @brief flag to mark running
   */
  bool running_ = false;

  /**
   * @brief thread
   */
  misc::Thread thread_;

  /**
   * @brief video driver
   */
  driver::video_driver_uptr driver_ = nullptr;

  /**
   * @brief last tick
   */
  std::chrono::steady_clock::time_point last_tick_{};

  /**
   * @brief time base
   */
  std::chrono::steady_clock::duration time_base_{};

  /**
   * @brief last pts
   */
  int64_t last_pts_ = 0;

  /**
   * @brief input context
   */
  input::input_ctx_sptr input_ctx_;

  uint64_t version_ = 0;

  common::sync_ctx_sptr sync_ctx_ = nullptr;

  demux::frame::frame_sptr frame_ = nullptr;

  demux::stream::stream_sptr stream_ = nullptr;
};

}

#endif //PLAYER_VIDEOOUTPUT_H
