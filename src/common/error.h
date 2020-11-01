//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_ERROR_H
#define PLAYER_ERROR_H

namespace common {

    /**
     * @brief error code used by player, error lt 0 is exception
     */
    enum class error {
        unknown = -1000,
        success = 0,
        eof,
    };

}

#endif //PLAYER_ERROR_H
