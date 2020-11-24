//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "VideoOutput.h"
#include "driver/DriverFactory.h"
#include "common/Config.h"
#include "demux/frame/IFrame.h"
#include "demux/stream/IStream.h"
#include "common/Slots.h"

namespace output::video {

common::Error VideoOutput::Init(const input::input_ctx_sptr &input_ctx) {
  auto ret = common::Error::SUCCESS;
  input_ctx_ = input_ctx;
  window_width_ = GET_CONFIG(window_width);
  window_height_ = GET_CONFIG(window_height);
  driver_ = driver::DriverFactory::create(GET_CONFIG(vo_driver));
  running_ = true;

  if (common::Error::SUCCESS != driver_->init(shared_from_this())) {
    ret = common::Error::UNKNOWN_ERROR;
  } else {
    AdjustHZ(GET_CONFIG(default_tick_hz));
  }

  return ret;
}

common::Error VideoOutput::Run() {
  do {} while (LoopImpl());
  return common::Error::SUCCESS;
}

common::Error VideoOutput::Stop() {
  running_ = false;
  return common::Error::SUCCESS;
}

input::input_ctx_sptr VideoOutput::GetInputCtx() {
  return input_ctx_;
}

bool VideoOutput::LoopImpl() {
  if (running_) {
    /// force reConfig
    if (need_re_config_) {
      driver_->reConfig(shared_from_this());
      need_re_config_ = false;
    }

    if (nullptr != frame_ && !frame_->IsLast()) {
      /**
       * we can not just dive into playback code with sync reason
       * sync with system clock
       */
      auto rendering_time = (frame_->GetPts() - last_pts_) * time_base_ + last_tick_;
      std::this_thread::sleep_until(rendering_time);
      last_tick_ = std::chrono::steady_clock::now();
      last_pts_ = frame_->GetPts();

      /// playback
      frame_rendering_ = frame_;
      driver_->drawImage(shared_from_this());
      frame_rendering_ = nullptr;
      /// playback

      frame_ = nullptr;
    }

    if (nullptr != stream_ &&
        common::Error::SUCCESS == stream_->Read(frame_)) {
      /**
       * we should do something for first frame
       * @attention the first frame always carry data
       */
      if (frame_->IsFirst()) {
        image_format_ = frame_->GetImageFormat();
        driver_->reConfig(shared_from_this());
        img_height_ = frame_->GetHeight();
        img_pitch_ = frame_->GetWidth();
        driver_->reConfig(shared_from_this());
        last_tick_ = std::chrono::steady_clock::now() + std::chrono::seconds(1);
        last_pts_ = 0;
        time_base_ = stream_->GetTimeBase();
      }

      /**
       * we should do something for last frame
       * @attention the last frame never carry data
       */
      if (frame_->IsLast()) {
        /// @todo do something
        stream_ = nullptr;
      }
    }

    /// video output ctl
    common::Signal signal;
    if (GET_FROM_SLOT(VIDEO_OUTPUT_CTL_SLOT, signal)) {
      if (common::Signal::NEXT_STREAM == signal) {
        stream_ = BLOCKING_GET_FROM_SLOT(VIDEO_OUTPUT_STREAM_SLOT);
      }
    }
  }
  return running_;
}

common::Error VideoOutput::LoopInMainThread() {
  driver_->waitEvents(shared_from_this());
  return common::Error::SUCCESS;
}

}
