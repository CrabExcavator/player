//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SYNC_H
#define PLAYER_SYNC_H

#include <mutex>
#include <condition_variable>

namespace core {

    /**
     * @brief sync different output
     */
    class Sync {
    public:
        /**
         * @brief default
         */
        Sync();

        /**
         * @brief init
         * @param [in] size number of output
         */
        void init(int size);

        /**
         * @brief call in output thread for sync
         */
        void wait();

        /**
         * @brief when close called, all thread stopped by wait should
         * start
         */
        void close();

    private:
        /**
         * @brief flag to mark close
         */
        bool _close;

        /**
         * @brief size of Sync
         */
        int _size;

        /**
         * @brief used in wait
         */
        int _cnt;

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

#endif //PLAYER_SYNC_H
