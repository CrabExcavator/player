//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_IMAGE_FORMAT_H
#define PLAYER_IMAGE_FORMAT_H

namespace video {

    /**
     * @brief image format supported by player
     */
    enum class image_format : std::int64_t {
        unknown = 0, /**< enum value 0 */
        yuv420p, /**< enum value 1 */
        numOfImgFmt, /**< enum value 2 */
    };
}

#endif //PLAYER_IMAGE_FORMAT_H
