//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_PLAYENTRY_H
#define PLAYER_PLAYENTRY_H

#include "misc/typeptr.h"

#include <string>
#include <memory>

namespace demux {
    class Demuxer;
}

namespace core {

    enum class entry_type {
        file,
        numOfEntryType
    };

    class PlayEntry {
    public:
        PlayEntry() = delete;
        PlayEntry(entry_type type, std::string uri, int64_t last_pts);
        PlayEntry(const PlayEntry& rhs) = default;
        PlayEntry(PlayEntry&& rhs) = default;
        PlayEntry& operator = (const PlayEntry& rhs) = default;
        PlayEntry& operator = (PlayEntry&& rhs) = default;
        ~PlayEntry() = default;
        bool operator < (const PlayEntry&rhs) const {
            return (this->type < rhs.type) || (this->type == rhs.type && this->uri < rhs.uri);
        }

    public:
        entry_type type;
        std::string uri;
        // this is last pts played
        int64_t last_pts;
    };

}

#endif //PLAYER_PLAYENTRY_H
