//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "DefaultConfig.h"

namespace common {

#define DEFINE_POCKET(type, key, value) misc::Pocket<type> DefaultConfig::key(#key, value)

DEFINE_POCKET(int, window_width, 1920);
DEFINE_POCKET(int, window_height, 1080);
DEFINE_POCKET(std::string, ao_driver, "SDL2");
DEFINE_POCKET(std::string, vo_driver, "PortAudio");
DEFINE_POCKET(player::play_method, default_play_method, player::play_method::entry_end);
DEFINE_POCKET(int, default_queue_size, 64);
DEFINE_POCKET(int, default_ticker_hz, 60);

}
