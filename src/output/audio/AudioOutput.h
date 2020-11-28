//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_AUDIOOUTPUT_H
#define PLAYER_AUDIOOUTPUT_H

#include <memory>
#include <chrono>
#include <folly/MPMCQueue.h>

#include "SampleFormat.h"
#include "output/audio/driver/AudioDriver.h"
#include "misc/typeptr.h"
#include "common/Error.h"
#include "misc/Ticker.h"
#include "output/IOutput.h"

namespace output::audio {

class AudioOutput : public IOutput, public std::enable_shared_from_this<AudioOutput> {
 public:
  AudioOutput();
  AudioOutput(const AudioOutput &rhs) = delete;
  AudioOutput(AudioOutput &&rhs) = delete;
  AudioOutput &operator=(const AudioOutput &rhs) = delete;
  AudioOutput &operator=(AudioOutput &&rhs) = delete;
  ~AudioOutput() override = default;

  common::Error Init();
  common::Error Run() override;
  common::Error Stop();

 public:
  demux::frame::frame_sptr frame_playing_ = nullptr;

  SampleFormat sample_format_;
  int num_of_channel_;
  int size_of_sample_;
  int sample_rate_;
  ChannelLayout channel_layout_;

 protected:
  bool LoopImpl() override;

 private:
  bool running_;
  audio::driver::audio_driver_uptr driver_;
  demux::frame::frame_sptr frame_;
  demux::stream::stream_sptr stream_;
  tool::resample::resample_sptr resample_;
};

}

#endif //PLAYER_AUDIOOUTPUT_H
