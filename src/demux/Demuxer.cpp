//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "Demuxer.h"
#include "DemuxContext.h"
#include "core/PlayEntry.h"
#include "core/SyncContext.h"
#include "stream/StreamFactory.h"

namespace demux {

    static void avformat_close_input_wrapper(AVFormatContext* ctx) {
        avformat_close_input(&ctx);
    }

    static void av_packet_free_wrapper(AVPacket* pkt) {
        av_packet_free(&pkt);
    }

    Demuxer::~Demuxer() {
        this->_av_format_ctx = nullptr;
    }

    Demuxer::Demuxer(): _streams() {

    }

    common::error Demuxer::init(const core::play_entry_sptr& entry, const demux_ctx_sptr& demux_ctx) {
        this->_av_format_ctx.reset(avformat_alloc_context(), avformat_close_input_wrapper);
        if (this->_av_format_ctx == nullptr) {
            return common::error::demuxerInitFail;
        }
        auto raw_ptr = this->_av_format_ctx.get();
        if (avformat_open_input(&raw_ptr, entry->uri.c_str(), nullptr, nullptr) < 0) {
            return common::error::demuxerInitFail;
        } else if (avformat_find_stream_info(raw_ptr, nullptr) < 0) {
            return common::error::demuxerInitFail;
        }
        for (int stream_index = 0 ; stream_index < this->_av_format_ctx->nb_streams ; stream_index++) {
            auto stream = stream::StreamFactory::create(
                    this->_av_format_ctx->streams[stream_index]);
            stream->init(shared_from_this());
            this->_streams.emplace_back(stream);
            demux_ctx->sync_ctx->addStream(stream);
        }
        this->_av_packet.reset(av_packet_alloc(), av_packet_free_wrapper);
        if (this->_av_packet == nullptr) {
            return common::error::demuxerInitFail;
        }
        return common::error::success;
    }

    common::error Demuxer::epoch() {
        int ret = av_read_frame(this->_av_format_ctx.get(), this->_av_packet.get());
        if (ret >= 0) {
            this->_streams.at(this->_av_packet->stream_index)->feed(this->_av_packet);
            av_packet_unref(this->_av_packet.get());
        }
        return ret < 0 ? common::error::eof : common::error::success;
    }

    common::error Demuxer::flush() {
        int stream_index = 0;
        for (auto& stream : this->_streams) {
            av_packet_unref(this->_av_packet.get());
            this->_av_packet.get()->stream_index = stream_index++;
            stream->feed(this->_av_packet);
        }
        return common::error::success;
    }

    common::error Demuxer::close() {
        for (auto& stream : this->_streams) {
            stream->close();
        }
        return common::error::success;
    }

}
