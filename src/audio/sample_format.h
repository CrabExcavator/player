//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SAMPLE_FORMAT_H
#define PLAYER_SAMPLE_FORMAT_H

namespace audio {

    /**
     * @brief sample format supported by player
     */
    enum class sample_format : std::int64_t {
        unknown = 0, /**< enum value 0 */
        fltp, /**< enum value 1 */
        numOfSampleFmt /**< enum value 2 */
    };

}

#endif //PLAYER_SAMPLE_FORMAT_H
