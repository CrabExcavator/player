//
// Created by CrabExcavator on 2020/11/5.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_VIDEOSTREAM_H
#define PLAYER_VIDEOSTREAM_H

#include "Stream.h"

namespace demux::stream {

    /**
     * @brief video stream
     */
    class VideoStream : public Stream {
    public:
        VideoStream() = default;

        common::error init(const demuxer_sptr& demuxer) override;
    };

}

#endif //PLAYER_VIDEOSTREAM_H
