//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_VIDEODRIVER_H
#define PLAYER_VIDEODRIVER_H

#include <memory>

#include "misc/typeptr.h"

namespace video::driver {

    class VideoDriver {
    public:
        VideoDriver() = default;

        virtual ~VideoDriver() = default;

        virtual void init(vo_sptr vo) = 0;

        virtual void drawImage(vo_sptr vo) = 0;

        virtual void waitEvents(vo_sptr vo) = 0;

        virtual void reConfig(vo_sptr vo) = 0;
    };

}

#endif //PLAYER_VIDEODRIVER_H