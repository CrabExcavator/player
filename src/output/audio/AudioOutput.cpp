//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "AudioOutput.h"
#include "driver/DriverFactory.h"
#include "common/Config.h"
#include "demux/frame/IFrame.h"
#include "demux/stream/IStream.h"
#include "common/Slots.h"

namespace output::audio {

common::Error AudioOutput::Init(const common::sync_ctx_sptr &sync_ctx) {
  this->driver_ = driver::DriverFactory::create(GET_CONFIG(ao_driver));
  this->version_ = sync_ctx->version;
  this->sync_ctx_ = sync_ctx;
  this->running_ = true;
  this->AdjustHZ(GET_CONFIG(default_ticker_hz));
  return common::Error::SUCCESS;
}

common::Error AudioOutput::Run() {
  auto ret = common::Error::SUCCESS;

  do {} while (this->Loop());
  return ret;
}

common::Error AudioOutput::Stop() {
  this->running_ = false;
  return common::Error::SUCCESS;
}

bool AudioOutput::LoopImpl() {
  if (this->running_) {
    /// force reConfig
    if (this->need_re_config_) {
      this->driver_->ReConfig(shared_from_this());
      this->need_re_config_ = false;
    }

    if (this->frame_ != nullptr && !this->frame_->IsLast()) {

      /// start playback
      this->frame_playing_ = this->frame_;
      this->driver_->Play(shared_from_this());
      this->frame_playing_ = nullptr;
      /// end playback

      this->frame_ = nullptr;
    }

    if (this->stream_ != nullptr &&
        common::Error::SUCCESS == this->stream_->Read(this->frame_)) {
      /**
       * we should do something for first frame
       * @attention the first frame always carry data
       */
      if (this->frame_->IsFirst()) {
        this->sample_format_ = this->frame_->GetSampleFormat();
        this->num_of_channel_ = this->frame_->GetNumOfChannel();
        this->size_of_sample_ = this->frame_->GetSampleSize();
        this->sample_rate_ = this->frame_->GetSampleRate();
        this->driver_->Init(shared_from_this());
      }

      /**
       * we should do something for last frame
       * @attentionq the last frame never carry data
       */
      if (this->frame_->IsLast()) {
        /// @todo do something
        this->stream_ = nullptr;
      }
    }

    /// audio output ctl
    common::Signal signal;
    if (GET_FROM_SLOT(AUDIO_OUTPUT_CTL_SLOT, signal)) {
      if (common::Signal::NEXT_STREAM == signal) {
        this->stream_ = BLOCKING_GET_FROM_SLOT(AUDIO_OUTPUT_STREAM_SLOT);
      }
    }
  }
  return this->running_;
}

}
