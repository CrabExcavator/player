//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_DRIVERFACTORY_H
#define PLAYER_DRIVERFACTORY_H

#include "AudioDriver.h"

namespace audio::driver {

    /**
     * @brief the factory to create audio driver
     */
    class DriverFactory {
    public:
        /**
         * @brief create audio driver with specify name
         * @param [in] driver_name the name of audio driver, could be
         * defined in config
         * @return created driver
         */
        static audio_driver_uptr create(const std::string& driver_name);
    };

}

#endif //PLAYER_DRIVERFACTORY_H
