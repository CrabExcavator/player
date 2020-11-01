//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_DRIVERPORTAUDIO_H
#define PLAYER_DRIVERPORTAUDIO_H

#include <portaudio.h>
#include <folly/MPMCQueue.h>

#include "AudioDriver.h"
#include "misc/Buffer.h"

namespace audio::driver {

    class DriverPortAudio: public AudioDriver {
    public:
        DriverPortAudio() = default;

        ~DriverPortAudio() override;

        // bind playback callback && bind terminate callback
        void init(ao_sptr ao) override;

        void play(ao_sptr ao) override;

        void stop(ao_sptr ao) override;

        void reConfig(ao_sptr ao) override;

        misc::vector_sptr<std::string> getDevices(ao_sptr ao) override;

    private:
        static void paStreamFinished(void* userData);

        static int paCallback(const void* inputBuffer, void *outputBuffer,
                              unsigned long framesPerBuffer,
                              const PaStreamCallbackTimeInfo* timeInfo,
                              PaStreamCallbackFlags statusFlags,
                              void* userData);

        void paStreamFinishedMethod();

        int paCallbackMethod(const void* inputBuffer, void* outputBuffer,
                             unsigned long samplesPerBuffer,
                             const PaStreamCallbackTimeInfo* timeInfo,
                             PaStreamCallbackFlags statusFlags);

    private:
        PaStream* _stream = nullptr;
        misc::Buffer<uint8_t, 65536> _buffer;
    };

}

#endif //PLAYER_DRIVERPORTAUDIO_H
