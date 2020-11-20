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

/**
 * @brief Audio output
 */
class AudioOutput : public IOutput, public std::enable_shared_from_this<AudioOutput> {
 public:
  AudioOutput() = default;
  AudioOutput(const AudioOutput &rhs) = delete;
  AudioOutput(AudioOutput &&rhs) = delete;
  AudioOutput &operator=(const AudioOutput &rhs) = delete;
  AudioOutput &operator=(AudioOutput &&rhs) = delete;
  ~AudioOutput() override = default;

  /**
   * @brief setNumOfStream audio output
   * @param [in] sync_ctx player context
   * @return error code
   */
  common::Error Init(const common::sync_ctx_sptr &sync_ctx);

  common::Error Run() override;

  /**
   * @brief stop audio playback thread
   * @return error code
   */
  common::Error Stop();

 public:

  /**
   * @brief frame is playing, always used by audio driver
   */
  demux::frame::frame_sptr frame_playing_ = nullptr;

  /**
   * @brief flag to mark force reConfig
   */
  bool need_re_config_ = false;

  /**
   * @brief sample format
   */
  SampleFormat sample_format_ = SampleFormat::UNKNOWN;

  /**
   * @brief number of channel
   */
  int num_of_channel_ = 0;

  /**
   * @brief the size of each sample in each channel
   */
  int size_of_sample_ = 0;

  /**
   * @brief sample rate
   */
  int sample_rate_ = 0;

 protected:
  bool LoopImpl() override;

 private:
  /**
   * @brief flag to mark is audio playback thread running
   */
  bool running_ = false;

  /**
   * @brief impl of driver
   */
  audio::driver::audio_driver_uptr driver_ = nullptr;

  uint64_t version_ = 0;

  common::sync_ctx_sptr sync_ctx_ = nullptr;

  demux::frame::frame_sptr frame_ = nullptr;

  demux::stream::stream_sptr stream_ = nullptr;
};

}

#endif //PLAYER_AUDIOOUTPUT_H
