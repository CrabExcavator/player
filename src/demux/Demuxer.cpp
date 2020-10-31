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

    Demuxer::Demuxer(): _streams() {

    }

    void Demuxer::init(const core::play_entry_sptr& entry, const demux_ctx_sptr& demux_ctx) {
        this->_entry = entry;
        this->_base_pts = entry->last_pts;
        this->_av_format_ctx.reset(avformat_alloc_context(), avformat_close_input_wrapper);
        bool success = true;
        success = (this->_av_format_ctx != nullptr);
        if (success) {
            auto raw_ptr = this->_av_format_ctx.get();
            success = (avformat_open_input(&raw_ptr, entry->uri.c_str(), nullptr, nullptr) >= 0);
        }
        if (success) {
            success = (avformat_find_stream_info(this->_av_format_ctx.get(), nullptr) >= 0);
        }
        if (success) {
            for (int stream_index = 0 ; stream_index < this->_av_format_ctx->nb_streams ; stream_index++) {
                auto stream = std::make_shared<Stream>();
                stream->init(this->_av_format_ctx, stream_index, demux_ctx);
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
            //if (this->_av_packet->stream_index == 1) {
                this->_streams.at(this->_av_packet->stream_index)->feed(this->_av_packet);
            //}
            av_packet_unref(this->_av_packet.get());
        }
        return ret;
    }

    int Demuxer::flush() {
        int stream_index = 0;
        for (auto& stream : this->_streams) {
            av_packet_unref(this->_av_packet.get());
            this->_av_packet.get()->stream_index = stream_index++;
            stream->feed(this->_av_packet);
        }
        return 0;
    }

}
