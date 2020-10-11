//
// Created by weihan on 2020/10/8.
//

#ifndef PLAYER_DEFAULTCONFIG_H
#define PLAYER_DEFAULTCONFIG_H

#include <string>

#include "misc/Pocket.h"

namespace common {

    class DefaultConfig {
    public:
        static misc::Pocket<int> window_width;
        static misc::Pocket<int> window_height;
        static misc::Pocket<std::string> vo_driver;
    };

}

#endif //PLAYER_DEFAULTCONFIG_H
