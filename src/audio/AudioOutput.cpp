//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "AudioOutput.h"
#include "driver/DriverFactory.h"
#include "common/Config.h"
#include "core/PlayerContext.h"
#include "demux/Frame.h"
#include "demux/stream/IStream.h"

namespace audio {

AudioOutput::AudioOutput() : thread_("audio output") {

}

common::Error AudioOutput::Init(const core::player_ctx_sptr &player_ctx) {
  this->driver_ = driver::DriverFactory::create(GET_CONFIG(ao_driver));
  this->version_ = player_ctx->sync_ctx->version;
  this->sync_ctx_ = player_ctx->sync_ctx;
  this->running_ = true;
  this->thread_.run([&]() {
    do {} while (this->Loop());
  });
  return common::Error::SUCCESS;
}

common::Error AudioOutput::StopRunning() {
  this->running_ = false;
  this->thread_.join();
  return common::Error::SUCCESS;
}

bool AudioOutput::Loop() {
  if (this->running_) {
    /// force reConfig
    if (this->need_re_config_) {
      this->driver_->reConfig(shared_from_this());
      this->need_re_config_ = false;
    }

    if (this->frame_ != nullptr) {
      /**
       * we can not just dive into playback code with sync reason
       * sync with other output
       * @todo use audio clock for sync reason
       */
      //_sync->wait();

      /// start playback
      this->frame_playing_ = this->frame_;
      this->driver_->play(shared_from_this());
      this->frame_playing_ = nullptr;
      /// end playback

      this->frame_ = nullptr;
    }

    /// update output version
    if (this->version_ != this->sync_ctx_->version) {
      this->sync_ctx_->getAudioStream(this->stream_);
      this->version_ = this->sync_ctx_->version;
    }

    if (this->stream_ != nullptr &&
        this->stream_->read(this->frame_) == common::Error::SUCCESS) {
      /**
       * we should do something for first frame
       * @attention the first frame always carry data
       */
      if (this->frame_->first) {
        this->sample_format_ = this->frame_->sample_fmt;
        this->num_of_channel_ = this->frame_->num_of_channel;
        this->size_of_sample_ = this->frame_->sample_size;
        this->sample_rate_ = this->frame_->sample_rate;
        this->driver_->init(shared_from_this());
      }

      /**
       * we should do something for last frame
       * @attentionq the last frame never carry data
       */
      if (this->frame_->last) {
        /// @todo do something
        this->stream_ = nullptr;
      }
    }
  }
  return this->running_;
}

}
