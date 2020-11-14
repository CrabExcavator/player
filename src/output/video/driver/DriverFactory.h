//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_DRIVERFACTORY_H
#define PLAYER_DRIVERFACTORY_H

#include <string>

#include "VideoDriver.h"

namespace video::driver {

/**
 * @brief video driver factory
 */
class DriverFactory {
 public:
  /**
   * @brief create video driver
   * @param driver_name video driver name
   * @return video driver
   */
  static video_driver_uptr create(const std::string &driver_name);
};

}

#endif //PLAYER_DRIVERFACTORY_H
