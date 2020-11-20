//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SAMPLE_FORMAT_H
#define PLAYER_SAMPLE_FORMAT_H

#include <map>

namespace output::audio {

/**
 * @brief sample format supported by player
 */
enum class SampleFormat : std::int64_t {
  UNKNOWN = 0, /**< enum value 0 */
  FLTP, /**< enum value 1 */
};

struct SampleFormatAttribute {
  SampleFormat sample_format;
  int sample_size;
};

extern std::map<SampleFormat, SampleFormatAttribute*> SampleFormatAttributeMap;

};

#endif //PLAYER_SAMPLE_FORMAT_H
