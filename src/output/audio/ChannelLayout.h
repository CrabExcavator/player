//
// Created by CrabExcavator on 2020/11/23.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_OUTPUT_AUDIO_CHANNELLAYOUT_H_
#define PLAYER_SRC_OUTPUT_AUDIO_CHANNELLAYOUT_H_

#include <cstdint>
#include <map>

namespace output::audio {

/**
 * @brief channel layout
 */
enum class ChannelLayout : std::int64_t {
  UNKNOWN = 0, /**< enum value 0 */
  STEREO, /**< enum value 1 */
  SURROUND, /**< enum value 2 */
};

struct ChannelLayoutAttribute {
  ChannelLayout channel_layout;
  int number_of_channel;
};

extern std::map<ChannelLayout, ChannelLayoutAttribute*> ChannelLayoutAttributeMap;

}

#endif //PLAYER_SRC_OUTPUT_AUDIO_CHANNELLAYOUT_H_
