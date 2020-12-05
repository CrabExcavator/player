//
// Created by CrabExcavator on 2020/12/2.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "DriverSDL.h"
#include "output/audio/AudioOutput.h"
#include "demux/frame/IFrame.h"

namespace output::audio::driver {

DriverSDL::~DriverSDL() {
  buffer_.close();
}

common::Error DriverSDL::Init(ao_sptr ao) {
  auto ret = common::Error::SUCCESS;
  return ret;
}

common::Error DriverSDL::Open(ao_sptr ao) {
  auto ret = common::Error::SUCCESS;
  auto &sdl_manager = tool::sdl::SDLManager::GetInstance();
  SDL_AudioSpec spec;
  spec.freq = ao->sample_rate_;
  spec.format = format_translate(ao->sample_format_);
  spec.channels = ao->num_of_channel_;
  spec.silence = 0;
  spec.samples = 1024;
  spec.callback = audio_callback;
  spec.userdata = this;

  if (common::Error::SUCCESS != (ret = sdl_manager->OpenAudio(&spec, nullptr))) {
    LOG(WARNING) << "open audio fail";
  } else if (common::Error::SUCCESS != (ret = sdl_manager->PauseAudio(0))) {
    LOG(WARNING) << "start callback fail";
  }
  return ret;
}

common::Error DriverSDL::Play(ao_sptr ao) {
  auto ret = common::Error::SUCCESS;

  if (nullptr != ao->frame_playing_) {
    if (SampleFormat::FLTP == ao->sample_format_) {
      int cur = 0;
      for (int sample = 0; sample < ao->frame_playing_->GetNumOfSample(); sample++) {
        misc::vector_sptr<misc::Slice> data = nullptr;
        if (common::Error::SUCCESS != ao->frame_playing_->GetData(data)) {
          // do nothing
        } else {
          for (auto aData : *data) {
            buffer_.put(aData.GetPtr(), cur, ao->size_of_sample_);
          }
        }
        cur += ao->size_of_sample_;
      }
    }
  }
  return ret;
}

common::Error DriverSDL::Stop(ao_sptr ao) {
  auto ret = common::Error::SUCCESS;
  auto &sdl_manager = tool::sdl::SDLManager::GetInstance();

  if (common::Error::SUCCESS != (ret = sdl_manager->PauseAudio(1))) {
    LOG(WARNING) << "pause fail";
  }
  return ret;
}

common::Error DriverSDL::ReConfig(ao_sptr ao) {
  auto ret = common::Error::UN_IMPL;
  return ret;
}

common::Error DriverSDL::GetDevices(ao_sptr ao, misc::vector_sptr<std::string> &devices) {
  auto ret = common::Error::UN_IMPL;
  return ret;
}

common::Error DriverSDL::GetDesc(ao_sptr ao, tool::resample::Desc &desc) {
  auto ret = common::Error::UN_IMPL;
  return ret;
}

SDL_AudioFormat DriverSDL::format_translate(audio::SampleFormat sample_format) {
  SDL_AudioFormat ret = 0;

  if (audio::SampleFormat::FLTP == sample_format) {
    ret = AUDIO_F32;
  }
  return ret;
}

void DriverSDL::audio_callback(void *data, Uint8 *stream, int len) {
  static_cast<DriverSDL*>(data)->playback(stream, len);
}

void DriverSDL::playback(Uint8 *stream, int len) {
  buffer_.get(stream, 0, len);
}

}
