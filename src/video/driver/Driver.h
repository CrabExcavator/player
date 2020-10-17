//
// Created by weihan on 2020/10/7.
//

#ifndef PLAYER_DRIVER_H
#define PLAYER_DRIVER_H

#include <memory>

#include "misc/typeptr.h"

namespace video::driver {

    class Driver {
    public:
        Driver() = default;

        virtual ~Driver() = 0;

        virtual void init(vo_sptr vo) = 0;

        virtual void drawImage(vo_sptr vo) = 0;

        virtual void waitEvents(vo_sptr vo) = 0;

        virtual void reConfig(vo_sptr vo) = 0;
    };

}

#endif //PLAYER_DRIVER_H