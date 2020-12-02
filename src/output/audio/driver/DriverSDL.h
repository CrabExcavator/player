//
// Created by CrabExcavator on 2020/12/2.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_OUTPUT_AUDIO_DRIVER_DRIVERSDL_H_
#define PLAYER_SRC_OUTPUT_AUDIO_DRIVER_DRIVERSDL_H_

#include "AudioDriver.h"
#include "misc/typeptr.h"

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
};

}

#endif //PLAYER_SRC_OUTPUT_AUDIO_DRIVER_DRIVERSDL_H_
