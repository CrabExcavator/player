//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "VideoOutput.h"
#include "video/driver/DriverFactory.h"
#include "common/Config.h"
#include "common/SyncContext.h"
#include "player/PlayerContext.h"
#include "demux/frame/IFrame.h"
#include "demux/stream/IStream.h"

namespace video {

static common::sync_ctx_sptr _sync = nullptr;

VideoOutput::VideoOutput() : thread_("vo") {

}

common::Error VideoOutput::Init(const player::player_ctx_sptr &player_ctx) {
  this->input_ctx_ = player_ctx->input_ctx;
  this->window_width_ = GET_CONFIG(window_width);
  this->window_height_ = GET_CONFIG(window_height);
  this->driver_ = driver::DriverFactory::create(GET_CONFIG(vo_driver));
  this->driver_->init(shared_from_this());
  this->sync_ctx_ = player_ctx->sync_ctx;
  this->version_ = player_ctx->sync_ctx->version;
  this->running_ = true;
  this->thread_.run([&]() {
    do {} while (this->Loop());
  });
  return common::Error::SUCCESS;
}

input::input_ctx_sptr VideoOutput::GetInputCtx() {
  return this->input_ctx_;
}

bool VideoOutput::Loop() {
  if (this->running_) {
    /// force reConfig
    if (this->need_re_config_) {
      this->driver_->reConfig(shared_from_this());
      this->need_re_config_ = false;
    }

    if (this->frame_ != nullptr && !this->frame_->IsLast()) {
      /**
       * we can not just dive into playback code with sync reason
       * sync with system clock
       */
      auto rendering_time = (this->frame_->GetPts() - this->last_pts_) * this->time_base_ + this->last_tick_;
      std::this_thread::sleep_until(rendering_time);
      this->last_tick_ = std::chrono::steady_clock::now();
      this->last_pts_ = this->frame_->GetPts();

      /**
       * we can not just dive into playback code with sync reason
       * sync with other output
       */
      //_sync->wait();

      /// playback
      this->frame_rendering_ = this->frame_;
      this->driver_->drawImage(shared_from_this());
      this->frame_rendering_ = nullptr;
      /// playback

      this->frame_ = nullptr;
    }

    if (this->version_ != this->sync_ctx_->version) {
      this->sync_ctx_->getVideoStream(this->stream_);
      this->version_ = this->sync_ctx_->version;
    }

    if (this->stream_ != nullptr && this->stream_->Read(this->frame_) == common::Error::SUCCESS) {
      /**
       * setNumOfStream driver if some args not match, idk is it ok putting in first frame
       */
      if (this->frame_->GetImageFormat() != this->image_format_) {
        this->image_format_ = this->frame_->GetImageFormat();
        this->driver_->reConfig(shared_from_this());
      }
      if (this->frame_->GetHeight() != this->img_height_ || this->frame_->GetWidth() != this->img_pitch_) {
        this->img_height_ = this->frame_->GetHeight();
        this->img_pitch_ = this->frame_->GetWidth();
        this->driver_->reConfig(shared_from_this());
      }

      /**
       * we should do something for first frame
       * @attention the first frame always carry data
       */
      if (this->frame_->IsFirst()) {
        this->last_tick_ = std::chrono::steady_clock::now() + std::chrono::seconds(1);
        this->last_pts_ = 0;
        this->time_base_ = this->stream_->GetTimeBase();
      }

      /**
       * we should do something for last frame
       * @attention the last frame never carry data
       */
      if (this->frame_->IsLast()) {
        /// @todo do something
        this->stream_ = nullptr;
      }
    }
  }
  return this->running_;
}

common::Error VideoOutput::LoopInMainThread() {
  this->driver_->waitEvents(shared_from_this());
  return common::Error::SUCCESS;
}

common::Error VideoOutput::StopRunning() {
  this->running_ = false;
  this->thread_.join();
  return common::Error::SUCCESS;
}

}
