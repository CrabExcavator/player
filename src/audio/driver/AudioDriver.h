//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_AUDIODRIVER_H
#define PLAYER_AUDIODRIVER_H

#include <memory>

#include "misc/typeptr.h"

namespace audio::driver {

    /**
     * @brief abstract class of audio driver
     */
    class AudioDriver {
    public:
        /**
         * @brief default constructor
         */
        AudioDriver() = default;

        /**
         * @brief default deConstructor
         */
        virtual ~AudioDriver() = default;

        /**
         * @brief init
         * @param [in] ao audio output
         */
        virtual void init(ao_sptr ao) = 0;

        /**
         * @brief playback one frame
         * @param [in] ao audio output
         */
        virtual void play(ao_sptr ao) = 0;

        /**
         * @brief stop playback
         * @param [in] ao audio output
         */
        virtual void stop(ao_sptr ao) = 0;

        /**
         * @brief reConfig
         * @param [in] ao audio output
         */
        virtual void reConfig(ao_sptr ao) = 0;

        /**
         * @brief get device
         * @param [in] ao audio output
         * @return list of device name
         */
        virtual misc::vector_sptr<std::string> getDevices(ao_sptr ao) = 0;
    };

}

#endif //PLAYER_AUDIODRIVER_H
