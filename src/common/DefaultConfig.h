//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_DEFAULTCONFIG_H
#define PLAYER_DEFAULTCONFIG_H

#include <string>
#include <map>

#include "misc/Pocket.h"
#include "core/PlayList.h"

namespace core {

    // todo serializer clazz
    class DefaultConfig {
    public:

        // core config
        static misc::Pocket<core::play_method> default_play_method;

        // ao config
        static misc::Pocket<int> window_width;
        static misc::Pocket<int> window_height;
        static misc::Pocket<std::string> ao_driver;

        // vo config
        static misc::Pocket<std::string> vo_driver;
        static misc::Pocket<int> default_queue_size;

    };

}

#endif //PLAYER_DEFAULTCONFIG_H
