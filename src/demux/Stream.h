//
// Created by weihan on 2020/10/11.
//

#ifndef PLAYER_STREAM_H
#define PLAYER_STREAM_H

#include <memory>
#include <functional>

#include "Frame.h"
#include "Demuxer.h"
#include "misc/typeptr.h"

namespace demux {

    class Stream {
    public:
        Stream() = delete;
        Stream(const std::shared_ptr<Demuxer>& demuxer, int index);
        Stream(const Stream& rhs) = delete;
        Stream(Stream&& rhs) = default;
        Stream& operator = (const Stream& rhs) = delete;
        Stream& operator = (Stream&& rhs) = default;
        void feed(const av_packet_sptr& packet);

    private:
        using av_codec_ctx_uptr = std::unique_ptr<AVCodecContext, std::function<void(AVCodecContext*)>>;
        std::weak_ptr<Demuxer> _demuxer;
        int _index;
        av_codec_ctx_uptr _av_codec_ctx;
        frame_sptr _frame;
    };

}

#endif //PLAYER_STREAM_H
