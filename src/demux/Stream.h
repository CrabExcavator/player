//
// Created by weihan on 2020/10/11.
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

    class Stream: public std::enable_shared_from_this<Stream> {
    public:
        friend class filter::Fill;
        Stream() = delete;
        Stream(const std::shared_ptr<Demuxer>& demuxer, int index, std::shared_ptr<folly::MPMCQueue<demux::frame_sptr>> queue);
        void init();
        Stream(const Stream& rhs) = delete;
        Stream(Stream&& rhs) = default;
        Stream& operator = (const Stream& rhs) = delete;
        Stream& operator = (Stream&& rhs) = default;
        void feed(const av_packet_sptr& packet);
        // return timeBase of stream in nanoseconds
        std::chrono::nanoseconds timeBase() const;

    public:
        using av_codec_ctx_uptr = std::unique_ptr<AVCodecContext, std::function<void(AVCodecContext*)>>;
        av_codec_ctx_uptr av_codec_ctx;
        std::shared_ptr<folly::MPMCQueue<demux::frame_sptr>> queue;

    private:
        std::weak_ptr<Demuxer> _demuxer;
        int _index;
        frame_sptr _frame;
        filter::frame_filter_chain_sptr _frame_filter_chain;
        bool _first = true;
    };

}

#endif //PLAYER_STREAM_H
