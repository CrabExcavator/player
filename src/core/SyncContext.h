//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SYNCCONTEXT_H
#define PLAYER_SYNCCONTEXT_H

#include <mutex>
#include <condition_variable>
#include <vector>

#include "common/error.h"
#include "misc/typeptr.h"

namespace core {

    /**
     * @brief keep different thread with proper tick rate
     */
    class SyncContext {
    public:
        /**
         * @brief default
         */
        SyncContext();

        /**
         * @brief setNumOfStream
         * @param player_ctx
         * @return
         */
        common::error init(core::player_ctx_sptr player_ctx);

        /**
         * @brief setNumOfStream
         * @param [in] size number of output
         */
        void setNumOfStream(int size);

        /**
         * @brief add stream to sync
         * @param stream
         * @return error code
         */
        common::error addStream(const demux::stream_sptr& stream);

        /**
         * @brief call in output thread for sync
         */
        void wait();

        /**
         * @brief when close called, all thread stopped by wait should
        w * start
         */
        void close();

    public:
        /**
         * @brief version used to check if all output is in correct state
         */
        uint64_t version{};

    private:
        /**
         * @brief flag to mark close
         */
        bool _close;

        /**
         * @brief size of SyncContext
         */
        int _size;

        /**
         * @brief used in wait
         */
        int _cnt;

        /**
         * @brief streams used  in different output
         */
        misc::vector_sptr<demux::stream_sptr> _streams;

        /**
         * @brief mutex
         */
        std::mutex _mutex;

        /**
         * @brief cond
         */
        std::condition_variable _cond{};
    };

}

#endif //PLAYER_SYNCCONTEXT_H
