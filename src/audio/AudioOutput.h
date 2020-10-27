//
// Created by weihan on 2020/10/20.
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

namespace audio {

    class AudioOutput: public std::enable_shared_from_this<AudioOutput> {
    public:
        AudioOutput();
        AudioOutput(const AudioOutput& rhs) = delete;
        AudioOutput(AudioOutput&& rhs) = delete;
        AudioOutput& operator = (const AudioOutput& rhs) = delete;
        AudioOutput& operator = (AudioOutput&& rhs) = delete;
        ~AudioOutput() = default;
        void init(const core::player_ctx_sptr& player_ctx);
        void stopRunning();

    public:
        std::shared_ptr<folly::MPMCQueue<demux::frame_sptr>> queue;
        demux::frame_sptr frame_playing = nullptr;
        bool need_reConfig = false;
        sample_format sampleFormat = sample_format::unknown;
        int num_of_channel = 0;
        int size_of_sample = 0;
        int sample_rate = 0;

    private:
        bool loop();

    private:
        bool _running = false;
        audio::driver::audio_driver_uptr _driver = nullptr;
        std::chrono::steady_clock::time_point _last_tick{};
        std::chrono::steady_clock::duration _time_base{};
        int64_t _last_pts = 0;
        demux::frame_sptr _frame = nullptr;
        misc::Thread _thread;
    };

}

#endif //PLAYER_AUDIOOUTPUT_H
