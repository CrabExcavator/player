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
     * @brief map from player sample format to PortAudio sample format
     */
    static std::map<sample_format, PaSampleFormat> sampleMap = {
            {sample_format::FLTP, paFloat32}
    };

    DriverPortAudio::~DriverPortAudio() {
        this->_buffer.close();
        Pa_StopStream(this->_stream);
        Pa_Terminate();
    }

    void DriverPortAudio::init(ao_sptr ao) {
        Pa_Initialize();
        PaStreamParameters outputParameters;
        outputParameters.device = Pa_GetDefaultOutputDevice();
        auto device = Pa_GetDeviceInfo(outputParameters.device);
        outputParameters.channelCount = std::min(ao->num_of_channel, device->maxOutputChannels); ao->num_of_channel = outputParameters.channelCount;
        outputParameters.sampleFormat = sampleMap.at(ao->sampleFormat);
        outputParameters.suggestedLatency = device->defaultLowOutputLatency;
        outputParameters.hostApiSpecificStreamInfo = nullptr;

        auto err = Pa_OpenStream(
                &this->_stream,
                nullptr,
                &outputParameters,
                ao->sample_rate,
                paFramesPerBufferUnspecified,
                paClipOff,
                &DriverPortAudio::paCallback,
                this
                );
        if (err != paNoError) {
            // todo error handle
            LOG(INFO) << err;
        }
        err = Pa_SetStreamFinishedCallback(this->_stream, &DriverPortAudio::paStreamFinished);
        if (err != paNoError) {
            // todo error handle
            LOG(INFO) << err;
        }
    }

    void DriverPortAudio::play(ao_sptr ao) {
        if (ao->frame_playing != nullptr) {
            switch (ao->sampleFormat) {
                case sample_format::FLTP: {
                    int cur = 0;
                    for (int sample = 0; sample < ao->frame_playing->num_of_sample; sample++) {
                        for (int channel = 0; channel < ao->num_of_channel; channel++) {
                            this->_buffer.put(ao->frame_playing->raw()->data[channel], cur,
                                              ao->size_of_sample);
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
    }

    void DriverPortAudio::stop(ao_sptr ao) {
        Pa_StopStream(this->_stream);
    }

    void DriverPortAudio::reConfig(ao_sptr ao) {

    }

    misc::vector_sptr<std::string> DriverPortAudio::getDevices(ao_sptr ao) {
        return misc::vector_sptr<std::string>();
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
        // todo add static driver config
        this->_buffer.get(static_cast<uint8_t*>(outputBuffer), 0, 2 * 4 * samplesPerBuffer);
        return paContinue;
    }

    void DriverPortAudio::paStreamFinished(void *userData) {
        return static_cast<DriverPortAudio*>(userData)->paStreamFinishedMethod();
    }

    void DriverPortAudio::paStreamFinishedMethod() {

    }

}
