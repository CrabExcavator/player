//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_TYPEPTR_H
#define PLAYER_TYPEPTR_H

#include <memory>
#include <vector>
#include <array>

namespace common {
class Config;
using config_sptr = std::shared_ptr<Config>;

//class SyncContext;
//using sync_ctx_sptr = std::shared_ptr<SyncContext>;
}

namespace misc {
template<typename T, size_t Size>
using array_sptr = std::shared_ptr<std::array<T, Size>>;

template<typename T>
using vector_sptr = std::shared_ptr<std::vector<T>>;

template<typename T>
class ChainNode;
template<typename T>
using chain_node_sptr = std::shared_ptr<ChainNode<T>>;

template<typename T>
class Chain;
template<typename T>
using chain_sptr = std::shared_ptr<Chain<T>>;

class Runnable;
using runnable_sptr = std::shared_ptr<Runnable>;
}

namespace player {
class PlayEntry;
using play_entry_sptr = std::shared_ptr<PlayEntry>;

class PlayerContext;
using player_ctx_sptr = std::shared_ptr<PlayerContext>;
using player_ctx_wptr = std::weak_ptr<PlayerContext>;

class PlayList;
using play_list_sptr = std::shared_ptr<PlayList>;
}

namespace demux {
class DemuxContext;
using demux_ctx_sptr = std::shared_ptr<DemuxContext>;

namespace stream {
class IStream;
using stream_sptr = std::shared_ptr<IStream>;

class FFStream;
using ffstream_sptr = std::shared_ptr<FFStream>;
}

namespace frame {
class IFrame;
using frame_sptr = std::shared_ptr<IFrame>;

class FFFrame;
using ffframe_sptr = std::shared_ptr<FFFrame>;
}

namespace demuxer {
class IDemuxer;
using demuxer_sptr = std::shared_ptr<IDemuxer>;
}
}

namespace input {
class InputContext;
using input_ctx_sptr = std::shared_ptr<InputContext>;

namespace handler {
using event_handler_chain_sptr = misc::chain_sptr<input_ctx_sptr>;
}
}

namespace output {

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

}

namespace tool {

namespace resample {
class IResample;
using resample_sptr = std::shared_ptr<IResample>;

class IResampleOutput;
using resample_output_sptr = std::shared_ptr<IResampleOutput>;
}

}

#endif //PLAYER_TYPEPTR_H
