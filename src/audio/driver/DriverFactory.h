//
// Created by weihan on 2020/10/20.
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
