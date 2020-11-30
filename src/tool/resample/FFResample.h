//
// Created by CrabExcavator on 2020/11/23.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_UTIL_RESAMPLE_FFRESAMPLE_H_
#define PLAYER_SRC_UTIL_RESAMPLE_FFRESAMPLE_H_

#include <glog/logging.h>

#include "IResample.h"
#include "common/Error.h"
#include "misc/avheader.h"

namespace tool::resample {

class FFResampleOutput : public IResampleOutput {
 public:
  ~FFResampleOutput() override;

  common::Error Init(uint8_t **data, Desc desc);

  common::Error GetData(misc::vector_sptr<misc::Slice> &out) override;

  common::Error GetDesc(Desc &desc) override;

 private:
  uint8_t **data_;
  Desc desc_;
};

class FFResample : public IResample {
 public:
  ~FFResample() override;

  common::Error Init(Desc src, Desc dst);

  common::Error operator () (const uint8_t **src_data,
                             int number_of_sample,
                             int line_size,
                             resample_output_sptr &resample_output) override;

 private:
  static uint64_t ChannelLayoutTranslate(output::audio::ChannelLayout channel_layout);

  static AVSampleFormat SampleFormatTranslate(output::audio::SampleFormat sample_format);

 private:
  swr_ctx_uptr swr_ctx_;
  Desc src_;
  Desc dst_;
};

}

#endif //PLAYER_SRC_UTIL_RESAMPLE_FFRESAMPLE_H_
