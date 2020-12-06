//
// Created by CrabExcavator on 2020/11/12.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "SampleFormat.h"

namespace output::audio {

std::map<SampleFormat, SampleFormatAttribute*> SampleFormatAttributeMap = {
    {SampleFormat::FLTP, new SampleFormatAttribute{
        .sample_format = SampleFormat::FLTP,
        .sample_size =  4
      }
    },
    {
        SampleFormat::S16P, new SampleFormatAttribute{
        .sample_format = SampleFormat::S16P,
        .sample_size = 2
      }
    },
    {
      SampleFormat::S16, new SampleFormatAttribute{
        .sample_format = SampleFormat::S16,
        .sample_size = 2
      }
    }
};

bool IsPlaneSampleFormat(SampleFormat format) {
  auto ret = false;

  if (SampleFormat::FLTP == format
  || SampleFormat::S16P == format) {
    ret = true;
  }
  return ret;
}

}