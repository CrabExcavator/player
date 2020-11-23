//
// Created by CrabExcavator on 2020/11/23.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "ChannelLayout.h"

namespace output::audio {

std::map<ChannelLayout, ChannelLayoutAttribute*> ChannelLayoutAttributeMap = {
    {ChannelLayout::STEREO, new ChannelLayoutAttribute{
      .channel_layout = ChannelLayout::STEREO,
      .number_of_channel = 2
    }}
};

}
