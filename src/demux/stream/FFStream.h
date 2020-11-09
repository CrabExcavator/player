//
// Created by CrabExcavator on 2020/11/9.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_FFSTREAM_H
#define PLAYER_FFSTREAM_H

#include <folly/MPMCQueue.h>

#include "IStream.h"
#include "misc/avheader.h"
#include "demux/filter/FrameFilterBase.h"

namespace demux::stream {

    /**
     * @brief av stream
     */
    class FFStream : public IStream {
    public:
        /**
         * @brief init avS
         * @param stream
         * @return
         */
        common::error init(const AVStream* stream);

        /**
         * @brief read a frame
         * @param [out] frame
         * @return error code
         */
        common::error read(frame_sptr& frame) override;

        /**
         * @brief close stream
         * @return error code
         */
        common::error close() override;

        /**
         * @brief get time base of stream
         * @return time base
         */
        std::chrono::nanoseconds getTimeBase() const override;

        /**
         * @brief what type of output should this stream bind to
         * @return output port
         */
        core::output_port getOutputPort() const override;

        /**
         * @brief feed packet
         * @param [in] packet
         * @return error code
         */
        common::error feed(const av_packet_sptr& packet);

    private:
        bool _first = true;

        AVCodec* _codec = nullptr;

        av_codec_ctx_uptr _codec_ctx = nullptr;

        std::shared_ptr<folly::MPMCQueue<frame_sptr>> _queue = nullptr;

        std::chrono::nanoseconds _time_base{};

        filter::frame_filter_chain_sptr _frame_filter_chain = nullptr;

        core::output_port _op = core::output_port::null;
    };

}

#endif //PLAYER_FFSTREAM_H
