//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_ERROR_H
#define PLAYER_ERROR_H

#include <cstdint>

namespace common {

    /**
     * @brief error code used by player, error lt 0 is exception
     */
    enum class error : std::int64_t {
        /// error code of stream
        streamUnknownError = -4000, /**< enum value -4000 */
        streamFeedPacketFail, /**< enum value -3999 */
        streamInitFail, /**< enum value -3998 */

        /// error code of video
        videoUnknownError = -3000, /**< enum value -3000 */
        videoDriverInitFail, /**< enum value -2999 */

        /// error code of audio
        audioUnknownError = -2000, /**< enum value -2000 */
        audioDriverInitFail, /**< enum value -1999 */
        audioDriverPlayFail, /**< enum value -1998 */

        unknownError = -1000, /**< enum value -1000 */

        /// no error
        success = 0, /**< enum value 0 */
        exit, /**< enum value 1 */
        eof , /**< enum value 2 */
    };

}

#endif //PLAYER_ERROR_H
