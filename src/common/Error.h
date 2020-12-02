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
enum class Error : std::int64_t {
  SDL_ERR_UNKNOWN = -15000, /**< enum value -15000 */
  SDL_ERR_INIT, /**< enum value -14999 */
  SDL_ERR_CREATE_WINDOW, /**< enum value -14998 */
  SDL_ERR_CREATE_RENDERER, /**< enum value -14997 */

  /// error code of demux
  demuxUnknownError = -5000, /**< enum value -5000 */
  demuxerInitFail = -4999, /**< enum value -4999 */

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

  UNKNOWN_ERROR = -1000, /**< enum value -1000 */
  OUT_OF_MEMORY = -999, /**< enum value -999 */
  INVALID_ARGS = -998, /**< enum value -998 */

  /// no error
  SUCCESS = 0, /**< enum value 0 */
  EXIT, /**< enum value 1 */
  END, /**< enum value 2 */
  UN_READ, /**< enum value 3 */
  noStream, /**< enum value 4 */
};

}

#endif //PLAYER_ERROR_H
