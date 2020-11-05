//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
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
#include "common/error.h"

namespace demux {

    using av_packet_sptr = std::shared_ptr<AVPacket>;

    /**
     * @brief demuxer used for entry
     */
    class Demuxer: public std::enable_shared_from_this<Demuxer> {
    public:
        /**
         * @brief default
         */
        Demuxer();

        /**
         * @brief setNumOfStream
         * @param [in] entry the entry to demux
         * @param [in] demux_ctx demux context is used to pass input queue to stream
         * @return error code
         */
        common::error init(const core::play_entry_sptr& entry, const demux_ctx_sptr& demux_ctx);

        /**
         * @brief default
         */
        ~Demuxer();

        /**
         * @brief handle next packet
         * @return error code
         */
        common::error epoch();

        /**
         * @brief some stream should feed empty packet to flush
         * @return error code
         */
        common::error flush();

        /**
         * @brief close demuxer
         * @return error code
         */
        common::error close();

    private:
        /**
         * @brief format context
         */
        std::shared_ptr<AVFormatContext> _av_format_ctx;

        /**
         * @brief list of stream
         */
        std::vector<stream::stream_sptr> _streams;

        /**
         * @brief packet to fill
         */
        av_packet_sptr _av_packet;
    };

}

#endif //PLAYER_DEMUXER_H
