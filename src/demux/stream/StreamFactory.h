//
// Created by CrabExcavator on 2020/11/5.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_STREAMFACTORY_H
#define PLAYER_STREAMFACTORY_H

#include "Stream.h"

namespace demux::stream {

    /**
     * @brief the factory to decide which kind of stream to create
     */
    class StreamFactory {
    public:
        static stream_sptr create(const AVStream * stream_);
    };

}

#endif //PLAYER_STREAMFACTORY_H
