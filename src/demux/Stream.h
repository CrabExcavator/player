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
#include "common/error.h"

namespace demux {

    /**
     * @brief process kinds of packet
     * @todo an abstract class of Stream
     */
    class Stream: public std::enable_shared_from_this<Stream> {
    public:
        friend class filter::Fill;

        /**
         * @brief default
         */
        Stream() = default;

        /**
         * @brief init stream
         * @param [in] av_fmt_ctx_
         * @param [in] index index of stream
         * @param [in] demux_ctx pass output queue
         * @return error code
         */
        common::error init(const std::shared_ptr<AVFormatContext>& av_fmt_ctx_, int index, const demux_ctx_sptr& demux_ctx);

        /**
         * @brief default
         * @param rhs
         */
        Stream(const Stream& rhs) = delete;

        /**
         * @brief default
         * @param rhs
         */
        Stream(Stream&& rhs) = default;

        /**
         * @brief default
         * @param rhs
         * @return
         */
        Stream& operator = (const Stream& rhs) = delete;

        /**
         * @brief default
         * @param rhs
         * @return
         */
        Stream& operator = (Stream&& rhs) = default;

        /**
         * @brief feed packet
         * @param packet the packet to feed
         * @return error code
         */
        common::error feed(const av_packet_sptr& packet);

        /**
         * @brief time base of stream
         * @return time base in nanoseconds
         */
        std::chrono::nanoseconds timeBase() const;

        /**
         * @brief close stream
         * @return error code
         */
        common::error close();

    public:
        using av_codec_ctx_uptr = std::unique_ptr<AVCodecContext, std::function<void(AVCodecContext*)>>;

        /**
         * @brief av codec
         */
        av_codec_ctx_uptr av_codec_ctx;

        /**
         * @brief output queue
         */
        std::shared_ptr<folly::MPMCQueue<demux::frame_sptr>> queue;

    private:
        /**
         * @brief index of stream in fmt context
         */
        int _index{};

        /**
         * @brief internal frame
         */
        frame_sptr _frame;

        /**
         * @brief filter chain for frame
         */
        filter::frame_filter_chain_sptr _frame_filter_chain;

        /**
         * @brief fmt context
         */
        std::shared_ptr<AVFormatContext> _av_fmt_ctx;

        /**
         * @brief flag to mark first frame
         */
        bool _first = true;
    };

}

#endif //PLAYER_STREAM_H
