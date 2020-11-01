//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_STREAM_H
#define PLAYER_STREAM_H

#include <memory>
#include <functional>
#include <folly/MPMCQueue.h>

#include "Frame.h"
#include "Demuxer.h"
#include "misc/typeptr.h"
#include "filter/FrameFilterBase.h"
#include "filter/Fill.h"

namespace demux {

    /**
     * @brief process kinds of packet
     * @todo an abstract class of Stream
     */
    class Stream: public std::enable_shared_from_this<Stream> {
    public:
        friend class filter::Fill;
        Stream() = default;
        void init(const std::shared_ptr<AVFormatContext>& av_fmt_ctx_, int index, const demux_ctx_sptr& demux_ctx);
        Stream(const Stream& rhs) = delete;
        Stream(Stream&& rhs) = default;
        Stream& operator = (const Stream& rhs) = delete;
        Stream& operator = (Stream&& rhs) = default;

        void feed(const av_packet_sptr& packet);

        // return timeBase of stream in nanoseconds
        std::chrono::nanoseconds timeBase() const;

        void close();

    public:
        using av_codec_ctx_uptr = std::unique_ptr<AVCodecContext, std::function<void(AVCodecContext*)>>;
        av_codec_ctx_uptr av_codec_ctx;
        std::shared_ptr<folly::MPMCQueue<demux::frame_sptr>> queue;

    private:
        int _index{};
        frame_sptr _frame;
        filter::frame_filter_chain_sptr _frame_filter_chain;
        std::shared_ptr<AVFormatContext> _av_fmt_ctx;
        bool _first = true;
    };

}

#endif //PLAYER_STREAM_H
