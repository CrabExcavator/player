//
// Created by CrabExcavator on 2020/12/2.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include <fstream>
#include <cmath>

#include "DriverSDL.h"
#include "output/audio/AudioOutput.h"
#include "demux/frame/IFrame.h"

namespace output::audio::driver {

DriverSDL::DriverSDL():
    buffer_(),
    device_id_(0) {}

DriverSDL::~DriverSDL() {
  auto &sdl_manager = tool::sdl::SDLManager::GetInstance();
  buffer_.Close();
  sdl_manager->PauseAudioDevice(device_id_, 1);
  SDL_CloseAudioDevice(device_id_);
  device_id_ = 0;
}

common::Error DriverSDL::Init(ao_sptr ao) {
  auto ret = common::Error::SUCCESS;
  if (!SDL_getenv("SDL_AUDIO_ALSA_SET_BUFFER_SIZE")) {
    SDL_setenv("SDL_AUDIO_ALSA_SET_BUFFER_SIZE","1", 1);
  }
  return ret;
}

common::Error DriverSDL::Open(ao_sptr ao) {
  auto ret = common::Error::SUCCESS;
  auto &sdl_manager = tool::sdl::SDLManager::GetInstance();
  SDL_AudioSpec have, want;
  SDL_memset(&have, 0, sizeof(have));
  SDL_memset(&want, 0, sizeof(want));
  want.freq = ao->sample_rate_;
  want.format = FormatTranslate(ao->sample_format_);
  want.channels = ao->num_of_channel_;
  want.silence = 0;
  want.samples = std::max(512, 2 << (int)(std::log2(want.freq/30)));
  want.callback = AudioCallback;
  want.userdata = this;
  auto *tmp_have = &have;
  if (common::Error::SUCCESS != (ret = sdl_manager->OpenAudioDevice(nullptr,
                                                                    0,
                                                                    &want,
                                                                    tmp_have,
                                                                    SDL_AUDIO_ALLOW_FREQUENCY_CHANGE | SDL_AUDIO_ALLOW_CHANNELS_CHANGE,
                                                                    device_id_))) {
    LOG(WARNING) << "open audio fail";
  } else if (common::Error::SUCCESS != (ret = sdl_manager->PauseAudioDevice(device_id_, 0))) {
    LOG(WARNING) << "start callback fail";
  }
  return ret;
}

common::Error DriverSDL::Play(ao_sptr ao) {
  auto ret = common::Error::SUCCESS;
  misc::vector_sptr<misc::Slice> data = nullptr;
  int cur = 0;

  if (nullptr != ao->frame_playing_) {
    if (common::Error::SUCCESS != (ret = ao->frame_playing_->GetData(data))) {
      // do nothing
    } else {
      if (IsPlaneSampleFormat(ao->sample_format_)) {
        for (int sample = 0; sample < ao->frame_playing_->GetNumOfSample(); sample++) {
          for (auto aData : *data) {
            buffer_.Put(aData.GetPtr(), cur, ao->size_of_sample_);
          }
          cur += ao->size_of_sample_;
        }
      } else {
        if (1 != data->size()) {
          ret = common::Error::UNKNOWN_ERROR;
        } else {
          buffer_.Put(data->at(0).GetPtr(), 0,
                      ao->num_of_channel_
                          * ao->size_of_sample_
                          * ao->frame_playing_->GetNumOfSample());
        }
      }
    }
  }
  return ret;
}

common::Error DriverSDL::Stop(ao_sptr ao) {
  auto ret = common::Error::SUCCESS;
  auto &sdl_manager = tool::sdl::SDLManager::GetInstance();

  if (common::Error::SUCCESS != (ret = sdl_manager->PauseAudioDevice(device_id_, 1))) {
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

  desc.sample_format = SampleFormat::S16;
  desc.sample_rate = ao->sample_rate_;
  desc.number_of_channel = ao->num_of_channel_;
  desc.layout = ao->channel_layout_;
  return ret;
}

SDL_AudioFormat DriverSDL::FormatTranslate(audio::SampleFormat sample_format) {
  SDL_AudioFormat ret = 0;

  if (audio::SampleFormat::FLTP == sample_format) {
    ret = AUDIO_F32;
  } else if (audio::SampleFormat::S16P == sample_format) {
    ret = AUDIO_S16SYS;
  } else if (audio::SampleFormat::S16 == sample_format) {
    ret = AUDIO_S16SYS;
  }
  return ret;
}

void DriverSDL::AudioCallback(void *data, Uint8 *stream, int len) {
  static_cast<DriverSDL *>(data)->Playback(stream, len);
}

void DriverSDL::Playback(Uint8 *stream, int len) {
  buffer_.Get(stream, 0, len);
}

}
