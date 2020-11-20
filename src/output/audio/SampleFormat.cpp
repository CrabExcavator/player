//
// Created by CrabExcavator on 2020/11/12.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "SampleFormat.h"

namespace output::audio {

std::map<SampleFormat, SampleFormatAttribute*> SampleFormatAttributeMap = {
    {SampleFormat::FLTP, new SampleFormatAttribute{
        .sample_format =  SampleFormat::FLTP,
        .sample_size =  4
    }}
};

}