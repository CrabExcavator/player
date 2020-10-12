//
// Created by weihan on 2020/10/12.
//

#ifndef PLAYER_TYPEPTR_H
#define PLAYER_TYPEPTR_H

#include <memory>

namespace core {
    class PlayEntry;
    using play_entry_sptr = std::shared_ptr<PlayEntry>;

    class PlayerContext;
    using player_ctx_sptr = std::shared_ptr<PlayerContext>;

    class PlayList;
    using play_list_sptr = std::shared_ptr<PlayList>;
}

namespace demux {
    class DemuxContext;
    using demux_ctx_sptr = std::shared_ptr<DemuxContext>;

    class Demuxer;
    using demuxer_sptr = std::shared_ptr<Demuxer>;

    class Frame;
    using frame_sptr = std::shared_ptr<Frame>;

    class Stream;
    using stream_sptr = std::shared_ptr<Stream>;
}

namespace input {
    class InputContext;
    using input_ctx_sptr = std::shared_ptr<InputContext>;
}

namespace video {
    class VideoOutput;
    using vo_sptr = std::shared_ptr<VideoOutput>;

    namespace driver {
        class Driver;
        using driver_uptr = std::unique_ptr<Driver>;
    }
}

#endif //PLAYER_TYPEPTR_H
