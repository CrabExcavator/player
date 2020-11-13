//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_AUDIODRIVER_H
#define PLAYER_AUDIODRIVER_H

#include <memory>

#include "misc/typeptr.h"
#include "common/Error.h"

namespace audio::driver {

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
  virtual common::Error init(ao_sptr ao) = 0;

  /**
   * @brief playback one frame
   * @param [in] ao audio output
   * @return error code
   */
  virtual common::Error play(ao_sptr ao) = 0;

  /**
   * @brief stop playback
   * @param [in] ao audio output
   * @return error code
   */
  virtual common::Error stop(ao_sptr ao) = 0;

  /**
   * @brief reConfig
   * @param [in] ao audio output
   * @return error code
   */
  virtual common::Error reConfig(ao_sptr ao) = 0;

  /**
   * @brief get device
   * @param [in] ao audio output
   * @param [out] devices list of device name
   * @return error code
   */
  virtual common::Error getDevices(ao_sptr ao,
                                   misc::vector_sptr<std::string> &devices) = 0;
};

}

#endif //PLAYER_AUDIODRIVER_H
