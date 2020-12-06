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

DriverPortAudio::~DriverPortAudio() {
  buffer_.close();
  Pa_StopStream(stream_);
  Pa_Terminate();
}

common::Error DriverPortAudio::Init(ao_sptr ao) {
  auto ret = common::Error::SUCCESS;

  if (paNoError != Pa_Initialize()) {
    ret = common::Error::audioDriverInitFail;
  }
  return ret;
}

common::Error DriverPortAudio::Open(ao_sptr ao) {
  auto ret = common::Error::SUCCESS;

  auto device = Pa_GetDefaultOutputDevice();
  auto deviceInfo = Pa_GetDeviceInfo(device);
  PaStreamParameters outputParameters;
  outputParameters.device = device;
  outputParameters.channelCount = ao->num_of_channel_;
  outputParameters.sampleFormat = sampleMap.at(ao->sample_format_);
  outputParameters.suggestedLatency = deviceInfo->defaultLowOutputLatency;
  outputParameters.hostApiSpecificStreamInfo = nullptr;
  unit_size = ao->size_of_sample_;
  if (paNoError != Pa_OpenStream(&stream_,
                    nullptr,
                    &outputParameters,
                    ao->sample_rate_,
                    paFramesPerBufferUnspecified,
                    paClipOff,
                    &DriverPortAudio::paCallback,
                    this) != paNoError) {
    LOG(WARNING) << "pa open stream fail";
    ret = common::Error::audioDriverInitFail;
  } else if (paNoError != Pa_SetStreamFinishedCallback(stream_,
                                                       &DriverPortAudio::paStreamFinished)) {
    LOG(WARNING) << "pa set stream finished callback";
    ret = common::Error::audioDriverInitFail;
  }
  return ret;
}

common::Error DriverPortAudio::Play(ao_sptr ao) {
  if (ao->frame_playing_ != nullptr) {
    switch (ao->sample_format_) {
      case SampleFormat::FLTP: {
        int cur = 0;
        for (int sample = 0; sample < ao->frame_playing_->GetNumOfSample(); sample++) {
          misc::vector_sptr<misc::Slice> data = nullptr;
          if (common::Error::SUCCESS != ao->frame_playing_->GetData(data)) {
            // do nothing
          } else {
            for (auto aData : *data) {
              buffer_.Put(aData.GetPtr(), cur, ao->size_of_sample_);
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
  }
  return common::Error::SUCCESS;
}

common::Error DriverPortAudio::Stop(ao_sptr ao) {
  Pa_StopStream(stream_);
  return common::Error::SUCCESS;
}

common::Error DriverPortAudio::ReConfig(ao_sptr ao) {
  return common::Error::SUCCESS;
}

common::Error DriverPortAudio::GetDevices(ao_sptr ao, misc::vector_sptr<std::string> &devices) {
  return common::Error::SUCCESS;
}

common::Error DriverPortAudio::GetDesc(ao_sptr ao, tool::resample::Desc &desc) {
  auto ret = common::Error::SUCCESS;

  auto device = Pa_GetDefaultOutputDevice();
  auto deviceInfo = Pa_GetDeviceInfo(device);
  num_of_channel = std::min(ao->num_of_channel_, deviceInfo->maxOutputChannels);
  desc.sample_format = ao->sample_format_;
  desc.sample_rate = ao->sample_rate_;
  desc.number_of_channel = num_of_channel;
  desc.layout = ChannelLayout::STEREO;
  return ret;
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
  buffer_.Get(static_cast<uint8_t *>(outputBuffer), 0, num_of_channel * unit_size * samplesPerBuffer);
  return paContinue;
}

void DriverPortAudio::paStreamFinished(void *userData) {
  return static_cast<DriverPortAudio *>(userData)->paStreamFinishedMethod();
}

void DriverPortAudio::paStreamFinishedMethod() {

}

}
