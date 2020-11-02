//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include <memory>

#include "core/PlayerContext.h"
#include "misc/Init.h"

namespace osal {

    /**
     * @brief main func in osx platform
     * @param [in] argc pass from true main func
     * @param [in] argv pass from true main func
     * @return error code i guess
     */
    int player_main(int argc, char *argv[]) {
        misc::Init _(argc, argv);
        auto player_context = std::make_shared<core::PlayerContext>();
        player_context->init();
        player_context->run();
        return 0;
    }

}