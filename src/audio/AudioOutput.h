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
#include "audio/driver/AudioDriver.h"
#include "misc/typeptr.h"
#include "misc/Thread.h"
#include "common/Error.h"

namespace audio {

/**
 * @brief Audio output
 */
class AudioOutput : public std::enable_shared_from_this<AudioOutput> {
 public:
  /**
   * @brief constructor do nothing
   */
  AudioOutput();

  /**
   * @brief delete
   * @param rhs
   */
  AudioOutput(const AudioOutput &rhs) = delete;

  /**
   * @brief delete
   * @param rhs
   */
  AudioOutput(AudioOutput &&rhs) = delete;

  /**
   * @brief delete
   * @param rhs
   * @return
   */
  AudioOutput &operator=(const AudioOutput &rhs) = delete;

  /**
   * @brief delete
   * @param rhs
   * @return
   */
  AudioOutput &operator=(AudioOutput &&rhs) = delete;

  /**
   * @brief default deConstructor
   */
  ~AudioOutput() = default;

  /**
   * @brief setNumOfStream audio output
   * @param [in] player_ctx player context
   * @return error code
   */
  common::Error Init(const core::player_ctx_sptr &player_ctx);

  /**
   * @brief stop audio playback thread
   * @return error code
   */
  common::Error StopRunning();

 public:

  /**
   * @brief frame is playing, always used by audio driver
   */
  demux::frame_sptr frame_playing_ = nullptr;

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

 private:
  bool Loop();

 private:
  /**
   * @brief flag to mark is audio playback thread running
   */
  bool running_ = false;

  /**
   * @brief audio playback thread
   */
  misc::Thread thread_;

  /**
   * @brief impl of driver
   */
  audio::driver::audio_driver_uptr driver_ = nullptr;

  uint64_t version_ = 0;

  core::sync_ctx_sptr sync_ctx_ = nullptr;

  demux::frame_sptr frame_ = nullptr;

  demux::stream::stream_sptr stream_ = nullptr;
};

}

#endif //PLAYER_AUDIOOUTPUT_H
