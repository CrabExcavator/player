//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_AUDIODRIVER_H
#define PLAYER_AUDIODRIVER_H

#include <memory>

#include "misc/typeptr.h"
#include "common/Error.h"
#include "tool/resample/IResample.h"

namespace output::audio::driver {

/**
 * @brief abstract class of audio driver
 */
class AudioDriver {
 public:
  /**
   * @brief default constructor
   */
  AudioDriver() = default;

  /**
   * @brief default deConstructor
   */
  virtual ~AudioDriver() = default;

  /**
   * @brief setNumOfStream
   * @param [in] ao audio output
   * @return error code
   */
  virtual common::Error Init(ao_sptr ao) = 0;

  /**
   * @brief open audio device
   * @param [in] ao audio output
   * @return error code
   */
  virtual common::Error Open(ao_sptr ao) = 0;

  /**
   * @brief Playback one frame
   * @param [in] ao audio output
   * @return error code
   */
  virtual common::Error Play(ao_sptr ao) = 0;

  /**
   * @brief stop Playback
   * @param [in] ao audio output
   * @return error code
   */
  virtual common::Error Stop(ao_sptr ao) = 0;

  /**
   * @brief ReConfig
   * @param [in] ao audio output
   * @return error code
   */
  virtual common::Error ReConfig(ao_sptr ao) = 0;

  /**
   * @brief get device
   * @param [in] ao audio output
   * @param [out] devices list of device name
   * @return error code
   */
  virtual common::Error GetDevices(ao_sptr ao,
                                   misc::vector_sptr<std::string> &devices) = 0;

  /**
   * @brief get output desc
   * @param [in] ao audio output
   * @param [out] desc descriptor of output device
   * @return error code
   */
  virtual common::Error GetDesc(ao_sptr ao,
                                tool::resample::Desc &desc) = 0;
};

}

#endif //PLAYER_AUDIODRIVER_H
