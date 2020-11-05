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
         * @brief setNumOfStream
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
         * @brief sync should setNumOfStream in demuxer, because demuxer know the number of stream
         */
        core::sync_ctx_sptr sync_ctx;

    private:
        /**
         * @brief demuxer for entry
         */
        demuxer_sptr _demuxer;

        /**
         * @brief input context
         */
        input::input_ctx_sptr _input_context;

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
