//
// Created by weihan on 2020/10/7.
//

#ifndef PLAYER_DRIVERFACTORY_H
#define PLAYER_DRIVERFACTORY_H

#include <string>

#include "Driver.h"

namespace video::driver {

    class DriverFactory {
    public:
        static driver_uptr create(std::string driver_name);
    };

}

#endif //PLAYER_DRIVERFACTORY_H
