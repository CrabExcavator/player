//
// Created by weihan on 2020/10/8.
//

#include "DefaultConfig.h"

namespace common {

#define DEFINE_POCKET(type, key, value) misc::Pocket<type> DefaultConfig::key(#key, value)

    DEFINE_POCKET(int, window_width, 1920);
    DEFINE_POCKET(int, window_height, 1080);
    DEFINE_POCKET(std::string, ao_driver, "SDL2");
    DEFINE_POCKET(std::string, vo_driver, "PortAudio");
    DEFINE_POCKET(core::play_method, default_play_method, core::play_method::list_loop);
    DEFINE_POCKET(int, default_queue_size, 10);

}
