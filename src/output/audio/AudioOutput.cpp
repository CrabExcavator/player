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
#include "tool/resample/FFResample.h"

namespace output::audio {

common::Error AudioOutput::Init() {
  driver_ = driver::DriverFactory::create(GET_CONFIG(ao_driver));
  running_ = true;
  AdjustHZ(UINT64_MAX);
  return common::Error::SUCCESS;
}

common::Error AudioOutput::Run() {
  auto ret = common::Error::SUCCESS;

  do {} while (Loop());
  return ret;
}

common::Error AudioOutput::Stop() {
  running_ = false;
  return common::Error::SUCCESS;
}

bool AudioOutput::LoopImpl() {
  if (running_) {
    /// force reConfig
    if (need_re_config_) {
      driver_->ReConfig(shared_from_this());
      need_re_config_ = false;
    }

    if (frame_ != nullptr && !frame_->IsLast()) {
      frame_->DoResample(resample_);

      /// start playback
      frame_playing_ = frame_;
      driver_->Play(shared_from_this());
      frame_playing_ = nullptr;
      /// end playback

      frame_ = nullptr;
    }

    if (nullptr != stream_ &&
        common::Error::SUCCESS == stream_->Read(frame_)) {
      /**
       * we should do something for first frame
       * @attention the first frame always carry data
       */
      if (frame_->IsFirst()) {
        sample_format_ = frame_->GetSampleFormat();
        num_of_channel_ = frame_->GetNumOfChannel();
        size_of_sample_ = frame_->GetSampleSize();
        sample_rate_ = frame_->GetSampleRate();
        driver_->Init(shared_from_this());
        tool::resample::Desc dst_desc{}, src_desc{};
        driver_->GetDesc(shared_from_this(), dst_desc);
        auto ff_resample = std::make_shared<tool::resample::FFResample>();
        src_desc = dst_desc;
        src_desc.sample_rate = frame_->GetSampleRate();
        src_desc.number_of_channel = num_of_channel_;
        src_desc.number_of_sample = frame_->GetNumOfSample();
        src_desc.linesize = frame_->GetAudioLineSize();
        src_desc.layout = frame_->GetChannelLayout();
        ff_resample->Init(src_desc, dst_desc);
        resample_ = ff_resample;
      }

      /**
       * we should do something for last frame
       * @attentionq the last frame never carry data
       */
      if (frame_->IsLast()) {
        /// @todo do something
        stream_ = nullptr;
      }
    }

    /// audio output ctl
    common::Signal signal;
    if (GET_FROM_SLOT(AUDIO_OUTPUT_CTL_SLOT, signal)) {
      if (common::Signal::NEXT_STREAM == signal) {
        stream_ = BLOCKING_GET_FROM_SLOT(AUDIO_OUTPUT_STREAM_SLOT);
      }
    }
  }
  return running_;
}

}
