//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "AudioOutput.h"
#include "driver/DriverFactory.h"
#include "common/Config.h"
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
    if (nullptr != frame_ && !frame_->IsLast()) {
      if (frame_->IsFirst()) {
        driver_->Init(shared_from_this());
        UpdateDesc(frame_);
        tool::resample::Desc dst_desc{}, src_desc{};
        if (common::Error::SUCCESS == driver_->GetDesc(shared_from_this(), dst_desc)) {
          auto ff_resample = std::make_shared<tool::resample::FFResample>();
          FillDesc(frame_, src_desc);
          ff_resample->Init(src_desc, dst_desc);
          resample_ = ff_resample;
        }
      }

      if (nullptr != resample_) {
        tool::resample::resample_output_sptr resample_output = nullptr;
        frame_->DoResample(resample_, resample_output);
        auto resample_frame = std::make_shared<demux::frame::ResampledFrame>();
        resample_frame->Init(frame_, resample_output);
        frame_ = resample_frame;
        if (frame_->IsFirst()) {
          UpdateDesc(frame_);
        }
      }

      if (frame_->IsFirst()) {
        driver_->Open(shared_from_this());
      }

      /// start Playback
      frame_playing_ = frame_;
      driver_->Play(shared_from_this());
      frame_playing_ = nullptr;
      /// end Playback

      frame_ = nullptr;
    }

    if (nullptr != stream_ &&
        common::Error::SUCCESS == stream_->Read(frame_)) {

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

void AudioOutput::UpdateDesc(const demux::frame::frame_sptr& frame) {
  sample_format_ = frame->GetSampleFormat();
  num_of_channel_ = frame->GetNumOfChannel();
  size_of_sample_ = frame->GetSampleSize();
  sample_rate_ = frame->GetSampleRate();
  channel_layout_ = frame->GetChannelLayout();
}

void AudioOutput::FillDesc(const demux::frame::frame_sptr &frame, tool::resample::Desc &desc) {
  desc.sample_format = frame->GetSampleFormat();
  desc.layout = frame->GetChannelLayout();
  desc.sample_rate = frame->GetSampleRate();
  desc.number_of_channel = frame->GetNumOfChannel();
  desc.number_of_sample = frame->GetNumOfSample();
  desc.linesize = frame->GetAudioLineSize();
}

}
