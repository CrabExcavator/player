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
#include "output/IOutput.h"

namespace output::video {

/**
 * @brief video output
 */
 class VideoOutput : public output::IOutput, public std::enable_shared_from_this<VideoOutput> {
 public:
  /**
   * @brief default
   */
  VideoOutput() = default;

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
  ~VideoOutput() override = default;

  /**
   * @brief setNumOfStream video output
   * @param [in] sync_ctx
   * @return error code
   */
  common::Error Init(const input::input_ctx_sptr &input_ctx);

  /**
   * @brief run
   * @return error code
   */
  common::Error Run() override;

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
   * @brief stop video output
   * @return error code
   */
  common::Error Stop();

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
  bool LoopImpl() override;

 private:
  /**
   * @brief flag to mark running
   */
  bool running_ = false;

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

  demux::frame::frame_sptr frame_ = nullptr;

  demux::stream::stream_sptr stream_ = nullptr;
};

}

#endif //PLAYER_VIDEOOUTPUT_H
