//
// Created by CrabExcavator on 2020/11/23.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "ChannelLayout.h"

namespace output::audio {

std::map<ChannelLayout, ChannelLayoutAttribute*> ChannelLayoutAttributeMap = {
    {
      ChannelLayout::STEREO, new ChannelLayoutAttribute{
      .channel_layout = ChannelLayout::STEREO,
      .number_of_channel = 2
      }
    },
    {
      ChannelLayout::SURROUND, new ChannelLayoutAttribute{
        .channel_layout = ChannelLayout::SURROUND,
        .number_of_channel = 3
      }
    },
    {
      ChannelLayout::_5POINT1, new ChannelLayoutAttribute{
        .channel_layout = ChannelLayout::_5POINT1,
        .number_of_channel = 6
      }
    },
    {
        ChannelLayout::_5POINT1_BACK, new ChannelLayoutAttribute{
        .channel_layout = ChannelLayout::_5POINT1_BACK,
        .number_of_channel = 6
        }
    }
};

}
