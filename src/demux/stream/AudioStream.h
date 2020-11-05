//
// Created by CrabExcavator on 2020/11/5.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_AUDIOSTREAM_H
#define PLAYER_AUDIOSTREAM_H

#include "Stream.h"

namespace demux::stream {

    /**
     * @brief audio stream
     */
    class AudioStream : public Stream {
    public:
        AudioStream() = default;

        common::error init(const demuxer_sptr& demuxer) override;
    };

}

#endif //PLAYER_AUDIOSTREAM_H
