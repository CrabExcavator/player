//
// Created by CrabExcavator on 2020/11/5.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_STREAM_H
#define PLAYER_STREAM_H

#include <folly/MPMCQueue.h>

#include "misc/typeptr.h"
#include "common/error.h"
#include "demux/Demuxer.h"
#include "demux/filter/FrameFilterBase.h"
#include "core/Output.h"

namespace demux::stream {

    /**
     * @brief stream interface
     */
    class Stream {
    public:
        friend class StreamFactory;
        using av_codec_ctx_uptr = std::unique_ptr<AVCodecContext, std::function<void(AVCodecContext*)>>;

        /**
         * @brief default
         */
        Stream() = default;

        /**
         * @brief get time base
         * @return time base
         */
        [[nodiscard]] std::chrono::nanoseconds getTimeBase() const;

        /**
         * @brief feed packet
         * @param [in] packet
         * @todo wrapper of packet
         * @return error code
         */
        common::error feed(const av_packet_sptr& packet);

        /**
         * @brief read a frame from stream
         * @param [out] frame
         * @return error code
         */
        common::error read(frame_sptr& frame);

        /**
         * @brief close stream
         * @attention if this func is override, it base impl should also be called
         * @return error code
         */
        virtual common::error close();

    public:
        /**
         * @brief output port
         */
        core::output_port op = core::output_port::null;

    protected:
        /**
         * @brief filter for frame
         */
        filter::frame_filter_chain_sptr _frame_filter_chain;

    private:
        /**
         * @brief set stream
         * @param [in] stream
         * @return error code
         */
        common::error setStream(const AVStream* stream);

    private:
        /**
         * @brief flag to mark first frame
         */
        bool _first = true;

        /**
         * @brief codec
         */
        AVCodec* _codec = nullptr;

        /**
         * @brief codec context
         */
        av_codec_ctx_uptr _codec_ctx;

        /**
         * @brief cached frame
         */
        std::shared_ptr<folly::MPMCQueue<frame_sptr>> _queue;

        /**
         * @brief time base
         */
        std::chrono::nanoseconds _time_base{};
    };

}

#endif //PLAYER_STREAM_H
