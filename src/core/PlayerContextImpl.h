//
// Created by weihan on 2020/10/7.
//

#ifndef PLAYER_PLAYERCONTEXTIMPL_H
#define PLAYER_PLAYERCONTEXTIMPL_H

#include <memory>

#include "video/VideoOutput.h"
#include "input/InputContext.h"

namespace core {

    class PlayerContextImpl {
    public:
        PlayerContextImpl();
        void run();

    private:
        bool loop();

    private:
        input::input_ctx_sptr _input_ctx;
        video::vo_sptr _vo;
    };

}


#endif //PLAYER_PLAYERCONTEXTIMPL_H
