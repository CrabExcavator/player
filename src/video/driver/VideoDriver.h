//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_VIDEODRIVER_H
#define PLAYER_VIDEODRIVER_H

#include <memory>

#include "misc/typeptr.h"

namespace video::driver {

    /**
     * @brief video driver
     */
    class VideoDriver {
    public:
        /**
         * @brief default
         */
        VideoDriver() = default;

        /**
         * @brief default
         */
        virtual ~VideoDriver() = default;

        /**
         * @brief init
         * @param [in] vo
         */
        virtual void init(vo_sptr vo) = 0;

        /**
         * @brief draw image
         * @param [in]vo
         */
        virtual void drawImage(vo_sptr vo) = 0;

        /**
         * @brief wait events
         * @param [in] vo
         */
        virtual void waitEvents(vo_sptr vo) = 0;

        /**
         * @brief reConfig
         * @param [in] vo
         */
        virtual void reConfig(vo_sptr vo) = 0;
    };

}

#endif //PLAYER_VIDEODRIVER_H