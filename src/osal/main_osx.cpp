//
// Created by weihan on 2020/10/6.
//

#include <memory>
#include <gflags/gflags.h>

#include "core/PlayerContext.h"

namespace osal {

    int player_main(int argc, char *argv[]) {
        gflags::ParseCommandLineFlags(&argc, &argv, true); //todo config

        auto player_context = std::make_shared<core::PlayerContext>();
        player_context->run();
        return 0;
    }

}