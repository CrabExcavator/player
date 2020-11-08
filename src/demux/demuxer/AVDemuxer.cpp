//
// Created by CrabExcavator on 2020/11/8.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include <glog/logging.h>

#include "AVDemuxer.h"
#include "core/PlayEntry.h"
#include "demux/stream/StreamFactory.h"

namespace demux::demuxer {

    static void avformat_close_input_wrapper(AVFormatContext* ctx) {
        avformat_close_input(&ctx);
    }

    static void av_packet_free_wrapper(AVPacket* pkt) {
        av_packet_free(&pkt);
    }

    common::error AVDemuxer::open(const core::play_entry_sptr &entry, misc::vector_sptr<stream::stream_sptr>& streams) {
        /** @attention all [out] pointer parm should be nullptr */
        assert(streams == nullptr);
        this->_streams = std::make_shared<std::vector<stream::stream_sptr>>();
        this->_av_format_ctx.reset(avformat_alloc_context(), avformat_close_input_wrapper);
        this->_av_packet.reset(av_packet_alloc(), av_packet_free_wrapper);
        if (this->_av_format_ctx == nullptr) {
            LOG(ERROR) << "create av format context fail";
            return common::error::demuxerInitFail;
        } else if (this->_av_packet == nullptr) {
            LOG(ERROR) << "create av packet fail";
            return common::error::demuxerInitFail;
        }
        auto raw_av_format_ctx = this->_av_format_ctx.get();
        if (avformat_open_input(&raw_av_format_ctx, entry->uri.c_str(), nullptr, nullptr) < 0) {
            LOG(ERROR) << "open av format context fail";
            return common::error::demuxerInitFail;
        } else if (avformat_find_stream_info(raw_av_format_ctx, nullptr) < 0) {
            LOG(ERROR) << "can't find all stream info";
            return common::error::demuxerInitFail;
        }
        for (int stream_index = 0 ; stream_index < this->_av_format_ctx->nb_streams ; stream_index++) {
            /// @todo add stream to output
            auto stream = stream::StreamFactory::create(
                    this->_av_format_ctx->streams[stream_index]);
            this->_streams->emplace_back(stream);
        }
        streams = this->_streams;
        return common::error::success;
    }

    common::error AVDemuxer::epoch() {
        if (av_read_frame(this->_av_format_ctx.get(), this->_av_packet.get()) < 0) {
            return common::error::eof;
        }
        this->_streams->at(this->_av_packet->stream_index)->feed(this->_av_packet);
        av_packet_unref(this->_av_packet.get());
        return common::error::success;
    }

    common::error AVDemuxer::close() {
        for (int stream_index = 0 ; stream_index < this->_streams->size() ; stream_index++) {
            av_packet_unref(this->_av_packet.get());
            this->_av_packet.get()->stream_index = stream_index;
            auto stream = this->_streams->at(stream_index);
            stream->feed(this->_av_packet);
            stream->close();
        }
        return common::error::success;
    }

}
