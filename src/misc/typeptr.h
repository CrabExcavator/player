//
// Created by weihan on 2020/10/12.
//

#ifndef PLAYER_TYPEPTR_H
#define PLAYER_TYPEPTR_H

#include <memory>
#include <vector>

namespace misc {
    template <typename T>
    using vector_sptr = std::shared_ptr<std::vector<T>>;

    template <typename T>
    class ChainNode;
    template <typename T>
    using chain_node_sptr = std::shared_ptr<ChainNode<T>>;

    template <typename T>
    class Chain;
    template <typename T>
    using chain_sptr = std::shared_ptr<Chain<T>>;
}

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
    using stream_wptr = std::weak_ptr<Stream>;
}

namespace input {
    class InputContext;
    using input_ctx_sptr = std::shared_ptr<InputContext>;
}

namespace video {
    class VideoOutput;
    using vo_sptr = std::shared_ptr<VideoOutput>;

    namespace driver {
        class VideoDriver;
        using video_driver_uptr = std::unique_ptr<VideoDriver>;
    }
}

namespace audio {
    class AudioOutput;
    using ao_sptr = std::shared_ptr<AudioOutput>;

    namespace driver {
        class AudioDriver;
        using audio_driver_uptr = std::unique_ptr<AudioDriver>;
    }
}

#endif //PLAYER_TYPEPTR_H
