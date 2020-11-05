//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_AUDIOOUTPUT_H
#define PLAYER_AUDIOOUTPUT_H

#include <memory>
#include <chrono>
#include <folly/MPMCQueue.h>

#include "sample_format.h"
#include "audio/driver/AudioDriver.h"
#include "misc/typeptr.h"
#include "misc/Thread.h"
#include "common/error.h"
#include "core/Output.h"

namespace audio {

    /**
     * @brief Audio output
     */
    class AudioOutput: public core::Output, public std::enable_shared_from_this<AudioOutput> {
    public:
        /**
         * @brief constructor do nothing
         */
        AudioOutput();

        /**
         * @brief delete
         * @param rhs
         */
        AudioOutput(const AudioOutput& rhs) = delete;

        /**
         * @brief delete
         * @param rhs
         */
        AudioOutput(AudioOutput&& rhs) = delete;

        /**
         * @brief delete
         * @param rhs
         * @return
         */
        AudioOutput& operator = (const AudioOutput& rhs) = delete;

        /**
         * @brief delete
         * @param rhs
         * @return
         */
        AudioOutput& operator = (AudioOutput&& rhs) = delete;

        /**
         * @brief default deConstructor
         */
        ~AudioOutput() = default;

        /**
         * @brief setNumOfStream audio output
         * @param [in] player_ctx player context
         * @return error code
         */
        common::error init(const core::player_ctx_sptr& player_ctx);

        /**
         * @brief stop audio playback thread
         * @return error code
         */
        common::error stopRunning();

    public:

        /**
         * @brief frame is playing, always used by audio driver
         */
        demux::frame_sptr frame_playing = nullptr;

        /**
         * @brief flag to mark force reConfig
         */
        bool need_reConfig = false;

        /**
         * @brief sample format
         */
        sample_format sampleFormat = sample_format::unknown;

        /**
         * @brief number of channel
         */
        int num_of_channel = 0;

        /**
         * @brief the size of each sample in each channel
         */
        int size_of_sample = 0;

        /**
         * @brief sample rate
         */
        int sample_rate = 0;

    private:
        bool loop();

    private:
        /**
         * @brief flag to mark is audio playback thread running
         */
        bool _running = false;

        /**
         * @brief audio playback thread
         */
        misc::Thread _thread;

        /**
         * @brief impl of driver
         */
        audio::driver::audio_driver_uptr _driver = nullptr;
    };

}

#endif //PLAYER_AUDIOOUTPUT_H
