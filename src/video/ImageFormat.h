//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_IMAGE_FORMAT_H
#define PLAYER_IMAGE_FORMAT_H

#include <map>

namespace video {

/**
 * @brief image format supported by player
 */
enum class ImageFormat : std::int64_t {
  unknown = 0, /**< enum value 0 */
  yuv420p, /**< enum value 1 */
};

struct ImageFormatAttribute {
  ImageFormat image_format;
};

extern std::map<ImageFormat, ImageFormatAttribute*> ImageFormatAttributeMap;

}

#endif //PLAYER_IMAGE_FORMAT_H
