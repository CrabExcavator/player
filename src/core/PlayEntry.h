//
// Created by weihan on 2020/10/9.
//

#ifndef PLAYER_PLAYENTRY_H
#define PLAYER_PLAYENTRY_H

#include <string>
#include <memory>

namespace demux {
    class Demuxer;
}

namespace core {

    class PlayEntry;
    using play_entry_sptr = std::shared_ptr<PlayEntry>;

    enum class entry_type {
        file,
        numOfEntryType
    };

    class PlayEntry {
    public:
        friend class demux::Demuxer;
        PlayEntry() = delete;
        PlayEntry(entry_type type, std::string uri, int64_t last_pts);
        PlayEntry(const PlayEntry& rhs) = default;
        PlayEntry(PlayEntry&& rhs) = default;
        PlayEntry& operator = (const PlayEntry& rhs) = default;
        PlayEntry& operator = (PlayEntry&& rhs) = default;
        ~PlayEntry() = default;
        bool operator < (const PlayEntry&rhs) const {
            return (this->_type < rhs._type) || (this->_type == rhs._type && this->_uri < rhs._uri);
        }

    private:
        entry_type _type;
        std::string _uri;
        // this is last pts played
        int64_t _last_pts;
    };

}

#endif //PLAYER_PLAYENTRY_H
