//
// Created by weihan on 2020/10/6.
//

#include <memory>
#include <gflags/gflags.h>

#include "core/PlayerContext.h"
#include "misc/Init.h"

namespace osal {

    int player_main(int argc, char *argv[]) {
        misc::Init _(argc, argv);
        auto player_context = std::make_shared<core::PlayerContext>();
        player_context->run();
        return 0;
    }

}