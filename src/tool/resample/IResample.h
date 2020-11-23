//
// Created by CrabExcavator on 2020/11/23.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_UTIL_RESAMPLE_IRESAMPLE_H_
#define PLAYER_SRC_UTIL_RESAMPLE_IRESAMPLE_H_

#include "output/audio/SampleFormat.h"
#include "output/audio/ChannelLayout.h"

namespace tool::resample {

struct Dest {
  output::audio::SampleFormat sample_format;
  output::audio::ChannelLayout layout;
  int number_of_channel;
  int number_of_sample;
  int sample_rate;
  int linesize;
};

class IResample {

};

}

#endif //PLAYER_SRC_UTIL_RESAMPLE_IRESAMPLE_H_
