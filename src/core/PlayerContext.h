//
// Created by weihan on 2020/10/7.
//

#ifndef PLAYER_PLAYERCONTEXT_H
#define PLAYER_PLAYERCONTEXT_H

#include <memory>
#include <folly/MPMCQueue.h>

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
        sync_sptr sync_;
        play_list_sptr  play_list;
        input::input_ctx_sptr input_ctx;
        // a ticker run in cycle to check is the right pts?
        // todo support multiple vo & ao
        std::shared_ptr<folly::MPMCQueue<demux::frame_sptr>> vo_queue;
        std::shared_ptr<folly::MPMCQueue<demux::frame_sptr>> ao_queue;

    private:
        video::vo_sptr _vo;
        audio::ao_sptr _ao;
        demux::demux_ctx_sptr _demux_ctx;
    };

}


#endif //PLAYER_PLAYERCONTEXT_H
