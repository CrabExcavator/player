//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_DEFAULTCONFIG_H
#define PLAYER_DEFAULTCONFIG_H

#include <string>
#include <map>

#include "misc/Pocket.h"
#include "player/PlayList.h"

namespace common {

/**
 * @brief default config
 * if user defined class should support config, operator << & >>
 * should be impl
 */
class DefaultConfig {
 public:

  /**
   * @brief play_method of play_list
   */
  static misc::Pocket<player::play_method> default_play_method;

  /**
   * @brief window_width of audio output
   */
  static misc::Pocket<int> window_width;

  /**
   * @brief window_height of audio output
   */
  static misc::Pocket<int> window_height;

  /**
   * @brief driver name of audio output
   */
  static misc::Pocket<std::string> ao_driver;

  /**
   * @brief driver name of video driver
   */
  static misc::Pocket<std::string> vo_driver;

  /**
   * @brief default queue size of concurrent queue
   */
  static misc::Pocket<int> default_queue_size;

  static misc::Pocket<int> default_ticker_hz;

};

}

#endif //PLAYER_DEFAULTCONFIG_H
