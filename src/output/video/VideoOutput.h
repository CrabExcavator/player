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

class VideoOutput : public output::IOutput, public std::enable_shared_from_this<VideoOutput> {
 public:
  VideoOutput();
  VideoOutput(const VideoOutput &rhs) = delete;
  VideoOutput(VideoOutput &&rhs) = default;
  VideoOutput &operator=(const VideoOutput &rhs) = delete;
  VideoOutput &operator=(VideoOutput &&rhs) = delete;
  ~VideoOutput() override = default;

  common::Error Init(const input::input_ctx_sptr &input_ctx);
  common::Error Run() override;
  input::input_ctx_sptr GetInputCtx();
  common::Error LoopInMainThread();
  common::Error Stop();

 public:
  demux::frame::frame_sptr frame_rendering_;

  ImageFormat image_format_;
  int window_width_;
  int window_height_;
  int img_pitch_;
  int img_height_;

 protected:
  bool LoopImpl() override;

 private:
  bool running_;
  driver::video_driver_uptr driver_;
  input::input_ctx_sptr input_ctx_;
  demux::frame::frame_sptr frame_;
  demux::stream::stream_sptr stream_;

  std::chrono::steady_clock::time_point last_tick_;
  std::chrono::steady_clock::duration time_base_;
  int64_t last_pts_;
};

}

#endif //PLAYER_VIDEOOUTPUT_H
