//
// Created by weihan on 2020/10/8.
//

#ifndef PLAYER_DEFAULTCONFIG_H
#define PLAYER_DEFAULTCONFIG_H

#include <string>

#include "misc/Pocket.h"
#include "core/PlayList.h"

namespace common {

    class DefaultConfig {
    public:
        static misc::Pocket<int> window_width;
        static misc::Pocket<int> window_height;
        static misc::Pocket<std::string> vo_driver;
        static misc::Pocket<core::play_method> default_play_method;
    };

}

#endif //PLAYER_DEFAULTCONFIG_H
