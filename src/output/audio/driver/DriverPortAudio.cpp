//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include <map>

#include "DriverPortAudio.h"
#include "output/audio/AudioOutput.h"
#include "demux/frame/IFrame.h"
#include "output/audio/SampleFormat.h"

namespace output::audio::driver {

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
static std::map<SampleFormat, PaSampleFormat> sampleMap = {
    {SampleFormat::FLTP, paFloat32}
};

/// @todo move to other place
static std::map<SampleFormat, uint> unitSizeMap = {
    {SampleFormat::FLTP, 4}
};

DriverPortAudio::~DriverPortAudio() {
  this->buffer_.close();
  Pa_StopStream(this->stream_);
  Pa_Terminate();
}

common::Error DriverPortAudio::Init(ao_sptr ao) {
  if (Pa_Initialize() != paNoError) {
    return common::Error::audioDriverInitFail;
  }

  /// setNumOfStream outputParameters
  auto device = Pa_GetDefaultOutputDevice();
  auto deviceInfo = Pa_GetDeviceInfo(device);
  PaStreamParameters outputParameters;
  outputParameters.device = Pa_GetDefaultOutputDevice();
  num_of_channel = std::min(ao->num_of_channel_, deviceInfo->maxOutputChannels);
  outputParameters.channelCount = num_of_channel;
  outputParameters.sampleFormat = sampleMap.at(ao->sample_format_);
  outputParameters.suggestedLatency = deviceInfo->defaultLowOutputLatency;
  outputParameters.hostApiSpecificStreamInfo = nullptr;
  ao->num_of_channel_ = outputParameters.channelCount;
  unit_size = unitSizeMap.at(ao->sample_format_);

  if (Pa_OpenStream(
      &this->stream_,
      nullptr,
      &outputParameters,
      ao->sample_rate_,
      paFramesPerBufferUnspecified,
      paClipOff,
      &DriverPortAudio::paCallback,
      this
  ) != paNoError) {
    return common::Error::audioDriverInitFail;
  }

  if (Pa_SetStreamFinishedCallback(this->stream_,
                                   &DriverPortAudio::paStreamFinished) != paNoError) {
    return common::Error::audioDriverInitFail;
  }
  return common::Error::SUCCESS;
}

common::Error DriverPortAudio::Play(ao_sptr ao) {
  if (ao->frame_playing_ != nullptr) {
    switch (ao->sample_format_) {
      case SampleFormat::FLTP: {
        int cur = 0;
        for (int sample = 0; sample < ao->frame_playing_->GetNumOfSample(); sample++) {
          misc::vector_sptr<common::Slice> data = nullptr;
          if (common::Error::SUCCESS != ao->frame_playing_->GetData(data)) {
            // do nothing
          } else {
            for (auto aData : *data) {
              this->buffer_.put(aData.GetPtr(), cur, ao->size_of_sample_);
            }
          }
          cur += ao->size_of_sample_;
        }
        break;
      }
      default: {
        break;
      }
    }
    if (Pa_IsStreamStopped(this->stream_)) {
      Pa_StartStream(this->stream_);
    }
  }
  return common::Error::SUCCESS;
}

common::Error DriverPortAudio::Stop(ao_sptr ao) {
  Pa_StopStream(this->stream_);
  return common::Error::SUCCESS;
}

common::Error DriverPortAudio::ReConfig(ao_sptr ao) {
  return common::Error::SUCCESS;
}

common::Error DriverPortAudio::GetDevices(ao_sptr ao, misc::vector_sptr<std::string> &devices) {
  return common::Error::SUCCESS;
}

int DriverPortAudio::paCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
                                const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags,
                                void *userData) {
  return static_cast<DriverPortAudio *>(userData)->paCallbackMethod(inputBuffer, outputBuffer,
                                                                    framesPerBuffer,
                                                                    timeInfo,
                                                                    statusFlags);
}

int DriverPortAudio::paCallbackMethod(const void *inputBuffer, void *outputBuffer, unsigned long samplesPerBuffer,
                                      const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags) {
  this->buffer_.get(static_cast<uint8_t *>(outputBuffer), 0, num_of_channel * unit_size * samplesPerBuffer);
  return paContinue;
}

void DriverPortAudio::paStreamFinished(void *userData) {
  return static_cast<DriverPortAudio *>(userData)->paStreamFinishedMethod();
}

void DriverPortAudio::paStreamFinishedMethod() {

}

}
