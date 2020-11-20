//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include <memory>

#include "DriverFactory.h"
#include "DriverPortAudio.h"

namespace output::audio::driver {

audio_driver_uptr DriverFactory::create(const std::string &driver_name) {
  return std::make_unique<DriverPortAudio>();
}

}
