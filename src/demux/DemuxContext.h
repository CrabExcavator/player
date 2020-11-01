//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_DEMUXCONTEXT_H
#define PLAYER_DEMUXCONTEXT_H

#include <memory>
#include <folly/MPMCQueue.h>

#include "Demuxer.h"
#include "core/PlayList.h"
#include "misc/Thread.h"
#include "misc/typeptr.h"
#include "common/error.h"

namespace demux {

    /**
     * @brief runtime demux
     */
    class DemuxContext: public std::enable_shared_from_this<DemuxContext> {
    public:
        /**
         * @brief default
         */
        DemuxContext() = default;

        /**
         * @brief init
         * @param [in] player_ctx
         * @return error code
         */
        common::error init(const core::player_ctx_sptr& player_ctx);

        /**
         * @brief one tick
         * @return false if end
         */
        bool loop();

        /**
         * @brief stop demux thread
         * @return error code
         */
        common::error stopRunning();

    public:
        /**
         * @brief frame input queue for video output
         */
        std::shared_ptr<folly::MPMCQueue<demux::frame_sptr>> vo_queue;

        /**
         * @brief frame input queue for audio output
         */
        std::shared_ptr<folly::MPMCQueue<demux::frame_sptr>> ao_queue;

        /**
         * @brief sync should init in demuxer, because demuxer know the number of stream
         */
        core::sync_sptr sync_;

    private:
        /**
         * @brief demuxer for entry
         */
        demuxer_sptr _demuxer;

        /**
         * @brief play list
         */
        core::play_list_sptr _play_list;

        /**
         * @brief flag to mark is demux thread running
         */
        bool _running = false;

        /**
         * @brief demux thread
         */
        misc::Thread _thread;
    };

}

#endif //PLAYER_DEMUXCONTEXT_H
