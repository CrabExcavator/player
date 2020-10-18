//
// Created by weihan on 2020/10/9.
//

#include <core/PlayEntry.h>

#include "Stream.h"
#include "Demuxer.h"
#include "exception/InitException.h"
#include "DemuxContext.h"

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

    Demuxer::Demuxer(const core::play_entry_sptr& entry): _streams() {
        this->_entry = entry;
        this->_base_pts = entry->_last_pts;
        this->_av_format_ctx.reset(avformat_alloc_context(), avformat_close_input_wrapper);
        bool success = true;
        success = (this->_av_format_ctx != nullptr);
        if (success) {
            auto raw_ptr = this->_av_format_ctx.get();
            success = (avformat_open_input(&raw_ptr, entry->_uri.c_str(), nullptr, nullptr) >= 0);
        }
        if (!success) {
            throw exception::DemuxerInitException();
        }
    }

    void Demuxer::init(const demux_ctx_sptr& demux_ctx) {
        bool success = true;
        success  = (avformat_find_stream_info(this->_av_format_ctx.get(), nullptr) >= 0);
        if (success) {
            for (int stream_index = 0 ; stream_index < this->_av_format_ctx->nb_streams ; stream_index++) {
                shared_from_this();
                auto stream = std::make_shared<Stream>(shared_from_this(), stream_index, demux_ctx->queue);
                stream->init();
                this->_streams.emplace_back(stream);
            }
        }
        if (success) {
            this->_av_packet.reset(av_packet_alloc(), av_packet_free_wrapper);
            success = (this->_av_packet != nullptr);
        }
        if (!success) {
            throw exception::DemuxerInitException();
        }
    }

    int Demuxer::epoch() {
        int ret = av_read_frame(this->_av_format_ctx.get(), this->_av_packet.get());
        if (ret >= 0) {
            if (this->_av_packet->stream_index == 0) {
                this->_streams.at(this->_av_packet->stream_index)->feed(this->_av_packet);
            }
            av_packet_unref(this->_av_packet.get());
        }
        return ret;
    }

}
