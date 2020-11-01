//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_DRIVERFACTORY_H
#define PLAYER_DRIVERFACTORY_H

#include "AudioDriver.h"

namespace audio::driver {

    class DriverFactory {
    public:
        static audio_driver_uptr create(const std::string& driver_name);
    };

}

#endif //PLAYER_DRIVERFACTORY_H
