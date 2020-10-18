//
// Created by weihan on 2020/10/7.
//

#ifndef PLAYER_PLAYERCONTEXT_H
#define PLAYER_PLAYERCONTEXT_H

#include <memory>
#include <folly/MPMCQueue.h>

#include "demux/DemuxContext.h"
#include "video/VideoOutput.h"
#include "input/InputContext.h"
#include "PlayList.h"
#include "misc/typeptr.h"

namespace core {

    class PlayerContext: public std::enable_shared_from_this<PlayerContext> {
    public:
        PlayerContext() = default;
        void init();
        void run();

    private:
        bool loop();

    public:
        play_list_sptr  play_list;
        input::input_ctx_sptr input_ctx;
        // todo put frame & audio & sub in the same queue ?
        // a ticker run in cycle to check is the right pts?
        std::shared_ptr<folly::MPMCQueue<demux::frame_sptr>> queue;

    private:
        video::vo_sptr _vo;
        demux::demux_ctx_sptr _demux_ctx;
    };

}


#endif //PLAYER_PLAYERCONTEXT_H
