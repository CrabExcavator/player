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
    enum class event : std::uint64_t {
        /// internal
        exit = 0x0000, /**< enum value 0x0000 */

        /// play
        nextEntry = 0x1000, /**< enum value 0x1000 */
        entryAvailable = 0x1001 /**< enum value 0x1001 */
    };

}


#endif //PLAYER_EVENT_H
