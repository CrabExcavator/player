//
// Created by CrabExcavator on 2020/11/23.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_UTIL_RESAMPLE_IRESAMPLE_H_
#define PLAYER_SRC_UTIL_RESAMPLE_IRESAMPLE_H_

#include "output/audio/SampleFormat.h"
#include "output/audio/ChannelLayout.h"
#include "common/Error.h"
#include "misc/Slice.h"
#include "misc/typeptr.h"

namespace tool::resample {

/**
 * @brief this is struct used to describe input frame and resample output
 *
 * sample_format, layout, sample_rate in used for Resample Init
 * number_of_channel, number_of_sample, linesize if used for resampling func
 */
struct Desc {
  output::audio::SampleFormat sample_format;
  output::audio::ChannelLayout layout;
  int sample_rate;
  int number_of_channel;
  int number_of_sample;
  int linesize;
};

class IResampleOutput {
 public:
  virtual ~IResampleOutput() = default;

  virtual common::Error GetData(misc::vector_sptr<misc::Slice> &out) = 0;

  virtual common::Error GetDesc(Desc &desc) = 0;
};

class IResample {
 public:
  virtual ~IResample() = default;

  virtual common::Error operator() (const uint8_t **src_data,
      int number_of_sample,
      int line_size,
      resample_output_sptr &resample_output) = 0;
};

}

#endif //PLAYER_SRC_UTIL_RESAMPLE_IRESAMPLE_H_
