//
// Created by weihan on 2020/10/9.
//

#ifndef PLAYER_PLAYENTRY_H
#define PLAYER_PLAYENTRY_H

#include <string>
#include <memory>

namespace core {

    class PlayEntry;
    using play_entry_sptr = std::shared_ptr<PlayEntry>;

    enum class entry_type {
        file,
        numOfEntryType
    };

    class PlayEntry {
    public:
        PlayEntry() = delete;
        PlayEntry(const PlayEntry& rhs) = default;
        PlayEntry(PlayEntry&& rhs) = default;
        PlayEntry& operator = (const PlayEntry& rhs) = default;
        PlayEntry& operator = (PlayEntry&& rhs) = default;
        ~PlayEntry() = default;

    private:
        entry_type _type;
        std::string _uri; // todo impl uri in misc
    };

}

#endif //PLAYER_PLAYENTRY_H
