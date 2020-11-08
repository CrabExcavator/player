//
// Created by CrabExcavator on 2020/11/8.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_DEMUXERFACTORY_H
#define PLAYER_DEMUXERFACTORY_H

#include "IDemuxer.h"

namespace demux::demuxer {

    /**
     * @brief the factory to decide which kind of demuxer to create
     */
    class DemuxerFactory {
    public:
        static demuxer_sptr create(const std::string& name);
    };

}

#endif //PLAYER_DEMUXERFACTORY_H
