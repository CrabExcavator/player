//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "AudioOutput.h"
#include "driver/DriverFactory.h"
#include "common/Config.h"
#include "demux/frame/IFrame.h"
#include "demux/frame/ResampledFrame.h"
#include "demux/stream/IStream.h"
#include "common/Slots.h"
#include "tool/resample/FFResample.h"

namespace output::audio {

AudioOutput::AudioOutput():
frame_playing_(nullptr),
sample_format_(SampleFormat::UNKNOWN),
num_of_channel_(0),
size_of_sample_(0),
sample_rate_(0),
channel_layout_(ChannelLayout::UNKNOWN),
running_(false),
driver_(nullptr),
frame_(nullptr),
stream_(nullptr),
resample_(nullptr){}

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
    if (frame_ != nullptr && !frame_->IsLast()) {
      tool::resample::resample_output_sptr resample_output = nullptr;
      frame_->DoResample(resample_, resample_output);
      auto resample_frame = std::make_shared<demux::frame::ResampledFrame>();
      resample_frame->Init(frame_, resample_output);
      frame_ = resample_frame;
      if (frame_->IsFirst()) {
        sample_format_ = frame_->GetSampleFormat();
        num_of_channel_ = frame_->GetNumOfChannel();
        size_of_sample_ = frame_->GetSampleSize();
        sample_rate_ = frame_->GetSampleRate();
        driver_->Open(shared_from_this());
      }

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
        // init audio output
        sample_format_ = frame_->GetSampleFormat();
        num_of_channel_ = frame_->GetNumOfChannel();
        size_of_sample_ = frame_->GetSampleSize();
        sample_rate_ = frame_->GetSampleRate();
        channel_layout_ = frame_->GetChannelLayout();
        driver_->Init(shared_from_this());
        // get desc
        tool::resample::Desc dst_desc{}, src_desc{};
        driver_->GetDesc(shared_from_this(), dst_desc);
        // init resample
        auto ff_resample = std::make_shared<tool::resample::FFResample>();
        src_desc.sample_format = frame_->GetSampleFormat();
        src_desc.sample_rate = frame_->GetSampleRate();
        src_desc.number_of_channel = frame_->GetNumOfChannel();
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
