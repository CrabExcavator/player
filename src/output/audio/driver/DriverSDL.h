//
// Created by CrabExcavator on 2020/12/2.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_OUTPUT_AUDIO_DRIVER_DRIVERSDL_H_
#define PLAYER_SRC_OUTPUT_AUDIO_DRIVER_DRIVERSDL_H_

#include "AudioDriver.h"
#include "misc/typeptr.h"
#include "misc/Buffer.h"
#include "tool/sdl/SDLManager.h"

namespace output::audio::driver {

class DriverSDL : public AudioDriver {
 public:
  ~DriverSDL() override;
  common::Error Init(ao_sptr ao) override;
  common::Error Open(ao_sptr ao) override;
  common::Error Play(ao_sptr ao) override;
  common::Error Stop(ao_sptr ao) override;
  common::Error ReConfig(ao_sptr ao) override;
  common::Error GetDevices(ao_sptr ao,
                           misc::vector_sptr<std::string> &devices) override;
  common::Error GetDesc(ao_sptr ao,
                        tool::resample::Desc &desc) override;

 private:
  static SDL_AudioFormat format_translate(audio::SampleFormat sample_format);
  static void audio_callback(void *data, Uint8 *stream, int len);
  void playback(Uint8 *stream, int len);

 private:
  misc::Buffer<uint8_t, 65536> buffer_;
};

}

#endif //PLAYER_SRC_OUTPUT_AUDIO_DRIVER_DRIVERSDL_H_
