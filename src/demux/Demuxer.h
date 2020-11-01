//
// Created by weihan on 2020/10/9.
//

#ifndef PLAYER_DEMUXER_H
#define PLAYER_DEMUXER_H

#include <memory>
#include <vector>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

#include "core/PlayEntry.h"
#include "misc/typeptr.h"

namespace demux {

    using av_packet_sptr = std::shared_ptr<AVPacket>;

    class Stream;

    class Demuxer: public std::enable_shared_from_this<Demuxer> {
    public:
        Demuxer();
        void init(const core::play_entry_sptr& entry, const demux_ctx_sptr& demux_ctx);
        ~Demuxer();
        int epoch();
        int flush();
        int close();
        int nbStreams() const;

    private:
        int64_t _base_pts{};
        std::weak_ptr<core::PlayEntry> _entry;
        std::shared_ptr<AVFormatContext> _av_format_ctx;
        std::vector<std::shared_ptr<Stream>> _streams;
        av_packet_sptr _av_packet;
    };

    using demuxer_sptr = std::shared_ptr<Demuxer>;

}

#endif //PLAYER_DEMUXER_H
