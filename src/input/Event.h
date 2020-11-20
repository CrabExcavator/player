//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_EVENT_H
#define PLAYER_EVENT_H

#include <cstdint>

namespace input {

/**
 * @brief event or input code used by player
 */
enum class Event : std::uint64_t {
  /// internal
  EXIT = 0x0000, /**< enum value 0x0000 */
};

}

#endif //PLAYER_EVENT_H
