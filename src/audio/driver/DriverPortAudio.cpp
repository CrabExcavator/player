//
// Created by weihan on 2020/10/21.
//

#include "DriverPortAudio.h"
#include "audio/AudioOutput.h"
#include "demux/Frame.h"

static const int length = 1000000000;

namespace audio::driver {

    DriverPortAudio::~DriverPortAudio() {
        Pa_StopStream(this->_stream);
        Pa_Terminate();
        this->_buffer.close();
    }

    void DriverPortAudio::init(ao_sptr ao) {
        Pa_Initialize();
        PaStreamParameters outputParameters;
        outputParameters.device = Pa_GetDefaultOutputDevice();
        // todo error handle
        outputParameters.channelCount = 2;
        outputParameters.sampleFormat = paFloat32;
        outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
        outputParameters.hostApiSpecificStreamInfo = nullptr;

        auto err = Pa_OpenStream(
                &this->_stream,
                nullptr,
                &outputParameters,
                44100,
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
            auto cur1 = 0; auto cur2 = 0;
            for (int i = 0 ; i < ao->frame_playing->raw()->nb_samples ; i++) {
                this->_buffer.put(ao->frame_playing->raw()->data[0], cur1, 4);
                cur1 += 4;
                this->_buffer.put(ao->frame_playing->raw()->data[1], cur2, 4);
                cur2 += 4;
            }
        }
        if (Pa_IsStreamStopped(this->_stream)) {
            Pa_StartStream(this->_stream);
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
        this->_buffer.get(static_cast<uint8_t*>(outputBuffer), 0, 2 * 4 * samplesPerBuffer);
        return paContinue;
    }

    void DriverPortAudio::paStreamFinished(void *userData) {
        return static_cast<DriverPortAudio*>(userData)->paStreamFinishedMethod();
    }

    void DriverPortAudio::paStreamFinishedMethod() {

    }

}
