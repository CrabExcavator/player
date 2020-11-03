//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include <map>

#include "DriverPortAudio.h"
#include "audio/AudioOutput.h"
#include "demux/Frame.h"
#include "audio/sample_format.h"

namespace audio::driver {

    /**
     * @brief number of channel
     */
     static int num_of_channel = 0;

     /**
      * @brief number of byte of sample
      */
     static int unit_size = 0;

    /**
     * @brief map from player sample format to PortAudio sample format
     */
    static std::map<sample_format, PaSampleFormat> sampleMap = {
            {sample_format::fltp, paFloat32}
    };

    /// @todo move to other place
    static std::map<sample_format, uint> unitSizeMap = {
            {sample_format::fltp, 4}
    };

    DriverPortAudio::~DriverPortAudio() {
        this->_buffer.close();
        Pa_StopStream(this->_stream);
        Pa_Terminate();
    }

    common::error DriverPortAudio::init(ao_sptr ao) {
        if (Pa_Initialize() != paNoError) {
            return common::error::audioDriverInitFail;
        }

        /// init outputParameters
        auto device = Pa_GetDefaultOutputDevice();
        auto deviceInfo = Pa_GetDeviceInfo(device);
        PaStreamParameters outputParameters;
        outputParameters.device = Pa_GetDefaultOutputDevice();
        num_of_channel = std::min(ao->num_of_channel, deviceInfo->maxOutputChannels);
        outputParameters.channelCount = num_of_channel;
        outputParameters.sampleFormat = sampleMap.at(ao->sampleFormat);
        outputParameters.suggestedLatency = deviceInfo->defaultLowOutputLatency;
        outputParameters.hostApiSpecificStreamInfo = nullptr;
        ao->num_of_channel = outputParameters.channelCount;
        unit_size = unitSizeMap.at(ao->sampleFormat);

        if (Pa_OpenStream(
                &this->_stream,
                nullptr,
                &outputParameters,
                ao->sample_rate,
                paFramesPerBufferUnspecified,
                paClipOff,
                &DriverPortAudio::paCallback,
                this
                ) != paNoError) {
            return common::error::audioDriverInitFail;
        }

        if (Pa_SetStreamFinishedCallback(this->_stream,
                                         &DriverPortAudio::paStreamFinished) != paNoError) {
            return common::error::audioDriverInitFail;
        }
        return common::error::success;
    }

    common::error DriverPortAudio::play(ao_sptr ao) {
        if (ao->frame_playing != nullptr) {
            switch (ao->sampleFormat) {
                case sample_format::fltp: {
                    int cur = 0;
                    for (int sample = 0; sample < ao->frame_playing->num_of_sample; sample++) {
                        for (int channel = 0; channel < ao->num_of_channel; channel++) {
                            this->_buffer.put(ao->frame_playing->raw()->data[channel],
                                              cur, ao->size_of_sample);
                        }
                        cur += ao->size_of_sample;
                    }
                    break;
                }
                default: {
                    break;
                }
            }
            if (Pa_IsStreamStopped(this->_stream)) {
                Pa_StartStream(this->_stream);
            }
        }
        return common::error::success;
    }

    common::error DriverPortAudio::stop(ao_sptr ao) {
        Pa_StopStream(this->_stream);
        return common::error::success;
    }

    common::error DriverPortAudio::reConfig(ao_sptr ao) {
        return common::error::success;
    }

    common::error DriverPortAudio::getDevices(ao_sptr ao, misc::vector_sptr<std::string>& devices) {
        return common::error::success;
    }

    int DriverPortAudio::paCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
                                    const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags,
                                    void *userData) {
        return static_cast<DriverPortAudio*>(userData)->paCallbackMethod(inputBuffer, outputBuffer,
                                                                         framesPerBuffer,
                                                                         timeInfo,
                                                                         statusFlags);
    }

    int DriverPortAudio::paCallbackMethod(const void *inputBuffer, void *outputBuffer, unsigned long samplesPerBuffer,
                                          const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags) {
        this->_buffer.get(static_cast<uint8_t*>(outputBuffer), 0, num_of_channel * unit_size * samplesPerBuffer);
        return paContinue;
    }

    void DriverPortAudio::paStreamFinished(void *userData) {
        return static_cast<DriverPortAudio*>(userData)->paStreamFinishedMethod();
    }

    void DriverPortAudio::paStreamFinishedMethod() {

    }

}
