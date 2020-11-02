//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_EVENT_H
#define PLAYER_EVENT_H

namespace input {

    /**
     * @brief event or input code used by player
     */
    enum class event {
        exit,
        key_up,
        key_down,
        key_left,
        key_right,
        window_resize,
        numOfEvent,
    };

}


#endif //PLAYER_EVENT_H
