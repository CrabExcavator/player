//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include <memory>

#include "DriverFactory.h"
#include "DriverSDL.h"

namespace output::video::driver {

video_driver_uptr DriverFactory::create(const std::string &driver_name) { //todo driver register
  return std::make_unique<DriverSDL>();
}

}
